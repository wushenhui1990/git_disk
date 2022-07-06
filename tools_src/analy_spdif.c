#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct jtag_raw_data
{
	double time_stamp;
	int val;
}jtag_raw_data;

static struct jtag_raw_data *raw_data_buf;
static int raw_data_count = 0;
static int raw_data_filled = 0;

#define DIFF_COUNT 128
#define DIFF_MARGIN 10
#define RATE_48_1_CELL_TIME 81
#define RATE_48_2_CELL_TIME 162
#define RATE_48_3_CELL_TIME 244
static void put_buf(int buf[], int buf_count[], int val)
{
	int i;
	for(i=0; i<DIFF_COUNT; i++){
		if(buf[i] == val){
			buf_count[i]++;
			return;
		}
		if(buf[i] == 0){
			buf[i] = val;
			buf_count[i]++;
			return;
		}
	}
	printf("buf over flow\n");
}

static void sort_buf(int buf[], int buf_count[])
{
	int i;
	int j;
	int max_val = 0;
	int max_val_index = 0;
	int temp;
	int count = 0;
	for(i=0; i<DIFF_COUNT; i++){
		if(buf[i] == 0){
			break;;
		}
		count++;
	}

	for(i=0; i<count; i++){
		max_val = buf[i];
		for(j=i+1; j<count; j++){
			if(buf[j] > max_val){
				max_val = buf[j];
				max_val_index = j;
			}
		}
		if(max_val_index != i){
			temp = buf[i];
			buf[i] = buf[max_val_index];
			buf[max_val_index] = temp;

			temp = buf_count[i];
			buf_count[i] = buf_count[max_val_index];
			buf_count[max_val_index] = temp;
		}
	}
}
static void parse(void)
{
	int cell_1_diff[DIFF_COUNT] = {0};
	int cell_2_diff[DIFF_COUNT] = {0};
	int cell_3_diff[DIFF_COUNT] = {0};
	int other_diff[DIFF_COUNT] = {0};

	int cell_1_diff_count[DIFF_COUNT] = {0};
	int cell_2_diff_count[DIFF_COUNT] = {0};
	int cell_3_diff_count[DIFF_COUNT] = {0};
	int other_diff_count[DIFF_COUNT] = {0};

	int i;
	int last_ns;
	int ns;
	int diff;
	struct jtag_raw_data rd;
	rd = raw_data_buf[0];
	last_ns = rd.time_stamp * 1000000000;
	for(i=1; i<raw_data_filled; i++){
		rd = raw_data_buf[i];
		ns = rd.time_stamp * 1000000000;
		diff = ns - last_ns;
		if(diff >= RATE_48_1_CELL_TIME - DIFF_MARGIN && 
			diff <= RATE_48_1_CELL_TIME + DIFF_MARGIN){
			put_buf(cell_1_diff, cell_1_diff_count, diff);
		}else if(diff >= RATE_48_2_CELL_TIME - DIFF_MARGIN && 
			diff <= RATE_48_2_CELL_TIME + DIFF_MARGIN){
			put_buf(cell_2_diff, cell_2_diff_count, diff);
		}else if(diff >= RATE_48_3_CELL_TIME - DIFF_MARGIN && 
			diff <= RATE_48_3_CELL_TIME + DIFF_MARGIN){
			put_buf(cell_3_diff, cell_3_diff_count, diff);
		}else{
			put_buf(other_diff, other_diff_count, diff);
		}
		last_ns = ns;
	}

	sort_buf(cell_1_diff, cell_1_diff_count);
	for(i=0; i<DIFF_COUNT; i++){
		if(cell_1_diff[i] == 0){
			break;
		}
		printf("cell 1 diff:%d, count:%d\n", cell_1_diff[i], cell_1_diff_count[i]);
	}
	printf("-----------------------------\n");
	sort_buf(cell_2_diff, cell_2_diff_count);
	for(i=0; i<DIFF_COUNT; i++){
		if(cell_2_diff[i] == 0){
			break;
		}
		printf("cell 2 diff:%d, count:%d\n", cell_2_diff[i], cell_2_diff_count[i]);
	}
	printf("-----------------------------\n");
	sort_buf(cell_3_diff, cell_3_diff_count);
	for(i=0; i<DIFF_COUNT; i++){
		if(cell_3_diff[i] == 0){
			break;
		}
		printf("cell 3 diff:%d, count:%d\n", cell_3_diff[i], cell_3_diff_count[i]);
	}

	printf("-----------------------------\n");
	sort_buf(other_diff, other_diff_count);
	for(i=0; i<DIFF_COUNT; i++){
		if(other_diff[i] == 0){
			break;
		}
		printf("other diff:%d, count:%d\n", other_diff[i], other_diff_count[i]);
	}
}

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
        char_len = fscanf(f_scv, "%lf, %d\n", &rp->time_stamp, &rp->val);
        if(char_len <= 0){
            printf("end of file\n");
            break;
        }
        raw_data_filled++;
    }
    printf("raw_data_filled:%d\n",raw_data_filled);
}


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("usage: ./analy_jtag_scv scvfile\n");
		return -1;
	}
	
    read_raw_data(argv[1]);
    parse();
    free(raw_data_buf);
	
	return -1;
}
