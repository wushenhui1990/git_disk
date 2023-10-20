#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int p_data(char *file_name0, char *file_name1, int bytes)
{
    int i;
    int r_data_size = 0;
    int c_size = 0;
    int c_samples = 0;

    int equal_count = 0;
    int diff_1_count = 0;
    int diff_2_count = 0;
    int diff_3_count = 0;
    int diff_l, diff_r;
    unsigned int data0l = 0;
    unsigned int data0r = 0;
    unsigned int data1l = 0;
    unsigned int data1r = 0;
    FILE* f1 = NULL;
    FILE* f0 = NULL;

    f1 = fopen(file_name1, "r");
    if(!f1)
    {
        printf("open:%s,failed\n",file_name1);
        return -1;
    }

    f0 = fopen(file_name0, "r");
    if(!f0)
    {
        printf("open:%s,failed\n",file_name0);
        return -1;
    }
    fseek(f0, SEEK_SET, SEEK_END);
    r_data_size = ftell(f0);
    fseek(f0, SEEK_SET, SEEK_SET);

    while(c_size < r_data_size){
        fread(&data0l, bytes, 1, f0);
        fread(&data0r, bytes, 1, f0);
        fread(&data1l, bytes, 1, f1);
        fread(&data1r, bytes, 1, f1);

        c_size += bytes << 1;
        diff_l = abs(data0l - data1l);
        diff_r = abs(data0r - data1r);
        if(diff_l > 2 || diff_r > 2){
            diff_3_count++;
            printf("0x%x,0x%x --- 0x%x,0x%x, p:%d\n", data0l, data0r, data1l, data1r, c_samples);
        }else if(diff_l > 1 || diff_r > 1){
            diff_2_count++;
        }else if(diff_l > 0 || diff_r > 0){
            diff_1_count++;
        }else{
            equal_count++;
        }
        c_samples++;
        data0l = 0;
        data0r = 0;
        data1l = 0;
        data1r = 0;
    }
    printf("eq:%d, %%%d\n", equal_count, equal_count * 100 / c_samples);
    printf("dff1:%d, %%%d\n", diff_1_count, diff_1_count * 100 / c_samples);
    printf("dff2:%d, %%%d\n", diff_2_count, diff_2_count * 100 / c_samples);
    printf("dff3:%d, %%%d\n", diff_3_count, diff_3_count * 100 / c_samples);

    
    fclose(f0);
    fclose(f1);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc == 4){
        return p_data(argv[1], argv[2], atoi(argv[3]));
    }else{
        printf("args: txt, sample bytes\n");
    }
}
