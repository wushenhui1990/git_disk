#include <stdio.h>

unsigned short report[] = 
{
0x0105,//usage page
0x0209,//usage
0x01A1,//collection
0x0185,//report id

0x0109,//usage
0x00A1,//collection
0x0905,//usage page
0x0119,//usage minimum

0x0529,//usage maximum
0x0015,//logical minimum
0x0125,//logical maximum
0x0595,//report count

0x0175,//report size
0x0281,//input
0x0195,//
0x0375,

0x0381,
0x0105,
0x0116,
0x26f8,

0x07ff,
0x0c75,
0x0295,
0x3009,


0x3109,
0x0681,
0x8115,
0x7f25,

0x0875,
0x0195,
0x3809,
0x0681,

0x05c0,
0x0a0c,
0x0238,
0x0195,

0x0681,
0xC0C0,
};

char global_item[][32] = 
{
"Usage Page",
"Logical Minimum",
"Logical Maximum",
"Physical Minimum",
"Physical Maximum",
"Unit Exponent",
"Unit",
"Report Size",
"Report ID",
"Report Count",
"Push",
"Pop",
};

char main_item[][32]=
{
"Input",
"Output",
"Collection",
"Feature",
"End Collection",
};

char local_item[][32]=
{
"Usage",
"Usage Minimum",
"Usage Maximum",
"Designator Index",
"Designator Minimum",
"Designator Maximum",
"String Index",
"String Minimum",
"String Maximum",
"Delimiter", 
};

int main(int argc, char *argv[])
{
	int i; 
	char type;
	for(i=0; i<sizeof(report)/sizeof(report[0]); i++)
	{
		type = report[i] &0xff;
		printf("0x%04x,//",report[i]);
		switch((type>>2)&0x3 )
		{
			case 0:
			printf("%s\n",main_item[(type>>4) &0x7]);
			break;
			case 1:
			printf("%s\n",global_item[(type>>4) &0xf]);
			break;
			case 2:
			printf("%s\n",local_item[(type>>4) &0xf]);
			break;
			default:
			printf("unknown\n");
			break;
		}
	}
	return 0;
}
