#include <stdio.h>
#include <stdlib.h>

int main(void) 
{
    char buf[32];
    double vd;
    float vf;
    fgets(buf,32,stdin);
    printf("input is:%s\n", buf);
    vd = atof(buf);
    vf = (float)vd;

    printf("double hex:0x%lx\n", vd);
    printf("float hex:0x%x\n",  vf);

    return 0;
}
