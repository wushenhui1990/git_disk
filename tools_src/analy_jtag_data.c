#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	RESET,
	IDLE,
	SELECT_DR,
	CAPTURE_DR,
	SHIFT_DR,
	EXIT1_DR,
	PAUSE_DR,
	EXIT2_DR,
	UPDATE_DR,
	SELECT_IR,
	CAPTURE_IR,
	SHIFT_IR,
	EXIT1_IR,
	PAUSE_IR,
	EXIT2_IR,
	UPDATE_IR,
};

struct jtag_tap_name
{
    int tap;
    char *name;
}jtag_tap_name;

static struct jtag_tap_name j_name[] = 
{
    {RESET,      "RESET",     },
    {IDLE,       "IDLE",      },
    {SELECT_DR,  "SELECT_DR", },
    {CAPTURE_DR, "CAPTURE_DR",},
    {SHIFT_DR,   "SHIFT_DR",  },
    {EXIT1_DR,   "EXIT1_DR",  },
    {PAUSE_DR,   "PAUSE_DR",  },
    {EXIT2_DR,   "EXIT2_DR",  },
    {UPDATE_DR,  "UPDATE_DR", },
    {SELECT_IR,  "SELECT_IR", },
    {CAPTURE_IR, "CAPTURE_IR",},
    {SHIFT_IR,   "SHIFT_IR",  },
    {EXIT1_IR,   "EXIT1_IR",  },
    {PAUSE_IR,   "PAUSE_IR",  },
    {EXIT2_IR,   "EXIT2_IR",  },
    {UPDATE_IR,  "UPDATE_IR", },
};

typedef enum
{ 
    ISC_NOOP          = 0x02,
    ISC_ERASE         = 0x05,
    ERASE_DONE        = 0x09,
    READ_ID_CODE      = 0x11,
    READ_USER_CODE    = 0x13,
    ISC_ENABLE        = 0x15,
    FAST_PROGRAM      = 0x17,
    READ_STATUS       = 0x41,
    JTAG_EF_PROGRAM   = 0x71,
    JTAG_EF_READ      = 0x73,
    JTAG_EF_ERASE     = 0x75,
    ISC_DISABLE       = 0x3A,
    REPROGRAM         = 0x3C,
    Bypass            = 0xFF
    
}GWFPGA_Inst_Def;

static struct jtag_tap_name c_name[] = 
{
{ISC_NOOP,"ISC_NOOP"},
{ISC_ERASE,"ISC_ERASE"},
{ERASE_DONE,"ERASE_DONE"},
{READ_ID_CODE,"READ_ID_CODE"},
{READ_USER_CODE,"READ_USER_CODE"},
{ISC_ENABLE,"ISC_ENABLE"},
{FAST_PROGRAM,"FAST_PROGRAM"},
{READ_STATUS,"READ_STATUS"},
{JTAG_EF_PROGRAM,"JTAG_EF_PROGRAM"},
{JTAG_EF_READ,"JTAG_EF_READ"},
{JTAG_EF_ERASE,"JTAG_EF_ERASE"},
{ISC_DISABLE,"ISC_DISABLE"},
{REPROGRAM,"REPROGRAM"},
{Bypass,"Bypass"},
};

#define TIME_STAMPS

struct jtag_raw_data
{
#ifdef TIME_STAMPS
	double time_stamp;
#else
	unsigned long long time_stamp;
#endif
	int tck;
	int tms;
	int tdi;
	int tdo;
}jtag_raw_data;

static struct jtag_raw_data *raw_data_buf;
static int raw_data_count = 0;
static int raw_data_filled = 0;

static int read_raw_data(char *file_name)
{
	int char_len = 0;
	struct jtag_raw_data *rp;
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
            raw_data_buf = realloc(raw_data_buf, sizeof(struct jtag_raw_data) * raw_data_count);
            if(!raw_data_buf)
            {
                printf("malloc failed, size:%d\n",raw_data_count);
                return -1;
            }
        }
        rp = raw_data_buf + raw_data_filled;
#ifdef TIME_STAMPS
        char_len = fscanf(f_scv, "%lf, %d, %d, %d, %d\n", &rp->time_stamp, &rp->tck, &rp->tms, &rp->tdi, &rp->tdo);
#else
        char_len = fscanf(f_scv, "%lld, %d, %d, %d, %d\n", &rp->time_stamp, &rp->tck, &rp->tms, &rp->tdi, &rp->tdo);
#endif
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
        struct jtag_raw_data rd;
        for(i=0; i<raw_data_filled; i++){
            rd = raw_data_buf[i];
            printf("%d, %d, %d, %d, %d\n", rd.time_stamp, rd.tck, rd.tms, rd.tdi, rd.tdo);
        }
    }
