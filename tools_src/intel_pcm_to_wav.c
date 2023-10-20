#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

static int p_data(char *file_name, int bytes, int rate)
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

    char out_name[128] = {0};

    strcpy(out_name, file_name);

    strcpy(&out_name[strlen(out_name) - 3], "wav");

    fout = fopen(out_name, "w");
    if(!fout)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }

    fin = fopen(file_name, "r");
    if(!fin)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }

    fwrite(wav_header, sizeof(wav_header), 1, fout);
    while(fread(&read_val, bytes, 1, fin) > 0){
        fwrite(&read_val, bytes, 1, fout);
        w_data_size += bytes;
        w_samples++;
    }

    printf("w size:%d, samples:%d, to %s\n", w_data_size, w_samples, out_name);
    if(rate % 44100 == 0 || rate % 48000 == 0){
        set_wav_header(bytes, rate,  w_data_size);
    }
    fseek(fout, 0, SEEK_SET);
    fwrite(wav_header, sizeof(wav_header), 1, fout);
    
    fclose(fin);
    fclose(fout);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 4){
        return p_data(argv[1], atoi(argv[2]), atoi(argv[3]));
    }else{
        printf("args: pcm, bytes, rate\n");
    }
}

