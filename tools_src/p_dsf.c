#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int p_data(char *file_name)
{
    int read_len;
    int data_len = 0;
    short bytes;
    int i;
    FILE* f= NULL;
    f= fopen(file_name,"r");
    if(!f)
    {
        printf("open:%s,failed\n",file_name);
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

#if 0
    for(i=0; i< data_len / bytes; i++){
        unsigned int data;
        read_len = fread(&data, bytes, 1, f);
        if(read_len < 1){
            printf("read:%s,failed\n",file_name);
            return -1;
        }
        //printf("0x%08x\n", data);
        printf("%02x%02x00\n", (data >> 8), data & 0xff);
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
    return 0;
}

int main(int argc, char *argv[])
{
    int bytes;
    if(argc != 2){
        printf("args: dsf file\n");
        return -1;
    }

    return p_data(argv[1]);
}
