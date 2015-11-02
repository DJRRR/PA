#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;
void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char *e){
	bool success=true;
	uint32_t ans;
	WP *find;
	ans=expr(e,&success);
	if(free_==NULL){
    	printf("There is no free watchpoints\n");
		assert(0);
		return NULL;
	}
	find=free_;
	free_=free_->next;
	find->ans=ans;
	find->expr=e;
	find->next=head;
	head=find;
	return head;
}





