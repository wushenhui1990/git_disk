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

static int p_data(char *file_name, int start_sec, int sec_count)
{
    int read_len;
    int data_len = 0;
    unsigned int src_sample_count = 0;
    unsigned int src_sec_count = 0;
    unsigned int sample_rate;
    unsigned int w_samples = 0;
    unsigned int samples_skiped = 0;
    unsigned int samples_skip = 0;
    unsigned int samples_count = 0;
    int w_data_size = 0;
    short bytes;
    int i;
    FILE* fd= NULL;
    FILE* fout = NULL;
	unsigned char data_flag[4];
    char out_name[128] = {0};
    fd= fopen(file_name,"r");
    if(!fd)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }

    fseek(fd, 24, SEEK_SET);
    read_len = fread(&sample_rate, 4, 1, fd);
    fseek(fd, 34, SEEK_SET);
    if(read_len < 1){
        printf("read:%s,failed\n",file_name);
        return -1;
    }
    read_len = fread(&bytes, 2, 1, fd);
    if(read_len < 1){
        printf("read:%s,failed\n",file_name);
        return -1;
    }
    bytes /= 8;

    //printf("byte persample:%d\n", bytes);

	fseek(fd,0x24,SEEK_SET);
	fread(data_flag, 4,1,fd);
    if(memcmp(data_flag, "data", 4UL) == 0){
	    fread(&data_len,sizeof(data_len),1,fd);
    }else{
        fseek(fd,0x48,SEEK_SET);
        fread(data_flag, 4,1,fd);
        if(memcmp(data_flag, "data", 4UL) == 0){
            fread(&data_len,sizeof(data_len),1,fd);
        }else{
            printf("unknow header\n");
            return -1;
        }
    }

    src_sample_count = data_len / bytes;
    src_sec_count = src_sample_count / 2 / sample_rate;
    printf("src sec:%d, sample count:%d\n", src_sec_count, src_sample_count);

    samples_skip = start_sec * sample_rate * 2;
    if(sec_count < 0 || sec_count > src_sec_count){
        sec_count = src_sec_count;
    }
    samples_count = sec_count * sample_rate * 2;
    if(start_sec + sec_count > src_sec_count){
        sec_count = src_sec_count - start_sec;
        samples_count = src_sample_count - samples_skip;
        printf("sec count too much, cut to:%d\n", sec_count);
    }
    printf("samples_skip:%u, samples_count:%u\n", samples_skip, samples_count);
    //printf("tell:%d\n", ftell(f));
    //printf("static int pcm_data[] = {\n");
    if(samples_skip > src_sample_count){
        printf("skip too much\n");
        fclose(fd);
        return -1;
    }


    strcpy(out_name, file_name);

    sprintf(&out_name[strlen(out_name) - 4], "_%ds_%ds.wav", start_sec, sec_count);

    fout = fopen(out_name, "w+");
    if(!fout)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }
    fwrite(wav_header, sizeof(wav_header), 1, fout);

    for(i=0; i< data_len / bytes; i++){
        unsigned int data_l = 0;
        unsigned int data_r = 0;
        read_len = fread(&data_l, bytes, 1, fd);
        if(read_len < 1){
            break;
        }
        fread(&data_r, bytes, 1, fd);

        if(samples_skiped < samples_skip){
            samples_skiped += 2;
        }else if(w_samples >= samples_count){
            break;
        }else{
            fwrite(&data_l, bytes, 1, fout);
            fwrite(&data_r, bytes, 1, fout);
            w_samples += 2;
            w_data_size += bytes * 2;
        }
    }
    set_wav_header(bytes, sample_rate,  w_data_size);
    fseek(fout, 0, SEEK_SET);
    fwrite(wav_header, sizeof(wav_header), 1, fout);

    printf("w samples:%d, to %s\n", w_samples, out_name);
    fclose(fd);
    fclose(fout);
    return 0;
}

int main(int argc, char *argv[])
{
    int bytes;
    if(argc != 4){
        printf("args: wave file, start sec, sec count\n");
        return -1;
    }

    return p_data(argv[1], atoi(argv[2]), atoi(argv[3]));
}
