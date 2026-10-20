#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int p_data(char *file_name)
{
    int read_len;
    int data_len = 0;
    int sample_rate;
    short bytes;
    int i;
    FILE* fd= NULL;
	unsigned char data_flag[4];
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

    //printf("data len:%d\n", data_len);
    //printf("tell:%d\n", ftell(f));
    printf("static int pcm_data[] = {\n");

    for(i=0; i< data_len / bytes; i++){
        unsigned int data_l = 0;
        unsigned int data_r = 0;
        read_len = fread(&data_l, bytes, 1, fd);
        if(read_len < 1){
            break;
        }
        fread(&data_r, bytes, 1, fd);
        if(i % 4 == 0){
            printf("\n");
        }
        //if(bytes == 2){
        //    printf("0x%08x,0x%08x,", data_l << 16, data_r << 16);
        //}else{
        //    printf("0x%08x,0x%08x,", data_l << 8, data_r << 8);
        //}
        if(bytes == 2){
            printf("0x%04x,0x%04x,", data_l, data_r);
        }else if(bytes == 3){
            printf("0x%06x,0x%06x,", data_l, data_r);
        }else{
            printf("0x%08x,0x%08x,", data_l, data_r);
        }
    }

    printf("\n};\n");
    printf("static int pcm_data_bytes = %d;\n", bytes);
    printf("static int pcm_sample_rate = %d;\n", sample_rate);
    fclose(fd);
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
