#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE* fd;
	unsigned int size = 0;
	unsigned int data = 0;
	unsigned char *p3;
	unsigned char bits[3];
	char *p = NULL;
	short *p2 = NULL;
	int i;
	unsigned short count = 0;
	fd = fopen(argv[1],"r+");
	if(!fd)
	{
		printf("open failed\n");
		return -1;
	}
	fseek(fd,0x28,SEEK_SET);
	fread(&size,sizeof(size),1,fd);
	printf("file size:%dm,%dk,%db\n",size>>20,(size&~((1<<20)-1))>>10,(size&~((1<<10)-1)));
	printf("size:%04x\n",size);
	p = malloc(size);
	if(!p)
	{
		printf("malloc failed\n");
		return -1;
	}
	p2=(short*)p;
	*p2 = 0;
	for(i=0; i<size/4; i++)
	{
		*p2++ = count++;
	}
	fwrite(p,size,1,fd);
	fclose(fd);
	return 0;
}
