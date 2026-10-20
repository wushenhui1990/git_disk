#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int read_raw_data(char *file_name, int is_big)
{
	int char_len = 0;
	struct jtag_raw_data *rp;
	unsigned char *buf;
	FILE* fd = NULL;
	int ret;
	fd = fopen(file_name,"r");
	if(!fd)
	{
		printf("open:%s,failed\n",file_name);
		return -1;
	}
	fseek(fd, 0, SEEK_END);
	char_len = ftell(fd);
	//printf("char len:%d\n", char_len);
	fseek(fd, 0, SEEK_SET);
	buf = malloc(char_len);
	if(buf){
		ret = fread(buf, 1, char_len, fd);
	}
	if(ret != char_len){
		printf("read error\n");
		return -1;
	}
	fclose(fd);
	{
		int i;
        int count = 0;
		for(i=0; i< char_len; i+=4){
            if(is_big){
                printf("0x%02x,", buf[i]);
                printf("0x%02x,", buf[i + 1]);
                printf("0x%02x,", buf[i + 2]);
                printf("0x%02x,", buf[i + 3]);
            }else{
                printf("0x%02x,", buf[i + 3]);
                printf("0x%02x,", buf[i + 2]);
                printf("0x%02x,", buf[i + 1]);
                printf("0x%02x,", buf[i]);
            }
			if((++count)%4 == 0)
				printf("\n");
            if(count == 660/4){
                break;
            }
		}
	}
    printf("\n");
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("usage: bin file, indian\n");
		return -1;
	}
    int is_big;
    if(strncmp(argv[2], "little", 1) == 0){
        is_big = 0;
    }else{
        is_big = 1;
    }
	
    read_raw_data(argv[1], is_big);
	
	return -1;
}
