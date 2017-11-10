#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* analy lcd data in csv file genrate by saleae(USB logic analyze)*/

struct lcd_raw_data
{
	double time_stamp;
    char data[9];
    char cs;
    char cd;
    char wr;
    char rd;
    char reset;
    char backlight;
    char voltage;
}lcd_raw_data;

static struct lcd_raw_data *raw_data_buf;
static int raw_data_count = 0;
static int raw_data_filled = 0;

static void p_raw_data(void)
{
    struct lcd_raw_data *rp = raw_data_buf;
    int i;
    for(i=0; i< raw_data_filled; i++){
    printf("%lf, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 
            rp->time_stamp, 
            rp->data[0], rp->data[1], rp->data[2], rp->data[3],
            rp->data[4], rp->data[5], rp->data[6], rp->data[7],
            rp->data[8], rp->cs, rp->cd, rp->wr, 
            rp->rd, rp->reset, rp->backlight, rp->voltage);
    rp++;
    }

}
static int read_raw_data(char *file_name)
{
	unsigned long char_len = 0;
    unsigned long file_len;
	struct lcd_raw_data *rp;
	FILE* f_scv = NULL;
	f_scv = fopen(file_name,"r");
	if(!f_scv)
	{
		printf("open:%s,failed\n",file_name);
		return -1;
	}
    fseek(f_scv, 0, SEEK_END);
    file_len = ftell(f_scv);
    fseek(f_scv, 0, SEEK_SET);
    printf("file_len:%ld\n", file_len);

    while(1){
        if(raw_data_count - raw_data_filled == 0 ){
            raw_data_count += 1024;
            raw_data_buf = realloc(raw_data_buf, sizeof(struct lcd_raw_data) * raw_data_count);
            if(!raw_data_buf)
            {
                printf("malloc failed, size:%d\n",raw_data_count);
                return -1;
            }
            //printf("%d%%\r", ftell(f_scv)*100/file_len);
        }
        rp = raw_data_buf + raw_data_filled;
        char_len = fscanf(f_scv, "%lf, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 
                &rp->time_stamp, 
                &rp->data[0], &rp->data[1], &rp->data[2], &rp->data[3],
                &rp->data[4], &rp->data[5], &rp->data[6], &rp->data[7],
                &rp->data[8], &rp->cs, &rp->cd, &rp->wr, 
                &rp->rd, &rp->reset, &rp->backlight, &rp->voltage);
        if(char_len <= 0 || char_len == EOF){
            printf("end of file\n");
            break;
        }
        raw_data_filled++;
    }
    printf("raw_data_filled:%ld\n",raw_data_filled);
#if 0
    {
        int i;
        struct lcd_raw_data rd;
        for(i=0; i<raw_data_filled; i++){
            rd = raw_data_buf[i];
            printf("%d, %d, %d, %d, %d\n", rd.time_stamp, rd.tck, rd.tms, rd.tdi, rd.tdo);
        }
    }
#endif
}


static int judge_delay(struct lcd_raw_data jd0, struct lcd_raw_data jd1)
{
    double delter = jd1.time_stamp - jd0.time_stamp;
    if(delter > 0.001){ 
        printf("delay:%lf\n",delter);
    }
}

static int get_data(struct lcd_raw_data *lcd)
{
    int a = 0;
    int i;
    for(i=0; i<9; i++){
        a |= (lcd->data[i] << i);
    }
    //return a;
    return a >> 1;
}

static int get_data2(struct lcd_raw_data *lcd)
{
    int a = 0;
    int i;
    for(i=0; i<9; i++){
        a |= (lcd->data[i] << i);
    }
    return a;
}
#if 1
static int last_lcd_data;
static int last_lcd_cd;
static int lcd_data_count;
static void lcd_data(struct lcd_raw_data *lcd)
{
    int data = get_data(lcd);
    if(last_lcd_data == data && last_lcd_cd == lcd->cd){
        lcd_data_count++;
    }else{
        if(lcd_data_count){
            if(0 == last_lcd_cd){
                printf("{SMART_CONFIG_CMD,0x%02x},\n", last_lcd_data);
            }else{
                printf("{SMART_CONFIG_DATA,0x%02x},\n", last_lcd_data);
            }
            if(lcd_data_count > 1)
                printf("count:%d\n", lcd_data_count);
            lcd_data_count = 0;
        }
        printf("0bit is on%02x\n", get_data2(lcd));
        if(0 == lcd->cd){
            printf("%lf, {SMART_CONFIG_CMD,0x%02x},\n", lcd->time_stamp, data);
        }else{
            printf("%lf, {SMART_CONFIG_DATA,0x%02x},\n", lcd->time_stamp, data);
        }
    }
    last_lcd_data = data;
    last_lcd_cd = lcd->cd;
}
#else

#define TIME_STAMP
static void lcd_data(struct lcd_raw_data *lcd)
{
    int data = get_data(lcd);
    if(0 == lcd->cd){
        printf("%lf, {SMART_CONFIG_CMD,0x%02x},\n", lcd->time_stamp, data);
    }else{
        printf("%lf, {SMART_CONFIG_DATA,0x%02x},\n", lcd->time_stamp, data);
    }
}
#endif

static void lcd_data2(struct lcd_raw_data *lcd)
{
    if(0 == lcd->cd){
        printf("{SMART_CONFIG_CMD,0x%02x},\n", get_data(lcd));
    }else{
        printf("{SMART_CONFIG_DATA,0x%02x},\n", get_data(lcd));
    }
}
static int parse_lcd(void)
{
    int i;
    struct lcd_raw_data *rd0 = raw_data_buf;
    struct lcd_raw_data *rd1 = raw_data_buf + 1;
    for(i=0; i<raw_data_filled; i++){
        //judge_delay(*rd0, *rd1);
        //if(rd0->time_stamp > 9.138)
         //   break;
        if(1 == rd0->reset && 1 == rd1->reset &&
            0 == rd1->cs){
            if(0 == rd0->wr && 1 == rd1->wr){
                lcd_data(rd1);
            }else if(0 == rd0->rd && 1 == rd1->rd){
                //lcd_data2(rd1);
            }
        }
        rd0++;
        rd1++;
    }
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("usage: ./analy_lcd_scv scvfile\n");
		return -1;
	}
	
    read_raw_data(argv[1]);
   // p_raw_data();
    parse_lcd();
    free(raw_data_buf);
	
	return -1;
}