#endif
}

static int jtag_tap = RESET;

static unsigned char *ir_in;
static unsigned char ir_in_temp;
static unsigned char *ir_out;
static unsigned char ir_out_temp;

static int ir_count;
static int ir_filled;
static int ir_bit_count;

static unsigned char *dr_out;
static unsigned char dr_out_temp;

static unsigned char *dr_in;
static unsigned char dr_in_temp;

static int dr_count;
static int dr_filled;
static int dr_bit_count;

static int dr_count_pp;
static int dr_relative_count;

static int addr_flag;
static int addr_count;
static int jtag_data(struct jtag_raw_data jd)
{
    if(jd.tck != 1){
        printf("%s, tck error\n", __FUNCTION__);
    }

    if(jtag_tap == SHIFT_IR){
        if(ir_count - ir_filled == 0){
            ir_count += 1024 * 1024;
            ir_in = realloc(ir_in, ir_count);
            if(!ir_in){
                printf("realloc ir_in failed\n");
                return -1;
            }
            ir_out = realloc(ir_out, ir_count);
            if(!ir_out){
                printf("realloc ir_in failed\n");
                return -1;
            }
        }
        if(jd.tdi){
            ir_in_temp |= (1<<ir_bit_count);
        }
        if(jd.tdo){
            ir_out_temp |= (1<<ir_bit_count);
        }
        ir_bit_count++;
        if(ir_bit_count == 8){
            char *p = ir_in + ir_filled;
            *p = ir_in_temp;
            p = ir_out + ir_filled;
            *p = ir_out_temp;
            ir_filled++;
            ir_bit_count = 0;
            //printf("ir in:0x%02x, out:0x%02x\n", ir_in_temp, ir_out_temp);
            if(dr_filled - dr_relative_count){
                //printf("last dr:%d\n",dr_filled - dr_relative_count);
                printf("},\n");
                printf("{\n");
                dr_relative_count = dr_filled;
            }
            if(dr_count_pp){
                //printf("\nir:0x%02x:", ir_in_temp);
                dr_count_pp = 0;
            }else{
                //printf("ir:0x%02x:", ir_in_temp);
            }
            if(ir_in_temp == JTAG_EF_PROGRAM){
                addr_flag = 1;
            }
#if 0
            {
                int i;
                for(i=0; i<sizeof(c_name)/sizeof(c_name[0]); i++)
                {
                    if(ir_in_temp == c_name[i].tap){
                        printf("%s\n",c_name[i].name);
                        break;
                    }
                }
            }
#endif
            ir_in_temp = 0;
            ir_out_temp = 0;
        }
    }

    if(jtag_tap == SHIFT_DR){
        if(dr_count - dr_filled == 0){
            dr_count += 1024;
            dr_in = realloc(dr_in, dr_count);
            if(!dr_in){
                printf("realloc dr_in failed\n");
                return -1;
            }
            dr_out = realloc(dr_out, dr_count);
            if(!dr_out){
                printf("realloc dr_in failed\n");
                return -1;
            }
        }
        if(jd.tdi){
            dr_in_temp |= (1<<dr_bit_count);
        }
        if(jd.tdo){
            dr_out_temp |= (1<<dr_bit_count);
        }
        dr_bit_count++;
        if(dr_bit_count == 8){
            char *p = dr_in + dr_filled;
            *p = dr_in_temp;
            p = dr_out + dr_filled;
            *p = dr_out_temp;
            dr_filled++;
            dr_bit_count = 0;
            //printf("dr in:0x%02x, out:0x%02x\n", dr_in_temp, dr_out_temp);
            if(addr_flag){
                addr_count++;
                if(addr_count == 4){
                    addr_flag = 0;
                    addr_count = 0;
                }
            }else{
                printf("0x%02x,",dr_in_temp);
                dr_count_pp++;
            }
            if(dr_count_pp == 16){
                printf("\n");
                dr_count_pp = 0;
            }
            dr_in_temp = 0;
            dr_out_temp = 0;
        }
    }
    return 0;
}

