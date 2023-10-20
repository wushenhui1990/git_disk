#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int p_data(char *file_name)
{
    int read_len = 0;
    short bytes;
    unsigned int read_val;
    int i;
    int x_pix, y_pix;
    int data_offset;
    int data_len;
    int w_data_size = 0;
    int w_samples = 0;
    FILE* fout = NULL;
    FILE* fin = NULL;

    char out_name[128] = {0};

    strcpy(out_name, file_name);
    strcpy(&out_name[strlen(out_name) - 3], "h");

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
    fseek(fin, 12, SEEK_SET);
    if(fread(&x_pix, 4, 1, fin) <= 0){
        printf("read in file err\n");
        return -1;
    }
    if(fread(&y_pix, 4, 1, fin) <= 0){
        printf("read in file err\n");
        return -1;
    }
    y_pix = -y_pix;

    printf("data len:%d, x:%d, y:%d\n", data_len, x_pix, y_pix);
    fseek(fin, 10, SEEK_SET);
    if(fread(&data_offset, 4, 1, fin) <= 0){
        printf("read in file err\n");
        return -1;
    }

    fseek(fin, data_offset, SEEK_SET);
    out_name[strlen(out_name)-2] = 0;
    fprintf(fout, "static const char %s[] = {\n", out_name);
    while(fread(&read_val, 2, 1, fin) > 0){
#if 1
        char r,g,b;
        unsigned short pix;
        b = (read_val) & 0x1f;
        g = (read_val >> 5) & 0x3f;
        r = (read_val >> 11) & 0x1f;
        pix = (b << 3) | (g >> 3);
        pix <<= 8;
        pix |= ((g  & 0x7) << 5) | r;
        //fprintf(fout, "0x%02x, 0x%02x, ", (pix >> 8) & 0xff, pix & 0xff);
        fprintf(fout, "0x%02x, 0x%02x, ", pix & 0xff, (pix >> 8) & 0xff);
#else
        fprintf(fout, "0x%02x, 0x%02x, ", read_val & 0xff, (read_val >> 8) & 0xff);
        //fprintf(fout, "0x%02x, 0x%02x, ", (read_val >> 8) & 0xff, read_val & 0xff);
#endif
        w_data_size += 2;
        if(w_data_size % 16 == 0){
            fprintf(fout, "\n");
        }
    }
    fprintf(fout, "\n};\n");
#if 0
    fprintf(fout, "lv_img_dsc_t %s_icon = {\n", out_name);
    fprintf(fout, ".header.always_zero = 0,\n");
    fprintf(fout, ".header.w = %d,\n", x_pix);
    fprintf(fout, ".header.h = %d,\n", y_pix);
    fprintf(fout, ".data_size = %d,\n", w_data_size);
    fprintf(fout, ".header.cf = LV_COLOR_FORMAT_NATIVE,\n");
    fprintf(fout, ".data = %s,\n};\n", out_name);
#endif

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

