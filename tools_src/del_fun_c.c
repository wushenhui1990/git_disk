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
	"struct",
	"enum",
	"ssize_t",
	"size_t",
	NULL,
};

char *fun_type_prev[] ={
	"const static",
	"static",
	"const",
	"unsigned",
	NULL,
};

char *strrstr_fun_type(char *src, int src_revert_size, char *find, char find_size)
{
	int i;
	for(i=find_size; i<src_revert_size; i++){
		char trial = *(src-i+find_size );
		char pre = *(src - i - 1);
		if(strncmp(src-i, find, find_size) == 0 && (trial == ' ' || trial == '\n') &&
				(pre == ' ' || pre == '\n')){
			break;
		}
		if(*(src - i) == '}' || *(src - i) == ';' || *(src - i) == '"' ||
				*(src - i) == '{' || *(src-i) == ')' || *(src - i) == '(' ||
				*(src - i) == '*' || *(src-i) == '/' || *(src - i) == '\\' ||
				( *(src-i) == '\n' &&  pre == '\n') ){

			return NULL;
		}
	}

	if(i >= src_revert_size){
		return NULL;
	}

	//printf(" fn start:%p\n", src - i);
	return src - i;

}


char *find_fun_type_p(char *need_find_str, int revert_size, char *find_char[])
{
	int i = 0;
	char *find_char_p = NULL;
	while(1){
		if(find_char[i] == NULL){
			break;
		}
		find_char_p	= strrstr_fun_type(need_find_str, revert_size, find_char[i], strlen(find_char[i]));
		if(find_char_p){
			break;
		}
		i++;
	}
	return find_char_p;
}

char *strrstr_fun_type_prev(char *src, int src_revert_size, char *find, char find_size)
{
	int i;
	for(i=find_size; i<src_revert_size; i++){
		char trial = *(src-i+find_size );
		char pre = *(src - i - 1);
		//if(pre != ' ' && pre != '\n')
		//	return NULL;
		if(strncmp(src-i, find, find_size) == 0 && (trial == ' ' || trial == '\n') &&
				(pre == ' ' || pre == '\n')){
			break;
		}
		if(*(src - i) == '}' || *(src - i) == ';' || *(src - i) == '"' ||
				( *(src-i) == '\n' &&  pre == '\n') ){

			return NULL;
		}
	}

	if(i >= src_revert_size){
		return NULL;
	}

	//printf(" fn start:%p\n", src - i);
	return src - i;

}
char *find_fun_type_prev_p(char *need_find_str, int revert_size, char *find_char[])
{
	int i = 0;
	char *find_char_p = NULL;
	while(1){
		if(find_char[i] == NULL){
			break;
		}
		find_char_p	= strrstr_fun_type_prev(need_find_str, revert_size, find_char[i], strlen(find_char[i]));
		if(find_char_p){
			break;
		}
		i++;
	}
	return find_char_p;
}

int find_pattern(char *buf, char **l_p, char l, char **r_p, char r)
{
	int pattern_l_count = 0;
	char *pattern_l_p;
	int pattern_r_count = 0;
	char *pattern_r_p;

	pattern_l_count = 0;
	pattern_r_count = 0;
	
	pattern_l_p = strchr(buf, l);
	if(!pattern_l_p){
		printf("find pattern l error\n");
		return -1;
	}
	pattern_l_count++;

	pattern_r_p = pattern_l_p;
	while(1){
		pattern_l_p = strchr(pattern_l_p+1, l);

		pattern_r_p = strchr(pattern_r_p+1, r);

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

	*l_p = pattern_l_p;
	*r_p = pattern_r_p;
	return 0;
}

int del_fun(char *file_buf, int size, char *fun_name)
{
	char *fun_name_p;
	char *fun_start_p = NULL;
	char *pattern_l_p;
	char *pattern_r_p;

	fun_name_p = strstr(file_buf, fun_name);
	
	if(fun_name_p){
		char *temp_p;
		temp_p	= find_fun_type_p(fun_name_p, fun_name_p - file_buf, fun_begin);
		if(temp_p){
			fun_start_p = temp_p;
			temp_p	= find_fun_type_prev_p(fun_name_p, fun_name_p - file_buf, fun_type_prev);
			if(temp_p)
				fun_start_p = temp_p;
		}else{
			printf("find fun start err\n");
			return -1;
		}
	}else{
		printf("find fun name error\n");
		return -1;
	}

	if(!fun_start_p){
		printf("find fun start error\n");
		return -1;
	}

	if(find_pattern(fun_name_p, &pattern_l_p, '(', &pattern_r_p, ')') == 0){
		if( *(pattern_r_p + 1) == ';'){ //declare
			pattern_r_p += 1;
		}else{
			if(find_pattern(pattern_r_p, &pattern_l_p, '{', &pattern_r_p, '}') == 0){
			}else{
				printf("find pattern { error\n");
				return -1;
			}
		}
	}else{
		printf("find pattern ( error\n");
		return -1;
	}
	
	
	int fun_size = pattern_r_p - fun_start_p + 1;
	int left_size = (file_buf + size) - pattern_r_p;
	memcpy(fun_start_p, pattern_r_p+1, left_size);
	memset(file_buf + size - fun_size, 0, fun_size);

	printf("%s\n", file_buf);
	return 0;
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

	ret = del_fun(file_buf, size, argv[1]);

	free(file_buf);

	return ret;

}
