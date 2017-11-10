#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	unsigned char bit_mark;
	unsigned char val;
	char *name;
}hid_item;

hid_item hid_item_type[]=
{
	{0xc,0,"main"},
	{0xc,1<<2,"global"},
	{0xc,2<<2,"local"},
	{0xc,3<<2,"reserve"},
};

hid_item hid_global_item[]=
{
	{0xf0,0x00,"usage page"},
	{0xf0,0x10,"logical minimum"},
	{0xf0,0x20,"logical maximum"},
	{0xf0,0x30,"physical minimum"},
	{0xf0,0x40,"physical maximum"},
	{0xf0,0x50,"unit exponent"},
	{0xf0,0x60,"unit"},
	{0xf0,0x70,"report size"},
	{0xf0,0x80,"report id"},
	{0xf0,0x90,"report count"},
	{0xf0,0xa0,"push"},
	{0xf0,0xb0,"pop"},
};

hid_item hid_local_item[]=
{
	{0xf0,0x00,"usage"},
	{0xf0,0x10,"usage minimum"},
	{0xf0,0x20,"usage maximum"},
	{0xf0,0x30,"designator index"},
	{0xf0,0x40,"designator minimum"},
	{0xf0,0x50,"designator maximum"},
	{0xf0,0x60,"not use"},
	{0xf0,0x70,"string index"},
	{0xf0,0x80,"string minimum"},
	{0xf0,0x90,"string maximum"},
	{0xf0,0xa0,"delimiter"},

};        

hid_item hid_main_item[]=
{
	{0xf0,0x80,"input"},
	{0xf0,0x90,"output"},
	{0xf0,0xb0,"feature"},
	{0xf0,0xa0,"collection"},
	{0xf0,0xc0,"end collection"},
};

char collection_type[][16]=
{
	{"physical"},
	{"Application"},
	{"Logical"},
	{"Report"},
	{"Named Array"},
	{"Usage switch"},
	{"Usage Modifier"},
};

#define MEMBER_SIZE(a) (sizeof(a)/sizeof(a[0]))

void p_tag_(hid_item* item, int tag_val, int item_num)
{
	int i;
	for(i=0; i<item_num; i++)
	{
		if(tag_val == item[i].val)
		{
			printf("%s:",item[i].name);
		}
	}
}

int p_tag(unsigned char type, unsigned char tag)
{
	switch(type)
	{
		case 0<<2:
			p_tag_(hid_main_item, tag, MEMBER_SIZE(hid_main_item));
			break;
		case 1<<2:
			p_tag_(hid_global_item, tag, MEMBER_SIZE(hid_global_item));
			break;
		case 2<<2:
			p_tag_(hid_local_item, tag, MEMBER_SIZE(hid_local_item));
			break;
	}
	return 0;
}

int p_type_tag(unsigned char prefix)
{
	int i;
	int val;
	for(i=0; i<sizeof(hid_item_type)/sizeof(hid_item_type[0]); i++)
	{
		val = prefix & (hid_item_type[i].bit_mark);
		if( val == hid_item_type[i].val)
		{
			printf("%s,",hid_item_type[i].name);
			p_tag(val, prefix&0xf0);
			break;
		}
	}
	return 0;
}

void p_val(unsigned char *p, int size)
{
	int i;
	for(i=0; i<size; i++)
	{
		printf("%02x,",*p++);
	}
	printf("\n");
}

int hid_parse(unsigned char *buf, int size)
{
	unsigned char *p = buf;
	int val_size;
	while(size>0)
	{
		val_size = (*p)&0x3;
		p_type_tag(*p);
		p++;
		size--;
		p_val(p,val_size);
		p+=val_size;
		size -= val_size;
	}
}


int main(int argc, char* argv[])
{
	FILE* fp;
	int size;
	char *buf;
	if(argc != 2)
	{
		printf("args : hid bin file path\n");
		return -1;
	}
	fp = fopen(argv[1], "r");
	if(!fp)
	{
		printf("open file err\n");
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = (char*)malloc(size);
	fread(buf,size, 1, fp);
	fclose(fp);
	
	hid_parse(buf, size);
	return 0;
}
