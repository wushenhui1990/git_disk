#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int p_data(char *file_name, int bytes)
{
    int i;
    int r_data_size = 0;
    int w_data_size = 0;
    int w_samples = 0;
    unsigned int datal = 0;
    unsigned int datar = 0;
    unsigned int buf[32];
    FILE* fout = NULL;
    FILE* fin = NULL;

    char out_name[128] = {0};

    strcpy(out_name, file_name);

    strcpy(&out_name[strlen(out_name) - 3], "pcm");

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
    fseek(fin, SEEK_SET, SEEK_END);
    r_data_size = ftell(fin);
    fseek(fin, SEEK_SET, SEEK_SET);


    while(fscanf(fin, "%X, %X; %s\n", &datal, &datar, buf) > 0){
        fwrite(&datal, bytes, 1, fout);
        fwrite(&datar, bytes, 1, fout);
        datal = 0;
        datar = 0;
        w_data_size += bytes << 1;
        w_samples++;
    }

    printf("r data size:%d\n", r_data_size);
    printf("w size:%d, samples:%d, to %s\n", w_data_size, w_samples, out_name);
    
    fclose(fin);
    fclose(fout);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc == 3){
        return p_data(argv[1], atoi(argv[2]));
    }else{
        printf("args: txt, sample bytes\n");
    }
}
