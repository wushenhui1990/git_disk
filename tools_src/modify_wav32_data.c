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
	unsigned int *p2 = NULL;
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
	p2=(unsigned int*)p;

	for(i=0; i<size/8; i++)
	{
		#if 0
		*p2++ = count1;
		*p2++ = count2;
		if(count1 == 0xffff){
			count1 = 1;
		}else{
            count1++;
		}
		if(count2 == 0){
			count2 = 0x8000;
		}else{
            count2--;
		}
		#elif 0
        if(last_val == 0xaaaa){
            *p2++ = 0x5555;
            *p2++ = 0x5555;
            last_val = 0x5555;
        }else{
            *p2++ = 0xaaaa;
            *p2++ = 0xaaaa;
            last_val = 0xaaaa;
        }
        #else
            *p2++ = 0x8000;
            *p2++ = 0x8000;
		//*p2++ = count1;
		//*p2++ = count1;
		//count1++;

		#endif
	}
	fwrite(p,size,1,fd);
	fclose(fd);
	return 0;
}
