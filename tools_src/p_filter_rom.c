#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apd_rom.h"
#include "gto_rom.h"
#include "ta_rom.h"
#include "bp_rom.h"

#define ROMAD_1024_START_INDEX 0x3FF
#define ROMAD_512_START_INDEX 0xBFF
#define ROMAD_MAX_INDEX 0x7FF

#define TO_16X 1

enum{
    FILTER_BP,
    FILTER_GTO,
    FILTER_APD,
    FILTER_TA,
};

static unsigned int ram[128];
static long accum;
static int sample_count;
int zero_cross = 0;
int last_romaddr;
static void calc_one_sample(int up_times, int filter_id)
{
    int romad;
    int romaddr;
    int data_rdo;
    int coef;
    int i;
    long mult;
    long firculc;
    int max_calc = 1024;
    long out_data = 0;
    unsigned int *rom = NULL;

    switch(filter_id){
        case FILTER_BP:
            rom = bp_rom;
            break;
        case FILTER_GTO:
            rom = gto_rom;
            break;
        case FILTER_APD:
            rom = apd_rom;
            break;
        case FILTER_TA:
            rom = ta_rom;
            break;
    }

    switch(up_times){
        case 16:
            romad = ROMAD_1024_START_INDEX;
            break;
        case 8:
            romad = (ROMAD_1024_START_INDEX >> 1) | (1 << 10);
            max_calc >>= 1;
            break;
        case 4:
            romad = (ROMAD_1024_START_INDEX >> 2) | (3 << 9);
            max_calc >>= 2;
            break;
        case 2:
            romad = (ROMAD_1024_START_INDEX >> 3) | (7 << 8);
            max_calc >>= 3;
            break;
    }
    sample_count++;
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
        printf("addr:%4d 0x%04X,addr diff:%5d val:%10d 0x%08X\n", romaddr, romaddr, last_romaddr - romaddr, coef, coef);
        last_romaddr = romaddr;
        //printf("%d\n", coef);
        if((i + 1) % 64 == 0){
            //printf("out:0x%08X\n", (accum >> 7) & 0x1ffffffff);
            printf("--------------\n");
        }
    }
}

static int p_data(int up_times, char* filter_str)
{
    int filter_id = 0;

    if(strcmp(filter_str, "bp") == 0){
        filter_id = FILTER_BP;
    }else if(strcmp(filter_str, "gto") == 0){
        filter_id = FILTER_GTO;
    }else if(strcmp(filter_str, "apd") == 0){
        filter_id = FILTER_APD;
    }else if(strcmp(filter_str, "ta") == 0){
        filter_id = FILTER_TA;
    }
    calc_one_sample(up_times, filter_id);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc == 3){
        return p_data(atoi(argv[1]), argv[2]);
    }

    printf("args: up times, filter type\n");
    return -1;
}
