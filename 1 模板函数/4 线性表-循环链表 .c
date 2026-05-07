#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>// qsort
#include <time.h>
//#include <windows.h>//（windows特有库）sleep函数等（含max、min）
//#include <conio.h>//（windows特有库）getch函数等
#include <errno.h>// 检查strtol转换溢出问题→"errno"

#define eps 1e-8
#define array_len(x) (sizeof(x)/sizeof(x[0]))//计算数组长度

/*
数据结构-线性表-循环链表 函数实现
基本信息类型：intclink
节点类型：cnode
若同时建立多个顺序表，需建立多个cnode变量类型 和 多个基本信息变量，并将所有函数的cnode分别换成对应词
（即：一种基本信息结构体、多种cnode、多套函数）
*/

static int clink_error_output = 1;  // 默认打印错误
// 链表节点结构（除link部分外可自定义）
typedef struct CNode {
        int num;
        struct CNode *link;
} cnode;

// 基本信息结构体
typedef struct {
	cnode *head;// 链表首节点地址
	cnode *tail;// 链表尾节点地址
	int size;// 链表节点个数
	
} intclink, *intclink_p;

void clink_set_error_output(int enable);
intclink* clink_intlist(int n, int sign);
cnode* clink_getplace(intclink *list, int t);
cnode* clink_addnode_pr(intclink *list, cnode *t);
cnode* clink_addcnode_head(intclink *list);
cnode* clink_addcnode_tail(intclink *list);
cnode* clink_addcnode(intclink *list, int k);
int clink_deletecnode(intclink *list, int k);
void clink_destroylist(intclink *list);
int clink_scanf(intclink *list, int k);
void clink_fillcnode_0(cnode *q);
int clink_fillcnode(cnode *q);
int clink_printlist(intclink *list);

int main() {
	
	
	
return 0;
}

// 自定义函数

// 控制是否可视化打印错误信息
void clink_set_error_output(int enable) {// 1→打印；0→不打印
    clink_error_output = enable ? 1 : 0;
    return;
}

// 创建链表（成功链表信息结构体指针，不成功可视化错误处理并返回NULL）
intclink* clink_intlist(int n, int sign) {// 节点总数n，是否录入数据（录入=1，不录入=0）
	intclink *list;
	if(!(list=(intclink*)malloc(sizeof(intclink)))) {// 创建链表信息并判断是否成功
		if (clink_error_output) printf("There is not enough space for the new linklist.\n");
		return NULL;
	}
	list->head=NULL;
	list->size=0;
	list->tail=NULL;
	
	cnode *q;
	
	if(n==0) {// 如果要创建空链表
		return list;
	}
	
	if(sign==0) {// 不录入数据
		for(int i=1; i<=n; i++) {// 循环创建n个节点（并全部写入0）
			if(!(q = (cnode*)malloc(sizeof(cnode)))) {// 创建节点并判断是否成功
				if (clink_error_output) printf("There is not enough space for the new linklist.\n");
				// 摧毁1~i-1节点
				if(i>1) {
					cnode *s=list->head, *t;
					for(int j=1; j<i; j++) {
						t=s->link;
						free(s);
						s=t;
					}
				}
				free(list);
				return NULL;
			}
			clink_fillcnode_0(q);
			if(list->head==NULL) list->head=q;
			else list->tail->link = q;// 设置第i-1节点的link
			list->tail = q;// tail移动到当前（第i）节点
			list->tail->link=NULL;// 始终保持尾结点link为0
		}
	}
	else {// 录入数据
		for(int i=1; i<=n; i++) {// 循环创建n个节点（并读取数据）
			if(!(q = (cnode*)malloc(sizeof(cnode)))) {// 创建节点并判断是否成功
				if (clink_error_output) printf("There is not enough space for the new linklist.\n");
				// 摧毁1~i-1节点
				if(i>1) {
					cnode *s=list->head, *t;
					for(int j=1; j<i; j++) {
						t=s->link;
						free(s);
						s=t;
					}
				}
				free(list);
				return NULL;
			}
			if(clink_fillcnode(q)<=0) {// 数据不足or输入的数据类型错误
				if (clink_error_output) printf("Failed to input data when creating the linklist.\n");
				// 摧毁1~i-1节点
				if(i>1) {
					cnode *s=list->head, *t;
					for(int j=1; j<i; j++) {
						t=s->link;
						free(s);
						s=t;
					}
				}
				free(q);
				free(list);
				return NULL;
			}
			if(list->head==NULL) list->head=q;
			else list->tail->link = q;// 设置第i-1节点的link
			list->tail = q;// tail移动到当前（第i）节点
			list->tail->link=NULL;// 始终保持尾结点link为0
		}
	}
	list->size = n;
	list->tail->link=list->head;// 循环链表修正
	return list;
}

