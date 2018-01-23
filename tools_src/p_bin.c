#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int read_raw_data(char *file_name)
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
		for(i=0; i< char_len; i++){
			printf("0x%02x,", buf[i]);
			if((i+1)%8 == 0)
				printf("\n");

		}
	}

}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("usage: ./analy_jtag_scv scvfile\n");
		return -1;
	}
	
    read_raw_data(argv[1]);
	
	return -1;
}
