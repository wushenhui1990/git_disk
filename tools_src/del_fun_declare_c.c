
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

char *fun_begin_2[]={
	"int ",
	"void ",
	"bool ",
	"char ",
	"short ",
	"long ",
	"unsigned ",
	"struct ",
	"enum ",
	"ssize_t ",
	"size_t ",
	NULL,
};

char *fun_type_prev[] ={
	"const static",
	"static",
	"const",
	NULL,
};

char *strrstr(char *src, int src_revert_size, char *find, char find_size)
{
	int i;
	for(i=find_size; i<src_revert_size; i++){
		char trial = *(src-i+find_size );
		if(strncmp(src-i, find, find_size) == 0 &&(trial == ' ' || trial == '\n')){
			break;
		}
		if(*(src - i) == '}' || *(src - i) == ';' || *(src - i) == '"' ||
				*(src - i) == '{' || *(src-i) == ')' || *(src - i) == '(' ||
				( *(src-i) == '\n' &&  *(src-i-1) == '\n') ){

			return NULL;
		}
	}

	if(i >= src_revert_size){
		return NULL;
	}

	//printf(" fn start:%p\n", src - i);
	return src - i;

}


char *find_str_p_revert(char *need_find_str, int revert_size, char *find_char[])
{
	int i = 0;
	char *find_char_p = NULL;
	while(1){
		if(find_char[i] == NULL){
			break;
		}
		find_char_p	= strrstr(need_find_str, revert_size, find_char[i], strlen(find_char[i]));
		if(find_char_p){
			break;
		}
		i++;
	}
	return find_char_p;
}

char *strnstr(char *find, int find_len, char *to_find)
{
	char *a = malloc(find_len + 1);
	memset(a, 0, find_len + 1);
	return strstr(find, to_find);
}

char *find_str_p(char *need_find_str, int find_len, char *find_char[])
{
	int i = 0;
	char *find_char_p = NULL;
	while(1){
		if(find_char[i] == NULL){
			break;
		}
		find_char_p	= strnstr(need_find_str, find_len, find_char[i]);
		if(find_char_p){
			break;
		}
		i++;
	}
	return find_char_p;
}

int del_fun(char *file_buf, int size, char *fun_name)
{
	char *fun_name_p;
	char *fun_start_p = NULL;
	int pattern_l_count = 0;
	char *pattern_l_p;
	int pattern_r_count = 0;
	char *pattern_r_p;

	fun_name_p = strstr(file_buf, fun_name);
	
	if(fun_name_p){
		char *temp_p;
		temp_p	= find_str_p_revert(fun_name_p, fun_name_p - file_buf, fun_begin);
		if(temp_p){
			fun_start_p = temp_p;
			temp_p	= find_str_p_revert(fun_name_p, fun_name_p - file_buf, fun_type_prev);
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
	pattern_l_count = 0;
	pattern_r_count = 0;
	
	pattern_l_p = strchr(fun_name_p, '(');
	if(!pattern_l_p){
		printf("find pattern l error\n");
		return -1;
	}

	pattern_r_p = strchr(fun_name_p, ')');
	if(pattern_r_p && *(pattern_r_p+1) == ';'){
		pattern_r_p = pattern_r_p + 1;
	}else{
		return -1;
	}

	if(find_str_p(pattern_l_p, pattern_r_p - pattern_l_p, fun_begin_2) == 0){
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
