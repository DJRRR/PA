#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	char *expr;
	uint32_t ans;
	/* TODO: Add more members if necessary */


} WP;

void init_wp_list();

bool new_wp(char *e);

void free_wp(WP* wp);

bool check_watchpoint();

void watchpoint_info();

WP *search_NO(int num);

#endif
