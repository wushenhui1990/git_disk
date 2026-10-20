#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int last_val;

int main(int argc, char *argv[])
{
	FILE* fd;
	unsigned int size = 0;
	unsigned int data = 0;
	unsigned char data_flag[4];
	unsigned char *p3;
	unsigned char bits[3];
	char *p = NULL;
	unsigned char *p2 = NULL;
	int i;
	unsigned int count1 = 1;
	unsigned int count2 = 0x8000;
	fd = fopen(argv[1],"r+");
	if(!fd)
	{
		printf("open failed\n");
		return -1;
	}
	fseek(fd,0x24,SEEK_SET);
	fread(data_flag, 4,1,fd);
    if(memcmp(data_flag, "data", 4UL) == 0){
	    fread(&size,sizeof(size),1,fd);
    }else{
        fseek(fd,0x48,SEEK_SET);
        fread(data_flag, 4,1,fd);
        if(memcmp(data_flag, "data", 4UL) == 0){
            fread(&size,sizeof(size),1,fd);
        }else{
            printf("unknow header\n");
            return -1;
        }
    }
	printf("file size:%dm,%dk,%db\n",size>>20,(size&~((1<<20)-1))>>10,(size&~((1<<10)-1)));
	printf("size:%04x\n",size);
	p = malloc(size);
	if(!p)
	{
		printf("malloc failed\n");
		return -1;
	}
	p2=(unsigned char*)p;

    int bulk_count = 0;
    int test_data = 1;
	for(i=0; i<size;)
	{
        
        int l_data = test_data;
        int r_data = test_data;
#if 1
        if(bulk_count++ > 1024 * 400){
            bulk_count = 0;
            test_data = 1;
            if(test_data == (1 << 24)){
                test_data = 1;
            }
        }
#else
        if(bulk_count++ > 2){
            bulk_count = 0;
            if(test_data == 1){
                test_data = (1 << 23);
            }else{
                test_data = 1;
            }
        }

#endif

        *p2++ = l_data & 0xff;
        *p2++ = (l_data >> 8) & 0xff;
        *p2++ = (l_data >> 16) & 0xff;
        *p2++ = r_data & 0xff;
        *p2++ = (r_data >> 8) & 0xff;
        *p2++ = (r_data >> 16) & 0xff;

        i += 3 * 2;
	}
	fwrite(p,size,1,fd);
	fclose(fd);
	return 0;
}
