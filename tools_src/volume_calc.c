#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float GetVolume(int val, double max_step)
{
	float fVolume;
	int nPos = val;

	if (0 == nPos)
		return -100.0f;

	double log_e_2 = log(2.0f);

	double log_e_n = log(max_step);
	double log_2_n = log_e_n / log_e_2;

	double log_e_x = log((double)nPos);
	double log_2_x = log_e_x / log_e_2;

	fVolume = (float)(10.0 * (log_2_x - log_2_n));

	return fVolume;
}

void p_err(void)
{
	printf("usage:max val,min val,step\n ");
	return;
}


int main(int argc, char *argv[])
{
	int i;
	int max_val, min_val, step;
	if(argc != 4)
	{
		p_err();
		return 0;
	}
	max_val = atoi(argv[1]);
	min_val = atoi(argv[2]);
	step = atoi(argv[3]);
	for(i=0; i<step; i++)
	{
		printf("%f,",GetVolume(i,step));
	}
	printf("\n");
}
