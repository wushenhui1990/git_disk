#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int p_data(char *file_name)
{
    int read_len;
    int data_len = 0;
    short bytes;
    int i;
    char out_file_name[64];
    FILE* f= NULL;
    FILE* f_out = NULL;
    f= fopen(file_name,"r");
    if(!f)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }
    strcpy(out_file_name, file_name);
    strcpy(&out_file_name[strlen(file_name)], ".pcm");
    f_out= fopen(out_file_name,"w");
    if(!f_out)
    {
        printf("open:%s,failed\n",out_file_name);
        return -1;
    }

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

#if 1
    for(i=0; i< data_len / bytes; i++){
        unsigned int data;
        read_len = fread(&data, bytes, 1, f);
        if(read_len < 1){
            printf("read:%s,failed\n",file_name);
            return -1;
        }
        //printf("0x%08x\n", data);
        if(i % 8 == 0){
            printf("\n");
        }
        printf("0x%04x,", data & 0xffff);
        fwrite(&data, 2, 1, f_out);
    }
#else
    for(i=0; i< data_len / bytes / 2; i++){
        unsigned int data;
        read_len = fread(&data, bytes, 1, f);
        if(read_len < 1){
            printf("read:%s,failed\n",file_name);
            return -1;
        }
        //printf("0x%08x\n", data);
        printf("%02x%02x00\n", (data >> 8), data & 0xff);
        fread(&data, bytes, 1, f);
    }

#endif
    fclose(f);
    fclose(f_out);
    return 0;
}

int main(int argc, char *argv[])
{
    int bytes;
    if(argc != 2){
        printf("args: wave file\n");
        return -1;
    }

    return p_data(argv[1]);
}
