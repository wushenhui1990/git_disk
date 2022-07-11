#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long sigmaadder1;
long sigmaadder2;
long sigmaadder3;
long sigmaadder4;
long sigmaadder5;
long sigmaadder5t;
long sigmalim1;
long sigmalim2;
long sigmalim3;
long sigmalim4;
long localadder;
long locallim;
long quantize;
long quantizet;

unsigned int dsd_data_out;
unsigned int dsd_data_shift;

static void calc_sample_bit(unsigned long sample)
{
    long bit_val;

    localadder = sigmaadder5t - quantizet;
    localadder &= (1UL<<39) - 1;

    if((localadder & (1UL << 38)) == 0 &&
            (
            (localadder & (1UL << 37)) || 
            (localadder & (1UL << 36)) || 
            (localadder & (1UL << 35)) || 
            (localadder & (1UL << 34))
            )
            ){
        locallim = 0x3ffffffff;
    }else if((localadder & (1UL << 38)) &&
            (
            ((localadder & (1UL << 37)) == 0) || 
            ((localadder & (1UL << 36)) == 0) || 
            ((localadder & (1UL << 35)) == 0) || 
            ((localadder & (1UL << 34)) == 0)
            )
            ){
        locallim = 0x7c00000001;
    }else{
        locallim = localadder;
    }


    bit_val = (sigmalim4 >> 38) & 1;
    sigmaadder5 = (bit_val << 38) | 
        ((sigmalim4 >> 1) & ((1UL<<38)-1));
    sigmaadder5 += locallim;
    sigmaadder5 &= (1UL<<39) - 1;
    //printf("**addr5:%010lx, locallim:%010lx, sigmalim1:%010lx\n", sigmaadder5, locallim, sigmalim4);

    if((sigmaadder5 & (1UL << 38)) == 0){
        quantize = 0xffffffff;
    }else{
        quantize = 0x7f00000001;
    }

    if((quantize & (1UL<<38)) == 0){
        dsd_data_out |= (1 << dsd_data_shift);
    }else{
        dsd_data_out &= ~(1 << dsd_data_shift);
    }
    dsd_data_shift++;
    //printf("out:%08x\n", dsd_data_out);
    if(dsd_data_shift >= 32){
        printf("%08x\n", dsd_data_out);
        dsd_data_shift = 0;
        dsd_data_out = 0;
    }

    bit_val = (sample >> 32) & 1;
    sigmaadder1 = (bit_val << 38) | (bit_val << 37) | (bit_val << 36) |(bit_val << 35) |(bit_val << 34) |(bit_val << 33) | sample;
    sigmaadder1 -= quantize;
    sigmaadder1 &= (1UL<<39) - 1;
    sigmaadder1 += sigmalim1;
    sigmaadder1 &= (1UL<<39) - 1;

    //printf("addr1:%010lx, quantize:%010lx, sigmalim1:%010lx\n", sigmaadder1, quantize, sigmalim1);

    bit_val = (sigmalim1 >> 38) & 1;
    sigmaadder2 = (bit_val << 38) | (bit_val << 37) | (bit_val << 36) | (bit_val << 35) | ((sigmalim1 >> 4) & ((1UL<<36) - 1));
    sigmaadder2 -= quantize;
    sigmaadder2 &= (1UL<<39) - 1;
    sigmaadder2 += sigmalim2;
    sigmaadder2 &= (1UL<<39) - 1;
    bit_val = (sigmalim3 >> 38) & 1;
    sigmaadder2 -= (bit_val << 38) | (bit_val << 37) | (bit_val << 36) | (bit_val << 35) | (bit_val << 34) | (bit_val << 33) | (bit_val << 32) | (bit_val << 31) | 
        ((sigmalim3 >> 8) & ((1UL<<32)-1));
    sigmaadder2 &= (1UL<<39) - 1;

    bit_val = (sigmalim2 >> 38) & 1;
    sigmaadder3 = (bit_val << 38) | (bit_val << 37) | (bit_val << 36) | ((sigmalim2 >> 3) & ((1UL<<37) - 1));
    sigmaadder3 -= quantize;
    sigmaadder3 &= (1UL<<39) - 1;
    sigmaadder3 += sigmalim3;
    sigmaadder3 &= (1UL<<39) - 1;

    bit_val = (sigmalim3 >> 38) & 1;
    sigmaadder4 = (bit_val << 38) | (bit_val << 37) | ((sigmalim3 >> 2) & ((1UL<<38) - 1));
    sigmaadder4 -= quantize;
    sigmaadder4 &= (1UL<<39) - 1;
    sigmaadder4 += sigmalim4;
    sigmaadder4 &= (1UL<<39) - 1;
    bit_val = (sigmaadder5t >> 38) & 1;
    sigmaadder4 -= (bit_val << 38) | (bit_val << 37) | (bit_val << 36) | (bit_val << 35) | (bit_val << 34) | (bit_val << 33) | (bit_val << 32) | (bit_val << 31) | 
        ((sigmaadder5t >> 8) & ((1UL<<32)-1));
    sigmaadder4 &= (1UL<<39) - 1;

}

