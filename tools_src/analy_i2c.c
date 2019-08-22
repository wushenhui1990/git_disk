#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USE_ENABLE
struct i2c_raw_data
{
	double time_stamp;
	char i2c_data;
	char i2c_clk;
#ifdef USE_ENABLE
	char i2c_enable;
#endif
}i2c_raw_data;

static struct i2c_raw_data *raw_data_buf;
static int raw_data_count = 0;
static int raw_data_filled = 0;

static int read_raw_data(char *file_name)
{
	int char_len = 0;
	struct i2c_raw_data *rp;
	FILE* f_scv = NULL;
	f_scv = fopen(file_name,"r");
	if(!f_scv)
	{
		printf("open:%s,failed\n",file_name);
		return -1;
	}

    while(1){
        if(raw_data_count - raw_data_filled == 0 ){
            raw_data_count += 1024;
            raw_data_buf = realloc(raw_data_buf, sizeof(struct i2c_raw_data) * raw_data_count);
            if(!raw_data_buf)
            {
                printf("malloc failed, size:%d\n",raw_data_count);
                return -1;
            }
        }
        rp = raw_data_buf + raw_data_filled;
        char_len = fscanf(f_scv, "%lf, %d, %d, %d\n", &rp->time_stamp, &rp->i2c_data, &rp->i2c_clk, &rp->i2c_enable);
        if(char_len <= 0){
            printf("end of file\n");
            break;
        }
        raw_data_filled++;
    }
    printf("raw_data_filled:%d\n",raw_data_filled);
#if 0
    {
        int i;
        struct i2c_raw_data rd;
        for(i=0; i<raw_data_filled; i++){
            rd = raw_data_buf[i];
            printf("%lf, %d, %d, %d\n", rd.time_stamp, rd.i2c_data, rd.i2c_clk, rd.i2c_enable);
        }
    }
#endif
}

enum
{
	I2C_STATE_DISABLE,
	I2C_STATE_ENABLE_AND_STOP,
	I2C_STATE_ENABLE_AND_START,
};

enum
{
	I2C_SUB_STATE_BEFORE_DEVICE_ADDR,
	I2C_SUB_STATE_AFTER_DEVICE_ADDR,
};

static char g_i2c_state;
static char g_i2c_sub_state;
static char g_i2c_device_addr;
static unsigned char g_i2c_temp_data;
static char g_current_i2c_bit;
static char g_i2c_is_read;
static char g_i2c_is_ack;

static void parse_i2c(struct i2c_raw_data *rd0, struct i2c_raw_data *rd1)
{
	if(g_i2c_state == I2C_STATE_ENABLE_AND_STOP){
		if(1 == rd0->i2c_clk && 1 == rd1->i2c_clk){
			if(1 == rd0->i2c_data && 0 == rd1->i2c_data){
				g_i2c_state = I2C_STATE_ENABLE_AND_START;
				printf("i2c start\n");
				g_i2c_sub_state = I2C_SUB_STATE_BEFORE_DEVICE_ADDR;
				g_current_i2c_bit = 0;
				g_i2c_device_addr = 0;
				g_i2c_temp_data = 0;
			}
		}
	}else if(g_i2c_state == I2C_STATE_ENABLE_AND_START){
		if(1 == rd1->i2c_clk){
			if(1 == rd0->i2c_data && 0 == rd1->i2c_data){
				g_i2c_state = I2C_STATE_ENABLE_AND_START;
				g_i2c_sub_state = I2C_SUB_STATE_BEFORE_DEVICE_ADDR;
				printf("i2c restart without stop\n");
				g_current_i2c_bit = 0;
				g_i2c_device_addr = 0;
				g_i2c_temp_data = 0;
				return;
			}
		}
		if(1 == rd0->i2c_clk && 1 == rd1->i2c_clk){
			if(0 == rd0->i2c_data && 1 == rd1->i2c_data){
				g_i2c_state = I2C_STATE_ENABLE_AND_STOP;
				printf("i2c stop\n");
			}
		}else if(0 == rd0->i2c_clk && 1 == rd1->i2c_clk){
			int i2c_bit_not_add = 0;
			if(g_i2c_sub_state == I2C_SUB_STATE_BEFORE_DEVICE_ADDR){
				if(g_current_i2c_bit <= 6){
					if(rd1->i2c_data){
						g_i2c_device_addr |= (1 << (6 - g_current_i2c_bit));
					}
				}else if(g_current_i2c_bit == 7){
					if(rd1->i2c_data){
						g_i2c_is_read = 1;
					}else{
						g_i2c_is_read = 0;
					}
					g_i2c_temp_data = 0;
				}else{
					if(rd1->i2c_data){
						g_i2c_is_ack = 0;
					}else{
						g_i2c_is_ack = 1;
						g_i2c_sub_state = I2C_SUB_STATE_AFTER_DEVICE_ADDR;
					}
					i2c_bit_not_add = 1;
					g_current_i2c_bit = 0;
					printf("i2c %s addr:%02x, ack:%d\n", g_i2c_is_read? "read":"write", g_i2c_device_addr, g_i2c_is_ack);
					g_i2c_device_addr = 0;
				}
			}else{
				if(g_current_i2c_bit <= 7){
					if(rd1->i2c_data){
						g_i2c_temp_data |= (1 << (7 - g_current_i2c_bit));
					}
				}else{
					if(rd1->i2c_data){
						g_i2c_is_ack = 0;
					}else{
						g_i2c_is_ack = 1;
					}
					i2c_bit_not_add = 1;
					g_current_i2c_bit = 0;
					printf("i2c data %02x, ack:%d\n", g_i2c_temp_data, g_i2c_is_ack);
					g_i2c_temp_data = 0;
				}
			}
			if(i2c_bit_not_add == 0){
				g_current_i2c_bit++;
			}
		}
	}
}


static void parse_i2c_and_enable(void)
{
    int i;
    struct i2c_raw_data *rd0 = raw_data_buf;
    struct i2c_raw_data *rd1 = raw_data_buf + 1;
    for(i=0; i<raw_data_filled; i++){
		if(g_i2c_state == I2C_STATE_DISABLE){
			if(1 == rd0->i2c_enable && 0 == rd1->i2c_enable){
				g_i2c_state = I2C_STATE_ENABLE_AND_STOP;
				printf("i2c enable\n");
			}
		}else{
			if(0 == rd0->i2c_enable && 1 == rd1->i2c_enable){
				g_i2c_state = I2C_STATE_DISABLE;
				printf("i2c disable\n");
			}else{
				parse_i2c(rd0, rd1);
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
		printf("usage: ./analy_i2c scvfile\n");
		return -1;
	}
	
    read_raw_data(argv[1]);
	parse_i2c_and_enable();
	
	return -1;
}
