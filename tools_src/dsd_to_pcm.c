#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char dsf_header[] =
{
    'D','S', 'D', ' ',
    28, 0, 0, 0, //size of this chunk l
    0, 0, 0, 0, //size of this chunk h
    0, 0, 0, 0,  //file size l *
    0, 0, 0, 0,  //file size h *
    0, 0, 0, 0,  //metadata chunk point l
    0, 0, 0, 0,  //metadata chunk point h
    'f','m', 't', ' ',
    52, 0, 0, 0, //size of this chunk l
    0, 0, 0, 0, //size of this chunk h
    1, 0, 0, 0, //format version
    0, 0, 0, 0, //format ID DSD RAW
    1, 0, 0, 0, //channel type
    1, 0, 0, 0, //channel num
    0x00, 0x11, 0x2b, 0, //sample rate *
    1, 0, 0, 0, //bits per sample
    0, 0, 0, 0,  //sample count l *
    0, 0, 0, 0,  //sample count h *
    0, 0x10, 0, 0,  //block size per channel
    0, 0, 0, 0,  //reserve
    'd', 'a', 't', 'a',
    0, 0, 0, 0, //data size l * n*4096
    0, 0, 0, 0, //data size h *
};

static unsigned int rom0[] = 
{
0xFFFFFD8D,0x000022FA,0x0000EEA9,0x00039FBB,0x000A3E4C,0x0016F0C1,0x002A240B,0x003FC2FC,
0x004CD303,0x003FD88F,0x00066DA6,0xFF989DE7,0xFF06E0F9,0xFE83582C,0xFE5CF09A,0xFEE5F675,
0x00487499,0x02570595,0x0472B4D2,0x059F2993,0x04D11818,0x0169A612,0xFBB5501B,0xF533E1B7,
0xF0758AEC,0xF07A9686,0xF7B657E0,0x07063043,0x1D03C535,0x3606AD25,0x4CEFFBCC,0x5C8DA13E,
0x6123BEC1,0x599357D1,0x47B44AE7,0x2FC1B9B5,0x170EA7B6,0x027952FF,0xF52515AD,0xEFDFA6D9,
0xF150902E,0xF6C72731,0xFD49FCCD,0x0282B679,0x053FEC8B,0x0579E5F9,0x03F74B38,0x01C9FCFB,
0xFFDC0F81,0xFEAFCD15,0xFE5900D2,0xFE9EE905,0xFF2C893E,0xFFB89EA7,0x0019B211,0x00469C7E,
0x004B3C9A,0x003A99CC,0x0024D978,0x00131E86,0x00081780,0x0002AC4F,0x00009E5E,0x00001230,
};

static unsigned int rom1[] = 
{
0x00000314,0x00003D67,0x00015C2B,0x0004D1A2,0x000CC84D,0x001B30B4,0x002F9E6A,0x00446C46,
0x004CCAF7,0x00364117,0xFFEFC3B0,0xFF7631DE,0xFEE1F45B,0xFE6D91DA,0xFE6C08A8,0xFF2A2811,
0x00C030C4,0x02E55F14,0x04DF45A2,0x05A43E50,0x04377E46,0x002A34D4,0xFA126763,0xF3BC30FA,
0xEFE76969,0xF18AFC73,0xFACBCE96,0x0C00DF2B,0x232B26C0,0x3C2A25CF,0x51B50D57,0x5ED40A6A,
0x605D787C,0x55F107F6,0x4215F8B4,0x29715CC3,0x115DADC4,0xFE639583,0xF3178B7C,0xEFB1D5DF,
0xF26C4484,0xF86A51E2,0xFEC883AD,0x03728A0F,0x058584BA,0x05387E5B,0x03717157,0x01417633,
0xFF7C5625,0xFE875D03,0xFE5EFDE7,0xFEBEE31E,0xFF51EDAE,0xFFD5C4EB,0x00299950,0x004AD397,
0x004856AD,0x00352853,0x001FD7A2,0x000FBC38,0x00064BAE,0x0001EE0C,0x0000650F,0x0000083B,
};

