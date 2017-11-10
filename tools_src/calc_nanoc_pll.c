#include <stdio.h>
#include <stdlib.h>


#define XIN 24000000

void printf_usage(void)
{
	printf("usage:lrclk,sclk/lrclk,clock_err\n");
}


int main(int argc, char *argv[])
{
	unsigned int m,n,od;
	float pll_clk_out;
	unsigned int g_err;
	int mclk_div;
	int i2s_lrclk;
	float i2s_mclk;
	int i2s_sclk;
	int multiple;

	if(argc != 4)
	{
		printf_usage();
		return -1;
	}
	i2s_lrclk = atoi(argv[1]);
	multiple = atoi(argv[2]);
	g_err = atoi(argv[3]);
	i2s_sclk = i2s_lrclk * multiple;

	for(od=2; od<16; od++)
	{
		for(n=2; n<16; n++)
		{
			for(m=2; m<255; m++)
			{
				for(mclk_div=1; mclk_div<255; mclk_div++)
				{
					int sclk_div = 0;
					pll_clk_out = XIN*m/(n*od*2);
					i2s_mclk = pll_clk_out/(mclk_div+1);
					while(sclk_div<=255)
					{
						float calc_sclk = i2s_mclk/sclk_div;
						int err;
						if(calc_sclk < i2s_sclk/2)
							break;
						err = (calc_sclk > i2s_sclk)?(calc_sclk - i2s_sclk):(i2s_sclk-calc_sclk);
						if(err <= g_err)
						{
							printf("n:%3d,m:%3d,od:%3d,pll_clk_out:%.0f.\n"\
									,n,m,od,pll_clk_out);
							printf("mclk:%.0f,mclk_div:%3d,sclk_div:%3d,err:%3d\n\n"\
									,i2s_mclk-1,mclk_div-1,sclk_div,err);

						}
						sclk_div += 2;
					}
				}
			}
		}
	}
	return 0;
}
