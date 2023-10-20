#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char bmp24_header[] = 
{
0x42, 0x4d, 0x38, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x10, 0xff, 0xff, 0xff, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x02, 0x84, 0x03, 0x00, 0x12, 0x0b, 0x00, 0x00, 0x12, 0x0b, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static int p_data(char *file_name)
{
    int read_len = 0;
    short bytes;
    unsigned int read_val;
    int i;
    int data_offset;
    int data_len;
    int w_data_size = 0;
    int w_samples = 0;
    FILE* fout = NULL;
    FILE* fin = NULL;

    char out_name[128] = {0};

    strcpy(out_name, file_name);
    strcpy(&out_name[strlen(out_name) - 5], "_24.bmp");

    fout = fopen(out_name, "w");
    if(!fout)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }

    fin = fopen(file_name, "r");
    if(!fin)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }

    fseek(fin, 2, SEEK_SET);
    if(fread(&data_len, 4, 1, fin) <= 0){
        printf("read in file err\n");
        return -1;
    }
    printf("data len:%d\n", data_len);
    fseek(fin, 10, SEEK_SET);
    if(fread(&data_offset, 4, 1, fin) <= 0){
        printf("read in file err\n");
        return -1;
    }

    fseek(fin, data_offset, SEEK_SET);
    out_name[strlen(out_name)-2] = 0;
    fprintf(fout, "static const char %s[] = {\n", out_name);
    fseek(fout, sizeof(bmp24_header), SEEK_SET);
    while(fread(&read_val, 2, 1, fin) > 0){
        unsigned short val = (read_val >> 8) | (read_val << 8);
        unsigned short* p_src = &val;
        unsigned int out;
        unsigned char r5, g6, b5;
		unsigned char r8, g8, b8;
        r5 = (*p_src >> 3) & 0x1f;
		g6 = (*p_src & 0x7)<< 3 | ((*p_src >> 13) & 0x7);
		b5 = (*p_src >> 8) & 0x1f;

        r8 = r5 << 3;
		g8 = (g6 << 2);
		b8 = (b5 << 3);

        out = (r8 << 16) | (g8 << 8) | b8;
        fwrite(&out, 3, 1, fout);
        w_data_size += 3;
    }
    unsigned int *p = (int*)&bmp24_header[2];
    *p = w_data_size;
    fseek(fout, 0, SEEK_SET);
    fwrite(bmp24_header, sizeof(bmp24_header), 1, fout);
    printf("w size:%d\n", w_data_size);
    fclose(fin);
    fclose(fout);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc == 2){
        return p_data(argv[1]);
    }else{
        printf("args: bmp16 file\n");
    }
}

