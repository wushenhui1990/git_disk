#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int p_data(char *file_name, int sample_bytes)
{
    int read_len;
    int data_len = 0;
    short bytes;
    unsigned int datal = 0;
    unsigned int datar = 0;
    int i = 0;
    int w_data_size = 0;
    int w_samples = 0;
    FILE* fout = NULL;
    FILE* fin = NULL;

    fin = fopen(file_name, "r");
    if(!fin)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }
    fseek(fin, 0, SEEK_END);
    data_len = ftell(fin);
    if( data_len % sample_bytes){
        fclose(fin);
        printf("size err, sample bits:%d, but file size:%d\n", sample_bytes, data_len);
    }

    fseek(fin, 0, SEEK_SET);

    switch(sample_bytes){
        case 2:
            printf("static short pcm_data[] = {\n");
            break;
        case 3:
            printf("static int pcm_data[] = {\n");
            break;
        case 4:
            printf("static int pcm_data[] = {\n");
            break;
    }

    while(data_len){
        i++;
        if(fread(&datal, sample_bytes, 1, fin) > 0){
            if(fread(&datar, sample_bytes, 1, fin) > 0){
                switch(sample_bytes){
                    case 2:
                        printf("0x%04X,0x%04X,", datal, datar);
                        break;
                    case 3:
                        printf("0x%06X,0x%06X,", datal, datar);
                        break;
                    case 4:
                        printf("0x%08X,0x%08X,", datal, datar);
                        break;
                }
            }else{
                printf("read err2\n");
                break;
            }
        }else{
            printf("read err1\n");
            break;
        }
        datal = 0;
        datar = 0;
        if(1 || i % 4 == 0){
            printf("\n");
        }
        data_len -= sample_bytes * 2;
    }

    printf("\n};\n");
    printf("static int pcm_data_bytes = %d;\n", sample_bytes);
    fclose(fin);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc == 3){
        return p_data(argv[1], atoi(argv[2]));
    }else{
        printf("args: intel pcm file, sample bytes\n");
    }
}

