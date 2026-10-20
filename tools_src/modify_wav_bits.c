#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static unsigned char wav_header_temp[] = 
{
    'R','I', 'F', 'F',
    36, 0, 0, 0, 
    'W', 'A', 'V', 'E',
    'f', 'm', 't', ' ', 
    16, 0, 0, 0,   
    1, 0, 
    2, 0, 
    0x44, 0xac, 0, 0, //sample rate*
    0x02, 0xb1, 0x1, 0, //byte rate *
    0x03, 0,  //blockalign *
    16, 0,  //bits persample*
    'd', 'a', 't', 'a',
    0, 0, 0, 0, //data size *
};

static void set_wav_header(unsigned char wav_header[], int sample_bytes, int sample_rate, int data_size)
{
    int chunksize = 36;
    int byte_rate = sample_rate * 2 *sample_bytes;
    short block_align = 2 * sample_bytes;
    short bits_per_sample = sample_bytes * 8;
    chunksize += data_size;
    wav_header[4] = (chunksize & 0xff);
    wav_header[5] = (chunksize >> 8) & 0xff;
    wav_header[6] = (chunksize >> 16) & 0xff;
    wav_header[7] = chunksize >> 24;

    wav_header[24] = (sample_rate & 0xff);
    wav_header[25] = (sample_rate >> 8) & 0xff;
    wav_header[26] = (sample_rate >> 16) & 0xff;
    wav_header[27] = sample_rate >> 24;

    wav_header[28] = (byte_rate & 0xff);
    wav_header[29] = (byte_rate >> 8) & 0xff;
    wav_header[30] = (byte_rate >> 16) & 0xff;
    wav_header[31] = byte_rate >> 24;

    wav_header[32] = block_align & 0xff;
    wav_header[33] = block_align >> 8;
     
    wav_header[34] = bits_per_sample & 0xff;
    wav_header[35] = bits_per_sample >> 8;

    wav_header[40] = (data_size & 0xff);
    wav_header[41] = (data_size >> 8) & 0xff;
    wav_header[42] = (data_size >> 16) & 0xff;
    wav_header[43] = data_size >> 24;
}

static void get_sample_rate_bits(unsigned char wav_header[], int* sample_bits, int* sample_rate)
{
    unsigned int bits = 0;
    unsigned int rate = 0;
    bits |= wav_header[35];
    bits <<= 8;
    bits |= wav_header[34];

    rate |= wav_header[27];
    rate <<= 8;
    rate |= wav_header[26];
    rate <<= 8;
    rate |= wav_header[25];
    rate <<= 8;
    rate |= wav_header[24];

    *sample_bits = bits;
    *sample_rate = rate;
}

static int p_data(char *file_name, int target_sample_bits)
{
    int read_len;
    int data_len = 0;
    unsigned int read_val;
    int i;
    int sample_read;
    int w_data_size = 0;
    int w_samples = 0;
    FILE* fout = NULL;
    FILE* fin = NULL;
    char read_wav_header[128];
    int r_sample_bits;
    int r_sample_rate;
    int r_sample_byte;
    int shift_bits;
    int w_sample_byte;

    char out_name[128] = {0};
    switch(target_sample_bits){
        case 16:
        case 24:
        case 32:
            printf("target bits:%d\n", target_sample_bits);
            break;
        default:
            printf("target bits err:%d\n", target_sample_bits);
            return -1;

    }

    fin = fopen(file_name, "r");
    if(!fin)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }
    if(fread(read_wav_header, sizeof(wav_header_temp), 1, fin) != 1){
        printf("read:%s,failed\n",file_name);
        return -1;
    }
    get_sample_rate_bits(read_wav_header, &r_sample_bits, &r_sample_rate);
    printf("read wav sample bits:%d, rate:%d\n", r_sample_bits, r_sample_rate);
    if(r_sample_bits == target_sample_bits){
        printf("origin bits:%d, target bits:%d\n",r_sample_bits, target_sample_bits);
        fclose(fin);
        return -1;
    }
    r_sample_byte = r_sample_bits / 8;
    shift_bits = target_sample_bits - r_sample_bits;
    w_sample_byte = target_sample_bits / 8;

    strncpy(out_name, file_name, strlen(file_name) - 4);

    sprintf(&out_name[strlen(file_name) - 4], "_to_%dbits.wav", target_sample_bits);

    fout = fopen(out_name, "w");
    if(!fout)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }

    fseek(fout, sizeof(wav_header_temp), SEEK_SET);
    while(fread(&read_val, r_sample_byte, 1, fin) > 0){
        int write_val = 0;
        if(shift_bits < 0){
            write_val = read_val >> -shift_bits;
        }else{
            write_val = read_val << shift_bits;
        }
        fwrite(&write_val, w_sample_byte, 1, fout);
        w_data_size += w_sample_byte;
        w_samples++;
    }

    printf("w size:%d, samples:%d, to %s\n", w_data_size, w_samples, out_name);
    if(r_sample_rate % 44100 == 0 || r_sample_rate % 48000 == 0){
        set_wav_header(wav_header_temp, w_sample_byte, r_sample_rate,  w_data_size);
    }
    fseek(fout, 0, SEEK_SET);
    fwrite(wav_header_temp, sizeof(wav_header_temp), 1, fout);
    
    fclose(fin);
    fclose(fout);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 3){
        return p_data(argv[1], atoi(argv[2]));
    }else{
        printf("args: wav, bits\n");
    }
}