// 获取链表list中第k个节点的地址（不成功可视化错误处理并返回NULL）
// 通过clink_getplace()>0 可确定：链表非空 且 k在链表中
cnode* clink_getplace(intclink *list, int k) {
	if(list==NULL) {
		if (clink_error_output) printf("Can't find the target clinklist.\n");
		return NULL;
	}
	cnode* place=list->head;
	if(k>list->size) {
		if (clink_error_output) printf("Can't find the target cnode.(longer than the list's size)\n");
		return NULL;// t超出链表节点总数
	}
	else if(k<=0) {
		if (clink_error_output) printf("Invalid position.(the position should >= 1)\n");
		return NULL;
	}
	for(int i=1; i<k; i++) {
		place = place->link;
	}
	return place;
}

// 向地址为t的节点后添加1个空节点
cnode* clink_addnode_pr(intclink *list, cnode *t) {
	if(list==NULL) {
		if (clink_error_output) printf("Can't find the target clinklist.\n");
		return NULL;
	}
	if(t==NULL) {
		if (clink_error_output) printf("Can't find the target node.\n");
		return NULL;
	}
	cnode *q;
	if(!(q = (cnode*)malloc(sizeof(cnode)))) {// 创建节点并判断是否成功
		if (clink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	clink_fillcnode_0(q);
	list->size++;
	
	cnode *t0=t->link;
	t->link=q;
	q->link=t0;
	if(t==list->tail) {// 在“形式尾结点”后添加
		list->tail=q;
	}
	return q;
}

// 向链表头前添加1个空节点
cnode* clink_addcnode_head(intclink *list) {
	if(list==NULL) {
		if (clink_error_output) printf("Can't find the target clinklist.\n");
		return NULL;
	}
	cnode *q;
	if(!(q = (cnode*)malloc(sizeof(cnode)))) {// 创建节点并判断是否成功
		if (clink_error_output) printf("There is not enough space for the new cnode.\n");
		return NULL;
	}
	clink_fillcnode_0(q);
	list->size++;
	
	if(list->tail==NULL) {// 如果是空链表
		list->head=q;
		list->tail=q;
		q->link=list->head;
	}
	else {
		q->link=list->head;
		list->head=q;
		list->tail->link=q;
	}
	return q;
}

// 向链表尾后添加1个空节点
cnode* clink_addcnode_tail(intclink *list) {
	if(list==NULL) {
		if (clink_error_output) printf("Can't find the target clinklist.\n");
		return NULL;
	}
	cnode *q;
	if(!(q = (cnode*)malloc(sizeof(cnode)))) {// 创建节点并判断是否成功
		if (clink_error_output) printf("There is not enough space for the new cnode.\n");
		return NULL;
	}
	clink_fillcnode_0(q);
	list->size++;
	
	if(list->head==NULL) {// 如果是空链表
		list->head=q;
		list->tail=q;
		q->link=q;
	}
	else {
		list->tail->link=q;
		list->tail=q;
		q->link=list->head;
	}
	return q;
}

// 向链表第k个节点后添加1个空节点（若k=0，视为clink_addcnode_head）（不成功可视化错误处理并返回NULL）
cnode* clink_addcnode(intclink *list, int k) {
	if(list==NULL) {
		if (clink_error_output) printf("Can't find the target clinklist.\n");
		return NULL;
	}
	if(k==0) {
		return clink_addcnode_head(list);
	}
	
	cnode *p=clink_getplace(list, k);// 确认第k节点存在（地址为p）
	if(p==NULL) return NULL;
	// 此时确定：链表非空 且 k在链表中
	
	return clink_addnode_pr(list, p);
}

// 删除链表的第k个节点（如果k超限，可视化错误处理并返回-1；其余情况返回1）
int clink_deletecnode(intclink *list, int k) {
	if(list==NULL) {
		if (clink_error_output) printf("Can't find the target clinklist.\n");
		return -1;
	}
	cnode *t=clink_getplace(list, k);// 确认第k节点存在（地址为t）
	if(t==NULL) {// getplace出错
		return -1;
	}
	// 此时确定：k在链表中
	
	if(list->size==1) {// 链表只有一个节点
		list->size=0;
		free(list->head);
		list->head=NULL;
		list->tail=NULL;
		return 1;
	}
	else if(k==1) {// 节点不止1个，但要删头节点
		list->head=list->head->link;// 更新头结点
		list->size--;
		list->tail->link=list->head;// 更新尾结点link
		free(t);
		return 1;
	}
	else if(k==list->size) {// 节点不止1个，但要删尾节点
		list->tail=clink_getplace(list, k-1);
		list->size--;
		list->tail->link=list->head;
		free(t);
		return 1;
	}
	else {// 要删除中间节点
		cnode *t1=clink_getplace(list, k-1);// 待删除节点的上一节点地址
		list->size--;
		t1->link=t->link;
		free(t);
		return 1;
	}
}

// 删除链表所有节点（包括链表信息）
void clink_destroylist(intclink *list) {
	if(list==NULL) {
		if (clink_error_output) printf("Can't find the target clinklist.\n");
		return;
	}
	else if(list->head==NULL) {// 如果是空链表（此处不用list->size意在适配intlist中删除局部节点
		free(list);
		return;
	}
	else if(list->head==list->tail) {// 只有一个节点
		free(list->head);
	}
	else {// 节点数>=2
		cnode *t=list->head->link, *next;
		cnode *t1=list->head;
		free(list->head);
		while(t!=t1) {
			next=t->link;
			free(t);
			t=next;
		}
	}
	free(list);
	return;
}

// 为链表前k个节点录入数据（k=-1代表能录多少录多少）（返回成功录入的节点个数；不成功可视化错误处理并返回0）
int clink_scanf(intclink *list, int k) {
	if(list==NULL) {
		if (clink_error_output) printf("Can't find the target clinklist.\n");
		return 0;
	}
	if(k>list->size || list->size==0 || k==0) {
		if (clink_error_output) printf("There are not enough cnodes for scanf.\n");
		return 0;
	}
	if(k<=-2) {
		if (clink_error_output) printf("Invalid position.(please use -1 or positive place.)\n");
		return 0;
	}
	// 此时：k不超范围 且 list不为空表
	if(k==-1) {
		int count=0;
		cnode *t=list->head;// t初始化为头节点
		while(1) {
			if(clink_fillcnode(t)<=0) {// 数据量少
				return count;
			}
			else {
				count++;
				if(count==list->size) {// 节点数少
					return list->size;
				}
			}
			t=t->link;
		}
	}
	else {
		cnode *t=list->head;// t初始化为头节点
		int count=0;
		while(k--) {
			if(clink_fillcnode(t)<=0) {// 数据量少
				if (clink_error_output) printf("There are not enough data for scanf.\n");
				return count;
			}
			else {
				count++;
			}
			t=t->link;
		}
		return count;
	}
}

// 向地址为q的节点写入0
void clink_fillcnode_0(cnode *q) {
	q->num=0;// 具体根据节点实际状态填写
	return;
}

// 向地址为q的节点录入数据（返回本节点成功录入数据个数；读取到文件尾时返回-1）
int clink_fillcnode(cnode *q) {
	return scanf("%d", &q->num);// 具体根据节点实际状态填写
}

// 打印全部链表数据（空链表等错误返回-1；其余情况返回1）
int clink_printlist(intclink *list) {
	if(list==NULL) {
		if (clink_error_output) printf("Can't find the target clinklist now.\n");
		return -1;
	}
	cnode *t=list->head;
	int count=0;
	if(t==NULL) return -1;
	printf("Here are contents of the linklist now:\n");
	int t0=list->size;
	while(t0--) {
		printf("%d:%d\n", ++count, t->num);// 具体根据节点实际状态填写
		t=t->link;
	}
	return 1;
}
