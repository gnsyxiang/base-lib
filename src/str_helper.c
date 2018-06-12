/**
 * 
 * Release under GPLv2.
 * 
 * @file    str_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/01 2018 09:30
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/01 2018      create the file
 * 
 *     last modified: 26/01 2018 09:30
 */
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>

#include "misc_helper.h"
#include "mem_helper.h"

#define STR_HELPER_GB
#include "str_helper.h"
#undef STR_HELPER_GB

/**
 * @brief first and end exchange string
 *
 * @param str: the switched string
 */
void str_swap_first_and_end(char *str)
{
	int len = strlen(str);
	int cnt = 0;

	while (cnt < len / 2) {
		swap(str[cnt], str[len - cnt - 1]);
		cnt++;
	}
}

/**
 * @brief get the extension name
 *
 * @param file_name: file path
 * @param ext_name: the extension name of the file 
 */
void str_get_file_extension_name(const char *file_name, char *ext_name)  
{  
	const char *p = file_name + strlen(file_name) - 1;
	char *pext_name = ext_name;

	while (*p != '.')
		*ext_name++ = *p--;

	*ext_name = '\0';

	str_swap_first_and_end(pext_name);
}  

void str_get_file_name_no_extension_name(const char *file_name, char *name)
{
	int str_len = strlen(file_name) - 1;
	const char *p = file_name + str_len;
	int cnt = 0;

	while (*p-- != '.')
		cnt++;

	memcpy(name, file_name, str_len - cnt);

	name[str_len - cnt] = '\0';
}

/**
 * @brief replace the specified string in a string 
 *
 * @param src: original string
 * @param oldstr: old string
 * @param newstr: new string
 * @param num: replace times
 *
 * @return return the new string after the replacement
 */
const char *str_replace_substr(const char *src, const char *oldstr, const char *newstr, int num)
{
	const char *dst = src;
	char *tmp = NULL;
	char *find_oldstr;
	int replace_cnt = 0;

	if (strcmp(oldstr, newstr) == 0) {
		dst = strdup(tmp);
		return dst;
	}

	while ((find_oldstr = strstr(dst, oldstr)) && replace_cnt++ < num) {
		tmp = alloc_mem(strlen(dst) + (strlen(newstr) + strlen(oldstr)) + 1);

		strncpy(tmp, dst, find_oldstr - dst);
		tmp[find_oldstr - dst] = '\0';

		strcat(tmp, newstr);

		strcat(tmp, find_oldstr + strlen(oldstr));

		dst = strdup(tmp);

		free_mem(tmp);
	}

	return dst;
}

int getch(void) 
{
	int c = 0;
	struct termios org_opts, new_opts;
	int res = 0;

	//-----  store old settings -----------
	res = tcgetattr(STDIN_FILENO, &org_opts);
	assert(res == 0);

	//---- set new terminal parms --------
	memcpy(&new_opts, &org_opts, sizeof(new_opts));
	new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);

	c = getchar();

	//------  restore old settings ---------
	res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
	assert(res==0);

	return c;
}

#ifdef USE_HOOK
/**
 * @brief LD_PRELOAD=xxx
 *        优先加载xxx路径的库
 * http://www.cnblogs.com/LittleHann/p/3854977.html?utm_source=tuicool&utm_medium=referral
 */
int strcmp(const char *s1, const char *s2)
{
	static void *handle = NULL;
	static strcmp_t old_strcmp = NULL;

	if(!handle) {
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_strcmp = (strcmp_t)dlsym(handle, "strcmp");
	}

	// do somethings
	printf("hook strcmp: s1=<%s>, s2=<%s> \n", s1, s2);

	return old_strcmp(s1, s2);
}
#endif

