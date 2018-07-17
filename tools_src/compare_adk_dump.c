
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int seek_to_begin(FILE *fd)
{
    char a;

    while(1){
        if(fread(&a, 1, 1, fd) == 1){
            if(a == '@'){
                fseek(fd, -1, SEEK_CUR);
                return 0;
            }
        }else{
            return -1;
        }
    }
}

int compare(FILE *fd1, FILE *fd2)
{
    int addr1, addr2;
    int val1, val2;
    int scan1 = 1;
    int scan2 = 1;

    int count = 0;
    while(1){
        if(scan1){
            if(fscanf(fd1, "@%x   %x\n", &addr1, &val1) <= 0){
                break;
            }
        }
        if(scan2){
            if(fscanf(fd2, "@%x   %x\n", &addr2, &val2) <= 0){
                break;
            }
        }

#if 1
        if(addr1 < addr2){
            scan1 = 1;
            scan2 = 0;
            if(val1 != 0xffff){
                printf("@%06x %04x - null\n", addr1, val1);
                count++;
            }
        }else if(addr1 > addr2){
            scan1 = 0;
            scan2 = 1;
            if(val2 != 0xffff){
                printf("@%06x null - %04x\n", addr2, val2);
                count++;
            }
        }else{
            scan1 = 1;
            scan2 = 1;
            if(val1 != val2){
                printf("@%06x %04x - %04x\n", addr1, val1, val2);
                count++;
            }
        }
#else
        printf("@%06x %04x -- @%06x  %04x\n", addr1, val1, addr2, val2);
#endif
    }
    printf("diff count:%d\n", count);
}

int main(int argc, char *argv[])
{
	FILE *fd1, *fd2;
	int ret;

	if(argc < 2){
		printf("usage xuv1 xuv2 \n");
		return 0;
	}
	fd1 = fopen(argv[1], "r+");
	if(!fd1){
		printf("open %s, error\n", argv[1]);
		return -1;
	}
	fd2 = fopen(argv[2], "r+");
	if(!fd2){
		printf("open %s, error\n", argv[2]);
		return -1;
	}

    if(seek_to_begin(fd1)){
        printf(" seek fd1 error\n");
        goto err;
    }

    if(seek_to_begin(fd2)){
        printf(" seek fd2 error\n");
        goto err;
    }

    compare(fd1, fd2);

err:
	fclose(fd1);
	fclose(fd2);

	return 0;
}
