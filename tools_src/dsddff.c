#include <stdio.h>
#include <string.h>
/* generate special dsd signal for 示波器 analyze*/


typedef long long uint64;

int main(int argc, char *argv[])
{
	FILE *fp;
	char id[4];
	unsigned char chunk[] = {0x00,0x00};
	uint64 size;
	char *p;
	char temp[8];
	uint64 i;
	int is_left = 0;
	if(argc != 2)
	{
		printf("usage: file\n");
	}
	fp = fopen(argv[1], "r+");
	if(!fp)
	{
		printf("open err\n");
		return -1;
	}
	fseek(fp, 0x8a, SEEK_SET);
	fread(id, 4, 1, fp);
	if(memcmp(id, "DSD ",4))
	{
		printf("file err\n");
		goto err;
	}
	p = (char*)&size;
	fread(temp, 8, 1, fp);
	for(i=7; i>=0; i--)
	{
		*p++ = temp[i];
	}
	printf("data size:%lld,%lldM,%lldk\n",size, size>>20, size>>10);

	for(i=0; i<size/sizeof(chunk); i++)
	{
		fwrite(chunk, sizeof(chunk), 1, fp);
		if(chunk[0] == 0xff && chunk[1] == 0xff)
		{
			chunk[0] = chunk[1] = 0;
		}	
		else 
		{
			if(is_left)
			{
				if(chunk[0] == 0)
				{
					chunk[0] = 1;
				}
				else
				{
					chunk[0] = (chunk[0]<<1) + 1;
				}
				is_left = 0;
			}
			else
			{
				if(chunk[1] == 0)
				{
					chunk[1] = 1;
				}
				else
				{
					chunk[1] = (chunk[1]<<1) + 1;
				}
				is_left = 1;

			}
		}
	}

	
	fclose(fp);
	return 0;
err:
	fclose(fp);
	return -1;
}

















