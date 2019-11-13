#include <stdio.h>
#include <stdlib.h>

typedef struct Page {
	int flag; //页框标记 
	int read;    //R位，代表是否读过 
	int modify;    //M位，代表是否写过 
	struct Page* next;  //下一节点 
	
}Page;

void add_node(Page* head, Page* node) {
	
	Page* p = head;
	while(p->next != head) p = p->next;
	
	p->next = node;
	node->next = head;
}

void traversal_list_from(Page* head) {
	
	Page* p = head;
	
	printf("当前总页框情况(-1页号说明该页尚未被使用):\n");
	printf("页号    R    M\n");
	
	do {
		printf("%d      %d    %d\n",p->flag,p->read,p->modify);
		p = p->next;
	} while(p!=head); 
}

void clock_displace_algorithm(Page* head, int flag, int read, int modify) {
	
	Page* p = head;
	
	/*
	第一轮查找，查找目标为R=0，M=0的结点，并置换 
	*/ 
	do {
		
		if(p->read == 0 && p->modify == 0) {
			
			printf("将页号为%d的页面淘汰\n",p->flag);
		    p->flag = flag;
		    p->read = read;
		    p->modify = modify;
		    return;
		}
		
		p = p->next;
	}while(p!=head);
	
	/*
	第二轮查找，查找目标为R=0，M=1的结点，并置换,并将所有扫描过的页号的R置为0、
	*/
    do {
		
		if(p->read == 0 && p->modify == 1) {
			
			printf("将页号为%d的页面淘汰\n",p->flag);
		    p->flag = flag;
		    p->read = read;
		    p->modify = modify;
		    return;
		}
		p->read = 0;
		p = p->next;
		
	}while(p!=head); 
	 
	/*
	若两轮查找之后仍未找到，则再执行一遍，此时必能找到结果，不会出现递归 
	*/ 
	clock_displace_algorithm(head,flag,read,modify);
}

int direct_alloc(Page* head, int flag, int read, int modify) {
	
	Page* p = head;
	do {
		if(p->flag == -1) { p->flag = flag; p->read = read; p->modify = modify; return 1;}
		p = p->next;
	}while(p!=head);
	
	return 0;
}

Page* construct_loop_list() {
	
	int i;
	int number;
	
	Page* head = (Page*)malloc(sizeof(Page));
	head->next = head;
	head->flag = -1;
	head->read = 0;
	head->modify = 0; 
	
	printf("输入作业可占用的总页框数:");
	scanf("%d",&number);
	
	for(i=0;i<number-1;i++) {
		
		Page* node = (Page*)malloc(sizeof(Page));
		node->read = 0;
		node->flag = -1;
		node->modify = 0;
		
		add_node(head,node);
	}
	
	return head;
}

void input_req_sequence(Page* head) {
	
	int flag, read, modify;
	
	while(1) {
	
	    printf("请输入一个对页号访问请求的序列，格式为(请求的页号 是否读 是否写)其中是为1否为0 出现负数视为退出:");
	    scanf("%d %d %d",&flag,&read,&modify);
	    
	    if(flag < 0 || read < 0 || modify < 0) break;
	
	    if(direct_alloc(head,flag,read,modify)) continue;
	
	    clock_displace_algorithm(head,flag,read,modify);
	
	    traversal_list_from(head);
    }
}

void user_select(Page* head) {
	
	int sel;
	while(1) {
		printf("输入选项以选择:(1)输入一个请求序列(2)打印当前页号序列(其他)退出:");
	    scanf("%d",&sel);
	    
	    if (sel == 1) input_req_sequence(head);
	    else if (sel == 2) traversal_list_from(head);
	    else break;
	}
}

int main (void) {
	
	Page* head = construct_loop_list();
	
	user_select(head);
}
