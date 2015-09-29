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

WP* new_wp(char *s,uint32_t result){
	if(free_==NULL) {
		printf("监视点已满\n");
		return NULL;
	}
	WP *tail=head;
	if(tail==NULL){			//从free_链表头取节点
			head=free_;
			free_=free_->next;
			head->next=NULL;
			head->NO=1;
			strcpy(head->expr,s);
			head->result=result;
			return head;
	}
	while(tail->next) tail=tail->next;
	tail->next=free_;
	free_->NO=tail->NO+1;
	free_->result=result;
	strcpy(free_->expr,s);
	free_=free_->next;
	tail->next->next=NULL;
	return tail->next;
}

void free_wp(int NUM){
	if(head==NULL) {
		printf("没有监视点\n");
		return;
	}
	WP *tail=head,*ptr=head->next;
	if(tail->NO==NUM){							//向free_链表头释放节点
		if(tail->next==NULL){
			head->next=free_;
			free_=head;
			head=NULL;
			return;
		}else{
			head=head->next;
			tail->next=free_;
			free_=tail;
			return;
		}
	}
	while(ptr&&ptr->NO!=NUM){
		tail=ptr;
		ptr=ptr->next;
	}
	if(ptr==NULL){					//序号超出
		printf("无此监视点\n");
		return;
	}
	if(free_==NULL){				//free_链表为空
		free_=ptr;
		ptr->next=NULL;
		tail->next=NULL;
		return;
	}
	tail->next=ptr->next;			
	ptr->next=free_;
	free_=ptr;
	tail=head;
	head->NO=1;
	while(tail->next){			//更新序号
		tail->next->NO=tail->NO+1;
		tail=tail->next;
	}
}

WP* getHead(){
	return head;
}

