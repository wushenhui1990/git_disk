#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int test_fun(int i)
{
    int return_val;
    long val;

    val = (long)i << 1;

    //printf("val:0x%08x, %ld,big:%d, low:%d i:0x%08x\n", 
     //       val, val, (int)0x7fffffff, (int)0x80000000U, i);
    if(val > (int)0x7fffffff){
     //   printf("long val:%ld, bigger\n", val);
        val = (int)0x7fffffff;
    }else if( val < (int)0x80000000U){
        int test_val = val;
        //printf("long val:%ld/0x%lx, smaller, test_val:%d/0x%08x, min:%d\n", 
         //       val, val, test_val, test_val, (int)0x80000000U);
        val = (int)0x80000000U;
    }
    return_val = val;
    return return_val;
}

int test_fun2(int i)
{
    int return_val;
    int val;

    val = (long)i << 1;

    return_val = val;
    return return_val;
}

int main(void)
{
    unsigned int i;
    unsigned int start_p_val;
    unsigned int diff_flag;
    int correct_val;

    printf("size of int:%d, long:%d\n", sizeof(int), sizeof(long));
    for(i=0; i<0xffffffffU; i++){
        int t1 = test_fun((int)i);
        int t2 = test_fun2((int)i);
        if(t1 != t2){
            if(correct_val != t1 || ++start_p_val != i){
                printf("in val:%ud/0x%08x, t1:%d/0x%08x, t2:%d/0x%08x\n", i, i, t1, t1, t2, t2);
                start_p_val = i;
                correct_val = t1;
            }
            diff_flag = 1;
        }else{
            if(diff_flag){
                printf("normal in val:%ud/0x%08x, t1:%d/0x%08x, t2:%d/0x%08x\n", i, i, t1, t1, t2, t2);
                diff_flag = 0;
            }
        }
    }

}
