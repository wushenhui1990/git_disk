#include <stdio.h>
#include <math.h>

unsigned char convert_db(char gain)
{
  float scale;
  float val;
  float persent;
  short return_val;

  scale = 12 + 34;

  val = gain + 34;

  persent = (val/scale);

  printf("persent:%f\n", persent);
  
  return_val = (255 * persent);
  printf("val:%d\n", return_val);
  return_val = 255 - return_val;
  
  return return_val;

}


#define CONV_TO_USBVOL(a) ( -( (a)<<7))

int main(void)
{
    double scale_r = pow(10.0f, (0) / 200.0f);
    char val;
    int i;
    for(val=(char)0xc; val >= (char)0xde; val--){
        printf("%d\n", val);
        printf("%d\n", convert_db(val));
        printf("-------\n");
    }

    for(i=0; i<=255; i++){
        printf("%3d, %04x\n", i, (unsigned short)CONV_TO_USBVOL(i));
    }

    printf("%f\n", scale_r);
    return 0;
}
