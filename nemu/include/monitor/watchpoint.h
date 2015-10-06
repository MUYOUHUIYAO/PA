#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	char expr[50];		//表达式
	uint32_t result; //表达式结果

	/* TODO: Add more members if necessary */


} WP;

void init_wp_list() ;
WP* new_wp(char *s,uint32_t result);
WP* getHead();
bool free_wp(int NUM);

#endif
