#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

int mem_dump(void* adr, const void* p, int len)
{
	int i, j;

	unsigned char data;
	const unsigned char* buf;

	buf = p;

	printf(" ADDRESS  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 0123456789ABCDEF\n");
	printf("========= ----------------------------------------------- ----------------\n");

	for (j = 0; j < len / 16; j++)
	{
		printf("%08X  ", adr);
		adr += 16;

		for (i = 0; i < 16; i++)
		{
			data = buf[j * 16 + i];
			printf("%02X ", data);
		}

		for (i = 0; i < 16; i++)
		{
			data = buf[j * 16 + i];
			if ((data < 32) || (data > 127))
				data = '.';

			printf("%c", data);
		}

		printf("\n");
	}

	if (len % 16)
	{
		printf("%08X  ", adr);

		for (i = 0; i < len % 16; i++)
		{
			data = buf[j * 16 + i];
			printf("%02X ", data);
		}

		for (; i < 16; i++)
			printf("   ");

		for (i = 0; i < len % 16; i++)
		{
			data = buf[j * 16 + i];
			if ((data < 32) || (data > 127))
				data = '.';

			printf("%c", data);
		}

		printf("\n");
	}

	printf("\n");

	return 0;
}
#define BUF_NUM 16
#define dprintf printf
struct uac_ring_buf{
    char *buf;
    volatile unsigned long long write;
    volatile unsigned long long read;
    int size;
    int w_wrap;//debug
    int r_wrap;//debug
};

struct snd_uac2_chip {
    struct uac_ring_buf uac_buf;
    unsigned long long sample_rate;
    unsigned long long sample_bits;
};
struct audio_dev {

    int i;
    /* The ALSA Sound Card it represents on the USB-Client side */
    struct snd_uac2_chip uac2;
};
static struct audio_dev *agdev_g;
ssize_t uac_read(char *ubuf, int usize)
{
    struct snd_uac2_chip *uac2 = &agdev_g->uac2;
    struct uac_ring_buf *cbuf = &uac2->uac_buf;
    char *r;
    char *r2;
    char *w;
    char *e = cbuf->buf + cbuf->size - 1;
    int space, space_end;

    if(usize > cbuf->size/2)
        return -1;
    while(cbuf->buf){
        r = (char*)cbuf->read;
        w = (char*)cbuf->write;
        if(w == r){
            space = 0;
        }else if(r > w){
            space_end = e - r;
            space = space_end + (w - cbuf->buf);
        }else{
            space = w - r - 1;
        }
        if(space < usize)
            usleep(1);
        else
            break;
    }
    if(cbuf->buf == 0){
        return -1;
    }
	r2 = r;

    if(r > w){
        if(space_end >= usize){
            memcpy(ubuf, r, usize);
            r += usize;
        }else{
            memcpy(ubuf, r, space_end + 1);
            memcpy(ubuf + space_end + 1, cbuf->buf, usize - space_end - 1);
            r = cbuf->buf + usize - space_end - 1;
        }
    }else{
        memcpy(ubuf, r, usize);
        r += usize;
    }
    cbuf->read = (unsigned long long)r;
	if(cbuf->read == cbuf->write){
		printf("read buf err,r:%p, r2:%p, w:%p,buf:%p, e:%p\n", r, r2, w, cbuf->buf, e);
	}
    return usize;
}

int write_ring_buf(struct snd_uac2_chip *uac2, unsigned char *src, unsigned size)
{
    struct uac_ring_buf *cbuf = &uac2->uac_buf;
    int buf_size = cbuf->size;
    char *w = (char*)cbuf->write;
    char *r = (char*)cbuf->read;
    char *e = cbuf->buf + cbuf->size - 1;
    int space, space_end;

    if(!cbuf->buf){
        return 0;
    }
    while(1){
        w = (char*)cbuf->write;
        r = (char*)cbuf->read;
        if(w == r){
            space = buf_size - 1;
        }else if(w > r){
            space_end = e - w;
            space = space_end + (r - cbuf->buf);
        }else{
            space = r - w -1;
        }

        if(space < size){
            usleep(100);
        }else{
            break;
        }
    }
    if(w == r){
        memcpy(w, src, size);
        w += size;
    }else if(w > r){
        if(space_end >= size){
            memcpy(w, src, size);
            w += size;
        }else{
            memcpy(w, src, space_end + 1);
            memcpy(cbuf->buf, src + space_end + 1, size - space_end - 1);
            w = cbuf->buf + size - space_end - 1;
        }
    }else{
        memcpy(w, src, size);
        w += size;
    }
    cbuf->write = (unsigned long long)w;
	if(cbuf->read == cbuf->write)
		printf("write buf err,r:%p, w:%p\n", r, w);
}

