#include <stdio.h>

static unsigned int spdif_core_bmc_msb(unsigned int code, int bits, int level)
{
	int i = 0;
	unsigned int bmc_code = 0;
	for (i = 0; i < bits; i++)
	{
		bmc_code <<= 2;
		if (code & 0x1)
		{
			if (level)
			{
				//   '01'
				bmc_code |= 0x1;
				level = 1;
			}
			else
			{
				//   '10'
				bmc_code |= 0x2;
				level = 0;
			}
		}
		else
		{
			if (level)
			{
				//  '00'
				bmc_code |= 0x0;
				level = 0;
			}
			else
			{
				//  '11'
				bmc_code |= 0x3;
				level = 1;
			}
		}
		code >>= 1;
	}
	return bmc_code;
}

void p_map(unsigned char *map)
{
	int i;
	printf("{\n");
	for(i=0; i<16; i++)
	{
		printf("0x%02x,", *map++);
		if( (i+1)%8 == 0)
		{
			printf("\n");
		}
	}
	printf("};\n");
}

int main(void)
{
	int i;
	unsigned char map[2][16] = {0};

	for (i = 0; i < 16; i++)
	{
		map[0][i] = (unsigned char)spdif_core_bmc_msb(i, 4, 0);
		map[1][i] = (unsigned char)spdif_core_bmc_msb(i, 4, 1);
	}
	
	p_map(map[0]);
	p_map(map[1]);

	return 0;
}
