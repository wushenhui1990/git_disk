#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void convert(FILE* input, FILE* output)
{
	char f;
	int scan_ret;
	int read_addr, calc_addr, val;
	unsigned char buf[2];
	calc_addr = 0;
	read_addr = 0;
	while(1){
		scan_ret = fscanf(input, "%c%06x   %04x\n", &f, &read_addr, &val);
		if(scan_ret <= 0){
			return;
		}

		while(calc_addr < read_addr){
			buf[0] = 0xff;
			buf[1] = 0xff;
			if(fwrite(buf, 1, 2, output) != 2){
				printf("write error\n");
				return;
			}
			calc_addr++;
		}
		buf[0] = val & 0xff;
		buf[1] = (val >> 8) & 0xff;
        //printf("addr:%06x, val:%04x, buf0:%02x, buf1:%02x\n", read_addr, val, buf[0], buf[1]);

		if(fwrite(buf, 1, 2, output) != 2){
			printf("write error\n");
			return;
		}
		calc_addr++;
	}

}

int main(int argc, char* argv[])
{
	FILE* input;
	FILE* output;
	char output_file[256];
	char *p;

	input = fopen(argv[1], "rb");

	if(!input){
		printf("file %s open error\n", argv[1]);
		return -1;
	}

	strcpy(output_file, argv[1]);
	p = strstr(output_file, ".xuv");

	if(!p){
		printf("file %s format error\n", argv[1]);
		fclose(input);
		return -1;
	}
	strcpy(p, ".bin");
	output = fopen(output_file, "wb");
	
	if(!output){
		printf("file %s open error\n", output);
		return -1;
	}
	convert(input, output);

	fclose(input);
	fclose(output);

	return 0;


}