static int prepare_circ_buf(struct uac_ring_buf *cbuf, unsigned long long rate, unsigned bits)
{
    unsigned size = 4096;
    switch(rate){
        case 32000:
        case 44100:
            break;
        case 88200:
        case 96000:
            size *= 2;
            break;
        case 176400:
        case 192000:
            size *= 4;
            break;
    }

    cbuf->buf = malloc(size * BUF_NUM);
    if(!cbuf->buf){
        printf("uac buf malloc failed\n");
        return -1;
    }
    cbuf->write = (unsigned long long)cbuf->buf;
    cbuf->read = (unsigned long long)cbuf->buf;
    cbuf->size = size * BUF_NUM;
    dprintf("prepare buf:%d\n", size * BUF_NUM);
    return 0;
}

static int err = 0;
void * thread_start(void *data)
{
    struct snd_uac2_chip *uac2;
    struct uac_ring_buf *cbuf;
    char *buf = 0;
    int count = 0;
    int *p;
    int i;
	int num;
    uac2 = &agdev_g->uac2;
	cbuf = &uac2->uac_buf;
    while(1){
		num = (random()%40960) & ~(3);
		buf = malloc(num);
		if(!buf)
			return 0 ;
        //printf("w:%08llx,r:%08llx\n",cbuf->write, cbuf->read);
        if(uac_read(buf, num) < 0)
            continue;
        printf("read:%d\n",num);
		p = (int*)buf;
        for(i = 0; i< num/4; i++)
        {
            if(count != *p){
                printf("i:%d, count:%d, *p:%d\n",i, count, *p);
                //mem_dump(buf,buf,4096);
				//printf("num:%d,", num);
				
                count = *p;
                err++;
            }
            p++;
            count++;
            if(err > 0)
                return 0;
        }
		free(buf);
		buf = 0;
    }
}

int main(void)
{
    struct snd_uac2_chip *uac2;
    int s;
    char *buf;
    int count = 0;
    int *p;
    int i;
    struct uac_ring_buf *cbuf;
	int num;
    pthread_t thread_id;        /* ID returned by pthread_create() */
    pthread_attr_t attr;
    agdev_g = malloc(sizeof(*agdev_g));
    uac2 = &agdev_g->uac2;
	cbuf = &uac2->uac_buf;
    prepare_circ_buf(&uac2->uac_buf, 44100, 32);

    s = pthread_attr_init(&attr);

    s = pthread_create(&thread_id, &attr,
            thread_start, 0);
	printf("date:%s, time:%s\n", __DATE__, __TIME__);

    while(1){
		num = (random()%1025) & ~(3);
		//num = 64;
		buf = malloc(num);
        p = (int*)buf;
        for(i=0; i<num/4; i++)
        {
            *p++ = count++;
        }
		//printf("write count:%08x\n",count);
        write_ring_buf(uac2, buf, num);
        printf("write:%d\n",num);
        usleep(2000);
        if(err > 0){
			mem_dump(uac2->uac_buf.buf,uac2->uac_buf.buf,uac2->uac_buf.size);
			printf("w:%08llx,r:%08llx, size:%d\n", cbuf->write, cbuf->read, cbuf->size);
			printf("buf:%p\n", cbuf->buf);
			printf("num:%d, %d\n", num,num%4);
            return 0;
		}
		free(buf);
    }


    return 0;
}