static unsigned int rom2[] = 
{
0x0000083B,0x0000650F,0x0001EE0C,0x00064BAE,0x000FBC38,0x001FD7A2,0x00352853,0x004856AD,
0x004AD397,0x00299950,0xFFD5C4EB,0xFF51EDAE,0xFEBEE31E,0xFE5EFDE7,0xFE875D03,0xFF7C5625,
0x01417633,0x03717157,0x05387E5B,0x058584BA,0x03728A0F,0xFEC883AD,0xF86A51E2,0xF26C4484,
0xEFB1D5DF,0xF3178B7C,0xFE639583,0x115DADC4,0x29715CC3,0x4215F8B4,0x55F107F6,0x605D787C,
0x5ED40A6A,0x51B50D57,0x3C2A25CF,0x232B26C0,0x0C00DF2B,0xFACBCE96,0xF18AFC73,0xEFE76969,
0xF3BC30FA,0xFA126763,0x002A34D4,0x04377E46,0x05A43E50,0x04DF45A2,0x02E55F14,0x00C030C4,
0xFF2A2811,0xFE6C08A8,0xFE6D91DA,0xFEE1F45B,0xFF7631DE,0xFFEFC3B0,0x00364117,0x004CCAF7,
0x00446C46,0x002F9E6A,0x001B30B4,0x000CC84D,0x0004D1A2,0x00015C2B,0x00003D67,0x00000314,
};

static unsigned int rom3[] = 
{
0x00001230,0x00009E5E,0x0002AC4F,0x00081780,0x00131E86,0x0024D978,0x003A99CC,0x004B3C9A,
0x00469C7E,0x0019B211,0xFFB89EA7,0xFF2C893E,0xFE9EE905,0xFE5900D2,0xFEAFCD15,0xFFDC0F81,
0x01C9FCFB,0x03F74B38,0x0579E5F9,0x053FEC8B,0x0282B679,0xFD49FCCD,0xF6C72731,0xF150902E,
0xEFDFA6D9,0xF52515AD,0x027952FF,0x170EA7B6,0x2FC1B9B5,0x47B44AE7,0x599357D1,0x6123BEC1,
0x5C8DA13E,0x4CEFFBCC,0x3606AD25,0x1D03C535,0x07063043,0xF7B657E0,0xF07A9686,0xF0758AEC,
0xF533E1B7,0xFBB5501B,0x0169A612,0x04D11818,0x059F2993,0x0472B4D2,0x02570595,0x00487499,
0xFEE5F675,0xFE5CF09A,0xFE83582C,0xFF06E0F9,0xFF989DE7,0x00066DA6,0x003FD88F,0x004CD303,
0x003FC2FC,0x002A240B,0x0016F0C1,0x000A3E4C,0x00039FBB,0x0000EEA9,0x000022FA,0xFFFFFD8D,
};

static char bit_buf0[8];
static char bit_buf1[8];
static char bit_buf2[8];
static char bit_buf3[8];
static char bit_buf_index;

static char zero_cross;

static void put_bit_to_buf(char b0, char b1, char b2, char b3)
{
    char buf_index = bit_buf_index >> 3;
    char bit_shift = bit_buf_index % 8;

    bit_buf0[buf_index] &= ~(1 << bit_shift);
    bit_buf0[buf_index] |= ((!!b0) << bit_shift);

    bit_buf1[buf_index] &= ~(1 << bit_shift);
    bit_buf1[buf_index] |= ((!!b1) << bit_shift);

    bit_buf2[buf_index] &= ~(1 << bit_shift);
    bit_buf2[buf_index] |= ((!!b2) << bit_shift);

    bit_buf3[buf_index] &= ~(1 << bit_shift);
    bit_buf3[buf_index] |= ((!!b3) << bit_shift);
   
    bit_buf_index++;
    bit_buf_index &= 0x3f;
}

static int get_bit(char *buf, char bit_index)
{
    char buf_index = bit_index >> 3;
    char bit_shift = bit_index % 8;

    return (buf[buf_index] >> bit_shift) & 1;
}

