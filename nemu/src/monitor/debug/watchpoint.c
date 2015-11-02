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
	if(!success){
		printf("EXPR WRONG!\n");
		assert(0);
		return NULL;
	}
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
void free_wp(WP *wp){
	if(wp==head){
		head=head->next;
		wp->next=free_;
		free_=wp;
		return ;
	}
	WP *search1;
	WP *search2;
	for(search1=head;search1->next!=NULL;search1=search1->next){
		if(search1->next==wp){
			break;
		}
	}
	if(search1==NULL){
		printf("There is no such watchpoint to free!\n");
		return ;
	}
	search1->next=wp->next;
	wp->next=NULL;
	if(wp->NO<free_->NO){
		wp->next=free_;
		free_=wp;
		return ;
	}
	for(search2=free_;search2->next!=NULL;search2=search2->next){
		if(search2->NO<wp->NO){
			break;
		}
	}
	wp->next=search2->next;
	search2->next=wp;
	return ;
}
	