static int same_taps_count;
static int jtag_tms(struct jtag_raw_data jd)
{
    int last_tap = jtag_tap;
    switch(jtag_tap){
        case RESET:
            if(0 == jd.tms)
                jtag_tap = IDLE;
            break;
        case IDLE:
            if(1 == jd.tms)
                jtag_tap = SELECT_DR;
            break;
        case SELECT_DR:
            if(1 == jd.tms)
                jtag_tap = SELECT_IR;
            else
                jtag_tap = CAPTURE_DR;
            break;
        case CAPTURE_DR:
            if(1 == jd.tms)
                jtag_tap = EXIT1_DR;
            else
                jtag_tap = SHIFT_DR;
            break;
        case SHIFT_DR:
            if(1 == jd.tms)
                jtag_tap = EXIT1_DR;
            break;
        case EXIT1_DR:
            if(1 == jd.tms)
                jtag_tap = UPDATE_DR;
            else
                jtag_tap = PAUSE_DR;
            break;
        case PAUSE_DR:
            if(1 == jd.tms)
                jtag_tap = EXIT2_DR;
            break;
        case EXIT2_DR:
            if(1 == jd.tms)
                jtag_tap = UPDATE_DR;
            else
                jtag_tap = SHIFT_DR;
            break;
        case UPDATE_DR:
            if(1 == jd.tms)
                jtag_tap = SELECT_DR;
            else
                jtag_tap = IDLE;
            break;

        case SELECT_IR:
            if(1 == jd.tms)
                jtag_tap = RESET;
            else
                jtag_tap = CAPTURE_IR;
            break;
        case CAPTURE_IR:
            if(1 == jd.tms)
                jtag_tap = EXIT1_IR;
            else
                jtag_tap = SHIFT_IR;
            break;
        case SHIFT_IR:
            if(1 == jd.tms)
                jtag_tap = EXIT1_IR;
            break;
        case EXIT1_IR:
            if(1 == jd.tms)
                jtag_tap = UPDATE_IR;
            else
                jtag_tap = PAUSE_IR;
            break;
        case PAUSE_IR:
            if(1 == jd.tms)
                jtag_tap = EXIT2_IR;
            break;
        case EXIT2_IR:
            if(1 == jd.tms)
                jtag_tap = UPDATE_IR;
            else
                jtag_tap = SHIFT_IR;
            break;
        case UPDATE_IR:
            if(1 == jd.tms)
                jtag_tap = SELECT_DR;
            else
                jtag_tap = IDLE;
            break;
    }
    if( (last_tap == SHIFT_DR && jtag_tap != SHIFT_DR) || 
            (last_tap == SHIFT_IR && jtag_tap != SHIFT_IR)){
        if(dr_bit_count){
            printf("unfinish 8bit dr,bit count:%d\n",dr_bit_count);
            dr_bit_count = 0;
            dr_out_temp = 0;
        }
        if(ir_bit_count){
            printf("unfinish 8bit ir,bit count:%d\n",ir_bit_count);
            ir_bit_count = 0;
            ir_out_temp = 0;
        }
    }
#if 0
    same_taps_count++;
    if(last_tap != jtag_tap){
        int i;
        printf("count:%d\n",same_taps_count);
        for(i=0; i<=UPDATE_IR; i++)
        {
            if(jtag_tap == j_name[i].tap){
                printf("jtag tap:%s,",j_name[i].name);
                break;
            }
        }
        same_taps_count = 0;
    }
#endif
}

static int judge_delay(struct jtag_raw_data jd0, struct jtag_raw_data jd1)
{
#ifdef TIME_STAMPS
    double delter = jd1.time_stamp - jd0.time_stamp;
    if(delter > 0.00001){ //10us
        printf("delay:%lf\n",delter);
    }
#else
    unsigned long long delter = jd1.time_stamp - jd0.time_stamp;
    if(delter > 100){
        printf("delay:%lld\n",delter);
    }
#endif
}

static int parse_jtag(void)
{
    int i;
    struct jtag_raw_data *rd0 = raw_data_buf;
    struct jtag_raw_data *rd1 = raw_data_buf + 1;
    for(i=0; i<raw_data_filled; i++){
        //judge_delay(*rd0, *rd1);
        if(0 == rd0->tck && 1 == rd1->tck){
            jtag_data(*rd1);
            jtag_tms(*rd1);
        }
        rd0++;
        rd1++;
    }
}

static int p_data(void)
{
    int i;
    printf("\nir in:");
    for(i=0; i<ir_filled; i++){
        if(i%8 == 0)
            printf("\n");
        printf("0x%02x,",ir_in[i]);
    }
    printf("\nir out:");
    for(i=0; i<ir_filled; i++){
        if(i%8 == 0)
            printf("\n");
        printf("0x%02x,",ir_out[i]);
    }
    printf("\ndr in:");
    for(i=0; i<dr_filled; i++){
        if(i%8 == 0)
            printf("\n");
        printf("0x%02x,",dr_in[i]);
    }
    printf("\ndr out:");
    for(i=0; i<dr_filled; i++){
        if(i%8 == 0)
            printf("\n");
        printf("0x%02x,",dr_out[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("usage: ./analy_jtag_scv scvfile\n");
		return -1;
	}
	
    read_raw_data(argv[1]);
    parse_jtag();
    //p_data();
    free(raw_data_buf);
    free(ir_in);
    free(ir_out);
    free(dr_in);
    free(dr_out);
	
	return -1;
}
