#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gto_rom.h"
//#include "bp_rom.h"

#define ROMAD_1024_START_INDEX 0x3FF
#define ROMAD_512_START_INDEX 0xBFF
#define ROMAD_MAX_INDEX 0x7FF

#define TO_16X 1
static unsigned int ram[128];

static int ram_w_index;
static int ram_r_index;

static long accum;
static void calc_one_sample(unsigned int sample, int up_times)
{
    int romad;
    int romaddr;
    int data_rdo;
    int coef;
    int i;
    long mult;
    long firculc;
    int max_calc = 1024;
    int calc_times = 64;

    ram_r_index = ram_w_index + 1;

    switch(up_times){
        case 16:
            romad = ROMAD_1024_START_INDEX;
            break;
        case 8:
            romad = (ROMAD_1024_START_INDEX >> 1) | (1 << 10);
            max_calc >>= 1;
            calc_times >>= 1;
            break;
        case 4:
            romad = (ROMAD_1024_START_INDEX >> 2) | (3 << 9);
            max_calc >>= 2;
            calc_times >>= 2;
            break;
        case 2:
            romad = (ROMAD_1024_START_INDEX >> 3) | (7 << 8);
            max_calc >>= 3;
            calc_times >>= 3;
            break;
    }
    //printf("in:0x%08x\n", sample);
    for(i=0; i<max_calc; i++){
        romad = (romad - 1) & 0x7FF;
        switch(up_times){
            case 16:
                romaddr = (romad & (1<<10)) | ((romad & (0x3f)) << 4) | ( ((romad >> 6) & 0xf) ^ 0xf);
                break;
            case 8:
                romaddr = (romad & (0x3<<9)) | ((romad & (0x3f)) << 3) | ( ((romad >> 6) & 0x7) ^ 0x7);
                break;
            case 4:
                romaddr = (romad & (0x7<<8)) | ((romad & (0x3f)) << 2) | ( ((romad >> 6) & 0x3) ^ 0x3);
                break;
            case 2:
                romaddr = (romad & (0xf<<7)) | ((romad & (0x3f)) << 1) | ( ((romad >> 6) & 0x1) ^ 0x1);
                break;
        }

        coef = rom[romaddr];
        //printf("romad:%03X, romaddr:%03X, dout:%08X\n", romad, romaddr, coef);
        data_rdo = ram[ram_r_index++];

        ram_r_index &= (64 -1);

        mult = data_rdo;
        mult *= coef;
        mult >>= 24;
        mult &= (1L<<40) - 1;
        accum += (mult & (1L<<39)) << 1 | (mult);
        /*
        if(data_rdo || accum){
            printf("data_rdo:%08X, mult:%LX, accum:%LX\n", data_rdo, mult, accum);
        }
        */
        if((i + 1) % 64 == 0){
            //printf("out:0x%08X\n", (accum >> 7) & 0x1ffffffff);
            int t = (accum >> 38) & 0x7;
            if(t > 0 && t <= 0x3){
                printf("%09lx\n", 0x7fffffff);
            }else if( t >= 0x4 && t < 0x7){
                printf("%09lx\n", 0x180000001);
            }else{
                printf("%09lx\n", (accum >> 7) & 0x1ffffffff);
            }
            //printf("%08x\n", (accum >> 7) & 0x1ffffffff);
            accum = (mult & (1L<<39)) << 1 | (mult);
        }
    }

    ram[ram_w_index++] = sample;
    ram_w_index &= (64 -1);
}

static int p_data(char *file_name, int txt_up_times)
{
    int read_len;
    int data_len = 0;
    short bytes;
    int sample_rate;
    int up_times = 16;
    int i;
    FILE* f= NULL;
    f= fopen(file_name,"r");
    if(!f)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }
    if(txt_up_times == 0){ //wav file
        fseek(f, 24, SEEK_SET);
        read_len = fread(&sample_rate, 4, 1, f);
        if(read_len < 1){
            printf("read:%s,failed\n",file_name);
            return -1;
        }
        if(sample_rate % 44100 == 0){
            up_times = 16 >> ((sample_rate / 44100) - 1);
        }else{
            up_times = 16 >> ((sample_rate / 48000) - 1);
        }
        //printf("up times:%d\n", up_times);

        fseek(f, 34, SEEK_SET);
        read_len = fread(&bytes, 2, 1, f);
        if(read_len < 1){
            printf("read:%s,failed\n",file_name);
            return -1;
        }
        bytes /= 8;
        //printf("byte persample:%d\n", bytes);

        fseek(f, 40, SEEK_SET);
        read_len = fread(&data_len, 4, 1, f);

        if(read_len < 1){
            printf("read:%s,failed\n",file_name);
            return -1;
        }
        //printf("data len:%d\n", data_len);
        //printf("tell:%d\n", ftell(f));

        for(i=0; i< data_len / bytes / 2; i++){
            //for(i=0; i< 4; i++){
            unsigned int data;
            read_len = fread(&data, bytes, 1, f);
            if(read_len < 1){
                printf("read:%s,failed\n",file_name);
                return -1;
            }
            //printf("0x%08x\n", data);
            //printf("o:%02x%02x00\n", (data >> 8), data & 0xff);
            calc_one_sample(data << 16, up_times);
            fread(&data, bytes, 1, f);
        }
    }else{
        unsigned int sample;
        while(fscanf(f, "%x", &sample) > 0){
            calc_one_sample(sample, txt_up_times);
        }
    }


    fclose(f);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc == 2){
        char *p = &argv[1][strlen(argv[1]) - 3];
        if(*p == 'w' && *(p+1) == 'a' && *(p+2) == 'v'){
            return p_data(argv[1], 0);
        }
    }else if(argc == 3){
        char *p = &argv[1][strlen(argv[1]) - 3];
        if(*p == 't' && *(p+1) == 'x' && *(p+2) == 't'){
            return p_data(argv[1], atoi(argv[2]));
        }
    }

    printf("args: wave file or txt file up times\n");
    return -1;
}
