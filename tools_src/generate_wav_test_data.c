#include <stdio.h>
#include <stdlib.h>

static char wav_header[] = 
{
    'R','I', 'F', 'F',
    36, 0, 0, 0, //big indian *
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

static void set_wav_header(int sample_bytes, int sample_rate, int data_size)
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

static int p_data(int bytes, int rate, int time_s)
{
    int i;
    int w_samples = 0;
    int w_size = 0;
    FILE* fout = NULL;
    int sample_val = 0;

    char out_name[128] = {0};
    sprintf(out_name, "test_%dbytes_%d_%ds.wav", bytes, rate, time_s);

    if(rate % 44100 != 0 && rate % 48000 != 0){
        printf("rate err:%d\n", rate);
        return -1;
    }

    fout = fopen(out_name, "w");
    if(!fout)
    {
        printf("open:%s,failed\n",out_name);
        return -1;
    }

    w_samples = rate * time_s;
    w_size = w_samples * bytes * 2;

    fseek(fout, sizeof(wav_header), SEEK_SET);
    for(i=0; i<w_samples; i++){
        switch(bytes){
            case 2:
                {
                    short data[2];
                    data[0] = sample_val;
                    data[1] = sample_val;
                    fwrite(data, 4, 1, fout);
                    if(sample_val++ > 0xffff){
                        sample_val = 0;
                    }
                }
                break;
            case 3:
                {
                    int data[2];
                    data[0] = sample_val;
                    data[1] = sample_val;
                    fwrite(&data[0], 3, 1, fout);
                    fwrite(&data[1], 3, 1, fout);
                    sample_val += 0x100;
                    sample_val |= 0xff;
                    if(sample_val > 0xffffff){
                        sample_val = 0;
                    }
                }
                break;
            case 4:
                {
                    int data[2];
                    data[0] = sample_val;
                    data[1] = sample_val;
                    fwrite(&data[0], 4, 1, fout);
                    fwrite(&data[1], 4, 1, fout);
                    sample_val++;
                }
                break;

        }
    }

    set_wav_header(bytes, rate, w_size);
    fseek(fout, 0, SEEK_SET);
    fwrite(wav_header, sizeof(wav_header), 1, fout);

    fclose(fout);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 4){
        return p_data(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    }else{
        printf("args: sample width, sample rate, time s\n");
    }
}


