#include <stdio.h>
#include <stdlib.h>

typedef struct Page {
	int flag; //页框标记 
	int access;    //R位，代表是否访问过 
	int modify;    //M位，代表是否修改过 
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
	printf("页号    A    M\n");
	
	do {
		printf("%d      %d    %d\n",p->flag,p->access,p->modify);
		p = p->next;
	} while(p!=head); 
}

/*时钟页面置换算法，把被淘汰的页面的下一个结点指针返回*/ 
Page* clock_displace_algorithm(Page* head, int flag) {
	
	Page* p = head;
	
	/*
	第一轮查找，查找目标为R=0，M=0的结点，并置换 
	*/ 
	do {
		
		if(p->access == 0 && p->modify == 0) {
			
			printf("将页号为%d的页面淘汰\n",p->flag);
		    p->flag = flag;
		    p->access = 1;
		    p->modify = 0;
		    return p->next;
		}
		
		p = p->next;
	}while(p!=head);
	
	/*
	第二轮查找，查找目标为R=0，M=1的结点，并置换,并将所有扫描过的页号的R置为0、
	*/
    do {
		
		if(p->access == 0 && p->modify == 1) {
			
			printf("将页号为%d的页面淘汰\n",p->flag);
		    p->flag = flag;
		    p->access = 1;
		    p->modify = 0;
		    return p->next;
		}
		p->access = 0;
		p = p->next;
		
	}while(p!=head); 
	 
	/*
	若两轮查找之后仍未找到，则再执行一遍，此时必能找到结果，不会出现递归 
	*/ 
	clock_displace_algorithm(head,flag);
}

int direct_alloc(Page* head, int flag) {
	
	Page* p = head;
	do {
		if(p->flag == -1) { p->flag = flag; p->access = 1; p->modify = 0; return 1;}
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
	head->access = 0;
	head->modify = 0; 
	
	printf("输入作业可占用的总页框数:");
	scanf("%d",&number);
	
	for(i=0;i<number-1;i++) {
		
		Page* node = (Page*)malloc(sizeof(Page));
		node->access = 0;
		node->flag = -1;
		node->modify = 0;
		
		add_node(head,node);
	}
	
	return head;
}

/*输入请求序列，先判断能否直接装入，再进行淘汰*/
Page* input_req_sequence(Page* head) {
	
	int flag;
	while(1) {
	
	    printf("请输入一个对页号访问请求的序列，请逐个输入(出现负数视为退出):");
	    scanf("%d",&flag);
	    
	    if(flag < 0) break;
	
	    if(direct_alloc(head,flag)) continue;
		
		if (access_page(head,flag)) {}
	    else { head = clock_displace_algorithm(head,flag); }
	
	    traversal_list_from(head);
    }
    
    return head;
}

/*访问页面*/ 
int access_page(Page* head, int flag) {
	
	Page* p = head;
	
	do {
		if(p->flag == flag) { p->access = 1; printf("访问成功!\n"); return 1;}
		p = p->next;
	}while(p!=head);
	
	printf("页号未找到！\n");
	return 0;
}

/*修改页面*/ 
void modify_page(Page* head) {
	
	Page* p = head;
	int flag;
	
	printf("请输入要修改的页号:");
	scanf("%d",&flag);
	
	do {
		if(p->flag == flag) { p->modify = 1; p->access = 1; printf("修改成功!\n"); return;}
		p = p->next;
	}while(p!=head);
	
	printf("页号未找到！\n");
	
}

/*时钟中断*/
void clock_suspend(Page* head) {
	
	Page* p = head;
	
	do {
		p->access = 0;
		p = p->next;
	}while(p!=head);
}

void user_select(Page* head) {
	
	int sel;
	while(1) {
		printf("输入选项以选择:(1)输入一个访问请求序列(2)打印当前页号序列(3)修改指定页面(4)时钟中断发生(其他)退出:");
	    scanf("%d",&sel);
	    
	    if (sel == 1) head = input_req_sequence(head);
	    else if (sel == 2) traversal_list_from(head);
	    else if (sel == 3) modify_page(head);
	    else if (sel == 4) clock_suspend(head);
	    else break;
	}
}

int main (void) {
	
	Page* head = construct_loop_list();
	
	user_select(head);
}
