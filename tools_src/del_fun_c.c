#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *fun_begin[]={
	"int",
	"void",
	"bool",
	"char",
	"short",
	"long",
	"unsigned",
	"ssize_t",
	"size_t",
	NULL,
};


char *strrstr(char *src, int src_revert_size, char *find, char find_size)
{
	int i;
	char static_str[] = "static ";
	//printf("find :%s, size:%d\n", find, find_size);
	for(i=find_size; i<src_revert_size; i++){
		char trial = *(src-i+find_size );
		if(strncmp(src-i, find, find_size) == 0 &&(trial == ' ' || trial == '\n')){
			break;
		}
		if(*(src - i) == '}' || *(src - i) == ';'){

			return NULL;
		}
	}

	if(i >= src_revert_size){
		return NULL;
	}
	if(src_revert_size - i > strlen(static_str)){
		if(strncmp((char*)(src - i - strlen(static_str)), static_str, strlen(static_str)) == 0){
			return src - i - strlen(static_str);
		}
	}


	//printf(" fn start:%p\n", src - i);
	return src - i;

}

int del_fun(char *file_buf, int size, char *fun_name)
{
	char *fun_name_p;
	char *fun_start_p = NULL;
	int i = 0;
	int pattern_l_count = 0;
	char *pattern_l_p;
	int pattern_r_count = 0;
	char *pattern_r_p;

	fun_name_p = strstr(file_buf, fun_name);
	
	if(fun_name_p){
		while(1){
			if(fun_begin[i] == NULL){
				break;
			}
			fun_start_p	= strrstr(fun_name_p, fun_name_p - file_buf, fun_begin[i], strlen(fun_begin[i]));
			if(fun_start_p){
				break;
			}
			i++;
		}
	}else{
		printf("find fun mane error\n");
		return -1;
	}

	if(!fun_start_p){
		printf("find fun start error\n");
		return -1;
	}
	pattern_l_count = 0;
	pattern_r_count = 0;
	
	pattern_l_p = strchr(fun_name_p, '{');
	if(!pattern_l_p){
		printf("find pattern l error\n");
		return -1;
	}
	pattern_l_count++;

	pattern_r_p = pattern_l_p;
	while(1){
		pattern_l_p = strchr(pattern_l_p+1, '{');

		pattern_r_p = strchr(pattern_r_p+1, '}');

		if(pattern_l_p != NULL && pattern_l_p < pattern_r_p ){
			pattern_l_count++;
		}
		if(pattern_r_p){
			pattern_r_count++;
		}else{
			printf("find pattern r error\n");
			return -1;
		}
		
		if(pattern_l_count == pattern_r_count){
			break;
		}
	}
	
	int fun_size = pattern_r_p - fun_start_p;
	int left_size = (file_buf + size) - pattern_r_p;
	memcpy(fun_start_p, pattern_r_p+1, left_size);
	memset(file_buf + size - fun_size, 0, fun_size);

	printf("%s\n", file_buf);
}

int main(int argc, char *argv[])
{
	FILE *fd;
	int size;
	char *file_buf;
	int ret;

	if(argc < 3){
		printf("usage del_fun_c fun_name file_name\n");
		return 0;
	}
	fd = fopen(argv[2], "r+");
	if(!fd){
		printf("open %s, error\n", argv[2]);
		return -1;
	}

	fseek(fd, SEEK_SET, SEEK_END);
	size = ftell(fd);
	fseek(fd, SEEK_SET, SEEK_SET);

	file_buf = malloc(size);
	if(!file_buf){
		printf("malloc failed\n");
		fclose(fd);
		return -1;
	}

	if(fread(file_buf, 1, size,fd) != size){
		printf("read error, size:%d\n", size);
		fclose(fd);
		return -1;
	}

	fclose(fd);

	del_fun(file_buf, size, argv[1]);

	free(file_buf);

	return 0;

}
