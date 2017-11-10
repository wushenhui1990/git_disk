#include <stdio.h>
#include <stdlib.h>



/*
arg1:all_volume_step;
arg2:step;
*/
void printf_use(void)
{
	printf("args:all_volume_step step first_vol\n");
}
int main(int argc, char *argv[])
{
	int all_step;
	float step;
	float first_val;
	int i;
	float *temp = 0;

	if(argc != 4)
	{
		printf_use();
		return -1;
	}
	all_step = atoi(argv[1]);
	step = atof(argv[2]);
	first_val = atof(argv[3]);
	temp = malloc(all_step *sizeof(float));
	if(temp == 0)
		return -1;

	for(i = 0; i < all_step; i++)
	{
		temp[i] = first_val ;
		first_val += step;
	}


	for(i = 0; i < all_step; i++)
	{
		printf("%4.0f,",temp[i]);
		if(i%10 == 0)
		{
			printf("\n");
		}
	}
	printf("\n----------------------------\n");
	for(i = all_step-1; i >= 0 ; i--)
	{
		printf("%4.0f,",temp[i]);
		if(i%10 == 0)
		{
			printf("\n");
		}
	}


}