void calc_acc_val(long *acc, unsigned int *rom, int dsd_is_high, int coef_count)
{
    int coef = rom[coef_count];
    int mult;
    long acc_t;
    if(dsd_is_high){
        mult = coef;
    }else{
        mult = -coef;
    }
    acc_t =  ((unsigned long)(mult & 0x80000000) << 4);
    acc_t |= ((unsigned long)(mult & 0x80000000) << 3);
    acc_t |= ((unsigned long)(mult & 0x80000000) << 2);
    acc_t |= ((unsigned long)(mult & 0x80000000) << 1);
    acc_t |= mult;
    //acc_t &= (unsigned long)0xfffffffff;
    if(coef_count == 0){
        *acc = acc_t;
    }else{
        *acc = *acc + acc_t;
    }
    //*acc &= (unsigned long)0xfffffffff;
    //printf("acc2:%lx\n", *acc);
#if 0
    if(rom == rom0 && zero_cross){
        printf("coef:0x%08x, coef_count:0x%02x, mult:%08x, acc:%09x\n",coef, coef_count, mult, *acc);
    }
#endif
}
typedef struct {
  // char id[4]; Read by upper level
  long size;
  char prop[4];
} dsdiff_header;

typedef struct {
  char id[4];
  long size;
} dsdiff_chunk_header;
static int p_data(char *file_name)
{
    int read_len;
    long data_len = 0;
    long i;
    int j, k;
    int coef_count = 0;
    long acc0 = 0;
    long acc1 = 0;
    long acc2 = 0;
    long acc3 = 0;
    long pcm;
    long pcmx;
    int pcm_sample;
    unsigned char r_start_index = 0;
    int dsd_r_byte = 0;
    int pcm_byte = 0;
    unsigned int sample;
    FILE* f= NULL;
    f= fopen(file_name,"r");
    if(!f)
    {
        printf("open:%s,failed\n",file_name);
        return -1;
    }

    while(fscanf(f, "%x", &sample)){
        unsigned char data = 0;
        data = sample;
        dsd_r_byte += 1;
        //printf("0x0000%02x\n", data);
        for(j=0; j<8; j+=4){
            put_bit_to_buf( !!(data & (0x80 >> j)), !!(data & (0x80 >> (j + 1))), !!(data & (0x80 >> (j + 2))), !!(data & (0x80 >> (j + 3))));
        }

        r_start_index = bit_buf_index;
        for(j=0; j<64; j++){
            calc_acc_val(&acc0, rom0, get_bit(bit_buf0, r_start_index), j);
            calc_acc_val(&acc1, rom1, get_bit(bit_buf1, r_start_index), j);
            calc_acc_val(&acc2, rom2, get_bit(bit_buf2, r_start_index), j);
            calc_acc_val(&acc3, rom3, get_bit(bit_buf3, r_start_index), j);
            r_start_index++;
            r_start_index &= 0x3f;
        }
        pcm = acc0 + acc1 + acc2 + acc3;
        if(pcm & ((long)0x3 << 33) == (long)0x3 << 33){
            pcmx = 0xC0000000;
        }else if(pcm & ( (long)0x4 << 33) == (long)0x4 << 33){
            pcmx = 0x13fffffff;
        }else{
            pcmx = (pcm >> 3) & 0x1ffffffff;
        }
        if(((pcmx >> 28) & 0x1f) == 0 ||
                ((pcmx >> 28) & 0x1f) == 0x1f){
            zero_cross = 1;
        }
        pcm_sample = (pcmx >> 1) + (pcmx & 1);
        if(zero_cross){
            printf("0x%08x\n", pcm_sample);
            pcm_byte += 4;
        }
        //printf("%02x%02x00\n", (data >> 8), data & 0xff);
    }
    //printf("dsd bytes:%d,pcm bytes:%d\n", dsd_r_byte, pcm_byte);
    fclose(f);
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("args: dsd text file\n");
        return -1;
    }

    return p_data(argv[1]);
}
