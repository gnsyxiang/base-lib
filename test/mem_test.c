/**
 *
 * Release under GPLv2.
 * 
 * @file    mem_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/12 2017 13:54
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/12 2017      create the file
 * 
 *     last modified: 20/12 2017 13:54
 */
#include <stdio.h>
#include <unistd.h>

#include "mem_helper.h"
#include "log_helper.h"
#include "parse_cmd.h"
#include "pthread_helper.h"

static void mem_test(void)
{
	char *p = malloc_mem(10);	

	for (int i = 0; i < 10; i++)
		*(p + i) = i;

	for (int i = 0; i < 10; i++)
		log_i("p[%d]: %d ", i, p[i]);

	free_mem(p);
}

static void heap_stack_address_test(void)
{
    int  first = 0;
    int* p0 = malloc(1024);
    int* p1 = malloc(1024 * 1024);
    int* p2 = malloc(512 * 1024 * 1024);
    int* p3 = malloc(1024 * 1024 * 1024);

    printf("print=%p \n", printf);
    printf("first=%p \n", &first);
    printf("p0=%p, p1=%p, p2=%p, p3=%p \n", p0, p1, p2, p3);
}

/**
 * @brief 测试栈空间大小
 *        主线程栈和线程栈之间栈内存的距离，线程之间栈内存的距离
 * eg:
 * first=0x7fa1f9dc 
 * (0x0x764594a0): first=0x76458f38 
 * (0x0x764594a0): p0=0x75300468 p1=0x751ff008 
 * (0x0x76c594a0): first=0x76c58f38 
 * (0x0x76c594a0): p0=0x41a2d8 p1=0x74dfe008 
 * (0x0x774594a0): first=0x77458f38 
 * (0x0x774594a0): p0=0x75000870 p1=0x74cfd008 
 * (0x0x75c594a0): first=0x75c58f38 
 * (0x0x75c594a0): p0=0x75000468 p1=0x74eff008 
 * (0x0x77c594a0): first=0x77c58f38 
 * (0x0x77c594a0): p0=0x75000c78 p1=0x74bfc008
 *
 * 1, 主线程和线程之间栈内存的距离
 *    0x7fa1f9dc - 0x76458f38 = 0x95C6AA4 = 157051556 = 157051556 / 1024 / 1024 = 149.77M
 *
 * 2, 线程之间栈的内存距离
 *    0x76458f38 - 0x76c58f38 = 0x800000 = 8388608 = 8388608 / 1024 = 8192K
 */

void* thread_proc(void* param)
{
	int  first = 0;
	int* p0 = malloc(1024);
	int* p1 = malloc(1024 * 1024);

	printf("(0x%p): first=%p \n",    (void *)pthread_self(), &first);
	printf("(0x%p): p0=%p p1=%p \n", (void *)pthread_self(), p0, p1);

	return NULL;
}

#define N 5
static void stack_size_test(void)
{
	int first = 0;
	printf("first=%p \n", &first);

	for(int i = 0; i < N; i++)
		create_a_attached_thread(NULL, thread_proc, NULL);

	sleep(1);
}

static void mem_test_init(void)
{
	printf(" 3. heap memory test \n");

	handle_test_cmd_t mem_test_cmd[] = {
		/*{"3", mem_test},*/
		/*{"3", heap_stack_address_test},*/
		{"3", stack_size_test},
	};

	register_test_cmd(mem_test_cmd, ARRAY_NUM(mem_test_cmd));
}
DECLARE_INIT(mem_test_init);