static void sample_change_to_bit(void)
{
    long bit_val;
    if((sigmaadder1 & (1UL << 38)) == 0 &&
            (
             (sigmaadder1 & (1UL << 37)) ||
             (sigmaadder1 & (1UL << 36))
             )
            ){
        sigmalim1 = 0xfffffffff;
    }else if((sigmaadder1 & (1UL << 38)) &&
            (
             ((sigmaadder1 & (1UL << 37)) == 0) ||
             ((sigmaadder1 & (1UL << 36)) == 0)
            )
            ){
        sigmalim1 = 0x7000000001;
    }else{
        sigmalim1 = sigmaadder1;
    }
            
    if((sigmaadder2 & (1UL << 38)) == 0 &&
            (
             (sigmaadder2 & (1UL << 37)) ||
             (sigmaadder2 & (1UL << 36)) ||
             (sigmaadder2 & (1UL << 35))
            )
            ){
        sigmalim2 = 0x7ffffffff;
    }else if((sigmaadder2 & (1UL << 38)) &&
            (
             ((sigmaadder2 & (1UL << 37)) == 0) ||
             ((sigmaadder2 & (1UL << 36)) == 0) ||
             ((sigmaadder2 & (1UL << 35)) == 0)
            )
            ){
        sigmalim2 = 0x7800000001;
    }else{
        sigmalim2 = sigmaadder2;
    }

    if((sigmaadder3 & (1UL << 38)) == 0 &&
            (
             (sigmaadder3 & (1UL << 37)) ||
             (sigmaadder3 & (1UL << 36)) ||
             (sigmaadder3 & (1UL << 35)) ||
             (sigmaadder3 & (1UL << 34))
            )
            ){
        sigmalim3 = 0x3ffffffff;
    }else if((sigmaadder3 & (1UL << 38)) &&
            (
             ((sigmaadder3 & (1UL << 37)) == 0) ||
             ((sigmaadder3 & (1UL << 36)) == 0) ||
             ((sigmaadder3 & (1UL << 35)) == 0) ||
             ((sigmaadder3 & (1UL << 34)) == 0)
            )
            ){
        sigmalim3 = 0x7c00000001;
    }else{
        sigmalim3 = sigmaadder3;
    }

    if((sigmaadder4 & (1UL << 38)) == 0 &&
            (
             (sigmaadder4 & (1UL << 37)) ||
             (sigmaadder4 & (1UL << 36)) ||
             (sigmaadder4 & (1UL << 35)) ||
             (sigmaadder4 & (1UL << 34))
            )
            ){
        sigmalim4 = 0x3ffffffff;
    }else if((sigmaadder4 & (1UL << 38)) &&
            (
             ((sigmaadder4 & (1UL << 37)) == 0) ||
             ((sigmaadder4 & (1UL << 36)) == 0) ||
             ((sigmaadder4 & (1UL << 35)) == 0) ||
             ((sigmaadder4 & (1UL << 34)) == 0)
            )
            ){
        sigmalim4 = 0x7c00000001;
    }else{
        sigmalim4 = sigmaadder4;
    }

    sigmaadder5t = sigmaadder5;
    quantizet = quantize;

}

static void p_debug(void)
{
    printf("addr1:%010lx, addr2:%010lx,addr3:%010lx,addr4:%010lx,addr5:%010lx,localaddr:%010lx\n", sigmaadder1,
            sigmaadder2,sigmaadder3,sigmaadder4,sigmaadder5, localadder);
    printf("lim1:%010lx, lim2:%010lx,lim3:%010lx,lim4:%010lx, localim:%010lx, qe:%010lx, qt:%010lx\n", sigmalim1,
            sigmalim2,sigmalim3,sigmalim4, locallim, quantize, quantizet);
}

static int p_data(char *file_name)
{
    char out_file_name[128];
    int name_len = strlen(file_name);
    unsigned long sample_read;
    unsigned long sample;
    FILE* fin = NULL;
    int i;
    int t_count = 0;
    int zero_cross;

    memcpy(out_file_name, file_name, name_len);
    out_file_name[name_len - 3] = 'd';
    out_file_name[name_len - 2] = 's';
    out_file_name[name_len - 1] = 'f';

    //printf("out file name:%s\n", out_file_name);

    quantizet = 0xffffffff;
    fin= fopen(file_name,"r");
    if(!fin)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }
    //calc_sample_bit(0);
    //p_debug();
    while(fscanf(fin, "%lx\n", &sample_read) > 0){
        sample = sample_read;
        if((sample >> 29) == 0 ||
                (sample >> 29) == 0xf){
            zero_cross = 1;
            if(zero_cross == 0){
                printf("zero cross\n");
            }
        }
        if(zero_cross){
            sample <<= 1;
        }else{
            sample = 0;
        }
        //printf("in 0x%lx\n", sample);
        for(i=0; i<32; i++){
            calc_sample_bit(sample);
            sample_change_to_bit();
            //calc_sample_bit(sample);
            //p_debug();
        }
        t_count++;
#if 0
        if(t_count > 4096){
            break;
        }
#endif
    }

    fclose(fin);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("args: pcm705 hex file\n");
        return -1;
    }

    return p_data(argv[1]);
}
