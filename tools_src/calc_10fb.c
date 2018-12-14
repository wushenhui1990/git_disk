#include <stdio.h>

#define USB20
unsigned long long rates[]=
{
	11025,
	16000,
	22050,
	32000,
	44100,
	48000,
	64000,
	88200,
	96000,
	176400,
	192000,
	352800,
	384000,
	352800 * 2,
	384000 * 2,
};

#define RATE_NUM (sizeof(rates)/sizeof(rates[0]))

unsigned int fb_val[RATE_NUM][33];
long long most_fb[]=
{
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,

	2,
	2,
	2,
	2,
	2,
	2,
};

int main(void)
{
	unsigned long long fb;
	int i,j;
	for(j=0; j<RATE_NUM; j++)
	{
		for(i=0; i<=32; i++)
		{
			#ifdef USB20
			fb= ( (rates[j] + (i-16)*most_fb[j])<<16)/8000; //2.0
			#else
			fb= ( (rates[j] + (i-16)*most_fb[j])<<14)/1000;
			#endif
			fb_val[j][i] = fb;
			printf("rate:%lld,i:%d,fb:%08x\n",rates[j],i,fb);

		}
	}
	for(j=0; j<RATE_NUM; j++)
	{
		printf("{ //samplerate:%d",rates[j]);
		for(i=0; i<=32; i++)
		{
			if(i<=16)
			{
				if(i%8 == 0)
					printf("\n");
			}
			else
			{
				if((i-1)%8 == 0)
					printf("\n");
			}
			printf("0x%08x,",fb_val[j][i]);
		}
		printf("\n},\n");
	}

	return 0;
}
