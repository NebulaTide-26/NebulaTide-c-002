/*
手动DIY目录：
DIY_node				
DIY_fillnode_0(k)		
DIY_fillnode(k)			
DIY_printnode(k)		（若需调用dlink_printlist时）
DIY_search(k, x)		（若需调用dlink_search时）
DIY_copy				（若需调用dlink_copy时）

说明：
所有涉及intplacecan参数的函数，调用要写PLACE0(x) or PLACE1(x)，代表指针 or 序号
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define DIY_node			int num;
// 用于自定义节点的数据结构

#define DIY_fillnode_0(k)	k->num=0
// 以k作为节点指针；用于填写成员赋值语句

#define DIY_fillnode(k)		scanf("%d", &k->num)
// 以k作为节点指针；要能直接填写在scanf()的()里

#define DIY_printnode(k)	printf("num=%d\n", k->num)
// 以k作为节点指针；要能直接填写在printf()的()里

#define DIY_search(k, x)	k->num==x
// 以k作为节点指针；要能直接填写在if()的()里（注意：仅用于简单的节点数据。对于多数据节点，可能要手动修改dlink_search函数！！！！！！）

#define DIY_copy			t2->num = t1->num;
// 将t1节点的所有数据复制给t2节点

#define PLACE0(p) ((intplace){.pr = (p)})
#define PLACE1(idx) ((intplace){.n = (idx)})
// 自动填写复合字面量

/*
数据结构-线性表-双向链表 函数实现
基本信息类型：intdlink
节点类型：node
若同时建立多个顺序表，需建立多个node变量类型 和 多个基本信息变量，并将所有函数的node分别换成对应词（并设定平行的不同函数名称）
（即：一种基本信息结构体、多种node、多套函数）
*/

static int dlink_error_output = 1;  // 默认打印错误（自定义函数外的static代表
// 链表节点结构（除link部分外可自定义）
typedef struct Node {
	DIY_node
	struct Node *link1;
	struct Node *link2;
} node;

// 基本信息结构体
typedef struct {
	node *head;// 链表首节点地址
	node *tail;// 链表尾节点地址
	int size;// 链表节点个数
} intdlink, *intdlink_p;

// 位置信息联合体（sign=0取地址，sign=1取序号）
// 所有涉及intplace的函数，调用要写PLACE0(x) or PLACE1(x)，代表指针 or 序号
typedef union {
	node *pr;
	int n;
} intplace;

// 1.链表结构控制
intdlink* dlink_intlist(int n, int sign);
node* dlink_addnode_head(intdlink *list);
node* dlink_addnode_tail(intdlink *list);
node* dlink_addnode(intdlink *list, int target, intplace x, int sign);
int dlink_deletenode(intdlink *list, int target, intplace x, int sign);
int dlink_deletelist(intdlink *list);
intdlink* dlink_append(intdlink* list1, intdlink* list2);
intdlink* dlink_copy(intdlink* list1);

// 2.节点位置信息获取&转换
int dlink_checkplace(intdlink *list, intplace x, int sign);
node* dlink_search(intdlink* list, int x);
int dlink_getplace_pr(intdlink *list, node *p);
node* dlink_getplace_n(intdlink *list, int n);

// 3.节点信息录入&输出
void dlink_fillnode_0(intdlink *list, intplace x, int sign);
int dlink_fillnode(intdlink *list, intplace x, int sign);
int dlink_printlist(intdlink *list);

// 4.其他（供其他函数调用的基础功能函数等）
void dlink_set_error_output(int enable);
static void dlink_basic_fillnode_0(node *q);
static int dlink_basic_fillnode(node *q);

int main() {
	
    
    
    
    return 0;
}

// 自定义函数

// 1.链表结构控制

// 创建链表
// 返回值：链表信息结构体指针（失败可视化错误处理并返回NULL）
intdlink* dlink_intlist(int n, int sign_fill) {// sign_fill=1录入数据，sign_fill=0不录入数据
	if(n<0) {
		if (dlink_error_output) printf("The number of nodes should >= 0.\n");
		return NULL;
	} else if(sign_fill!=1 && sign_fill!=0) {
		if (dlink_error_output) printf("The sign to set data should be 0 or 1.\n");
		return NULL;
	}
	intdlink *list;
	if(!(list=(intdlink*)malloc(sizeof(intdlink)))) {// 创建链表信息并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new dlinklist.\n");
		return NULL;
	}
	list->head=NULL;
	list->size=0;
	list->tail=NULL;

	node *q;

	if(n==0) {// 如果要创建空链表
		return list;
	}
	
	for(int i=1; i<=n; i++) {// 循环创建n个节点
		if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
			if (dlink_error_output) printf("There is not enough space for the new dlinklist.\n");
			dlink_deletelist(list);// 摧毁1~i-1节点
			return NULL;
		}
		switch(sign_fill) {
			case 0:// 写入0
				dlink_basic_fillnode_0(q);
				break;
			case 1:// 读取数据
				if(dlink_basic_fillnode(q)<=0) {// 数据不足or输入的数据类型错误
					if (dlink_error_output) printf("Failed to input data when creating the dlinklist.\n");
					dlink_deletelist(list);// 摧毁1~i-1节点
					free(q);
					return NULL;
				}
				break;
		}
		if(list->head==NULL) {// 首个节点
			list->head=q;
			q->link1=NULL;
		}
		else {
			q->link1 = list->tail;
			list->tail->link2 = q;// 设置第i-1节点的link2
		}
		list->tail = q;// tail移动到当前（第i）节点
		list->tail->link2=NULL;// 始终保持新结点link2为0
	}
	list->size = n;
	return list;
}

// 向链表头前添加1个空节点
node* dlink_addnode_head(intdlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	dlink_basic_fillnode_0(q);
	q->link1 = NULL;
	list->size++;
	
	if(list->tail==NULL) {//如果是空链表
		list->head = q;
		list->tail = q;
		q->link2 = NULL;
	} else {
		q->link2 = list->head;
		list->head->link1 = q;
		list->head = q;
	}
	return q;
}

// 向链表尾后添加1个空节点
node* dlink_addnode_tail(intdlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	dlink_basic_fillnode_0(q);
	q->link2 = NULL;
	list->size++;
	
	if(list->tail==NULL) {//如果是空链表
		list->head = q;
		list->tail = q;
		q->link1 = NULL;
	} else {
		q->link1 = list->tail;
		list->tail->link2 = q;
		list->tail = q;
	}
	return q;
}

// 向intplace=x的节点的前or后添加1个空节点
node* dlink_addnode(intdlink *list, int target, intplace x, int sign) {// target：-1=前，1=后
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist.\n");
		return NULL;
	}
	if(target!=-1 && target!=1) {
		if (dlink_error_output) printf("The \"target\" used to determine direction should be -1 or 1.\n");
		return NULL;
	}
	if(dlink_checkplace(list, x, sign)==-1) {
		return NULL;
	}
	if(sign) {// 统一翻译成x.pr
		x.pr = dlink_getplace_n(list, x.n);
	}
	switch(target) {// 针对头尾提前单独处理
		case -1:
			if(x.pr==list->head) return dlink_addnode_head(list);
			break;
		case 1:
			if(x.pr==list->tail) return dlink_addnode_tail(list);
			break;
	}
	// 此时：链表size一定>=2，且新节点不插在头尾
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	dlink_basic_fillnode_0(q);
	list->size++;
	switch(target) {
		case -1:// 节点前增加
			q->link2 = x.pr;
			q->link1 = x.pr->link1;
			x.pr->link1->link2 = q;
			x.pr->link1 = q;						
			break;
		case 1:// 节点后增加
			q->link1 = x.pr;
			q->link2 = x.pr->link2;
			x.pr->link2->link1 = q;
			x.pr->link2 = q;						
			break;
	}
	return q;
}

// 删除intplace=x的节点 or 其前/后节点
// 返回值：1（失败返回-1）
int dlink_deletenode(intdlink *list, int target, intplace x, int sign) {// target：-1=前，0=x，1=后
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist.\n");
		return -1;
	}
	if(target!=-1 && target!=0 && target!=1) {
		if (dlink_error_output) printf("The \"target\" used to determine direction should be -1 , 0 or 1.\n");
		return -1;
	}
	if(dlink_checkplace(list, x, sign)==-1) {// 判断位置是否合法
		return -1;
	}
	if(sign) {// 统一翻译成x.pr
		x.pr = dlink_getplace_n(list, x.n);
	}
	switch(target) {
		case 0:// 删除当前节点
			if(list->size==1) {// 全链表仅一个节点
				free(list->head);
				list->head = NULL;
				list->tail = NULL;
				list->size = 0;
				break;
			}
			if(x.pr==list->head) {// 删除头结点
				node *t = list->head;
				list->head = list->head->link2;
				list->head->link1 = NULL;
				free(t);
			} else if(x.pr==list->tail) {// 删除尾结点
				node *t = list->tail;
				list->tail = list->tail->link1;
				list->tail->link2 = NULL;
				free(t);
			}
			else {
				x.pr->link1->link2 = x.pr->link2;
				x.pr->link2->link1 = x.pr->link1;
				free(x.pr);
			}
			list->size--;
			break;
		case -1:// 删除前一节点
			if(x.pr==list->head) {
				if (dlink_error_output) printf("Cannot delete the preceding node.(already at boundary)\n");
				return -1;
			}
			return dlink_deletenode(list, 0, PLACE0(x.pr->link1), 0);
		case 1:// 删除后一节点
			if(x.pr==list->tail) {
				if (dlink_error_output) printf("Cannot delete the following node.(already at boundary)\n");
				return -1;
			}
			return dlink_deletenode(list, 0, PLACE0(x.pr->link2), 0);
	}
	return 1;
}

// 删除全部节点，删除链表信息
// 返回值：1（失败返回-1）
int dlink_deletelist(intdlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist.\n");
		return -1;
	}
	for(node *t=list->head, *p; t!=NULL;) {
		p = t->link2;
		free(t);
		t = p;
	}
	free(list);
	return 1;
}

// 将list2追加到list1后（需保证node类型相同；将删除list2，如需要可提前调用dlink_copy(list2)）
// 返回值：list1的链表信息结构体指针（失败返回NULL）
intdlink* dlink_append(intdlink* list1, intdlink* list2) {
	if(list1==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist 1 now.\n");
		return NULL;
	} else if(list2==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist 2 now.\n");
		return NULL;
	} else if(list2==list1) {
		if (dlink_error_output) printf("Can't append a list to itself.\n");
		return NULL;
	}
	// 单独处理空表问题
	if(list1->size==0 && list2->size==0) {
		free(list2);
		return list1;
	} else if(list1->size==0) {
		list1->head = list2->head;
		list1->size = list2->size;
		list1->tail = list2->tail;
		free(list2);
		return list1;
	} else if(list2->size==0) {
		free(list2);
		return list1;
	}
	
	list1->tail->link2 = list2->head;
	list2->head->link1 = list1->tail;
	list1->tail = list2->tail;
	list1->size += list2->size;
	free(list2);
	return list1;
}

// 复制list1
intdlink* dlink_copy(intdlink* list1) {
	if(list1==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist now.\n");
		return NULL;
	}
	intdlink *list2 = dlink_intlist(list1->size, 0);
	if(list1->size==0) {
		return list2;
	}
	for(node *t1=list1->head, *t2=list2->head; t1!=NULL; t1=t1->link2, t2=t2->link2) {
		DIY_copy
	}
	return list2;
}


// 2.节点位置信息获取&转换

// 可视化检查intplace=x的位置信息是否合法
// 返回值：1（位置非法返回-1）
// 特别注意：为避免每次调用checkplace都遍历链表，此处不检查地址是否一定在链表中！！！	如有需要，可单独检验dlink_getplace_pr(list, x.pr)!=-1
// 返回1时确保：链表存在且非空，intplace在链表里（地址除外）
int dlink_checkplace(intdlink *list, intplace x, int sign) {
	if(sign!=0 && sign!=1) {
		if (dlink_error_output) printf("The sign to translate \"intplace\" should be 0 or 1.\n");
		return -1;
	}
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist now.\n");
		return -1;
	}
	if(list->size==0) {
		if (dlink_error_output) printf("The target dlinklist is empty.\n");
		return -1;
	}
	
	switch(sign) {
		case 0:{// 地址
			node *pr = x.pr;
			if(pr==NULL) {
				if (dlink_error_output) printf("Invalid position.(the position should not be NULL)\n");
				return -1;
			}
			return 1;
			break;
		}
		case 1:{// 序号
			int n = x.n;
			if(n<=0) {
				if (dlink_error_output) printf("Invalid position.(the node number should >= 1)\n");
				return -1;
			}
			if(n>list->size) {
				if (dlink_error_output) printf("Can't find the target node.(longer than the list's size)\n");
				return -1;
			}
			return 1;
			break;
		}
	}
	return -1;
}

// 查找数据（从head开始，查找第一个特定成员值为x的节点）
// 返回值：找到返回相应节点指针（找不到返回NULL）
// tips：大多数场景下，需要将该函数放入main中，并用变量形式填写DIY_search(t, x)；本函数仅作为格式模板
node* dlink_search(intdlink* list, int x) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist now.\n");
		return NULL;
	}
	if(list->size==0) {
		return NULL;
	}
	for(node *t=list->head; t!=NULL; t = t->link2) {
		if(DIY_search(t, x)) {// 根据所查找成员&节点结构DIY
			return t;
		}
	}
	return NULL;
}

// 转换：节点地址->节点序号
// 返回值：节点序号（失败返回-1）
int dlink_getplace_pr(intdlink *list, node *p) {
	if(dlink_checkplace(list, PLACE0(p), 0)==-1) {
		return -1;
	}
	int count=1;
	for(node *t=list->head; t!=NULL; t=t->link2, count++) {
		if(t==p) {
			return count;
		}
	}
	if (dlink_error_output) printf("Invalid position.(the position isn't in the list)\n");
	return -1;
}

// 转换：节点序号->节点地址
// 返回值：节点地址（失败返回NULL）
node* dlink_getplace_n(intdlink *list, int n) {
	if(dlink_checkplace(list, PLACE1(n), 1)==-1) {
		return NULL;
	}
	node *t=list->head;
	n--;
	while(n--) {
		t = t->link2;
	}
	return t;
}


// 3.节点信息录入&输出

// 向intplace=x的节点写入0
void dlink_fillnode_0(intdlink *list, intplace x, int sign) {
	if(dlink_checkplace(list, x, sign)==-1) {// 判断位置是否合法
		return;
	}
	if(sign) {// 统一翻译成x.pr
		x.pr = dlink_getplace_n(list, x.n);
	}
	dlink_basic_fillnode_0(x.pr);
	return;
}

// 向intplace=x的节点录入数据
// 返回值：本节点成功录入数据个数（读取到文件尾时返回-1）
int dlink_fillnode(intdlink *list, intplace x, int sign) {
	if(dlink_checkplace(list, x, sign)==-1) {// 判断位置是否合法
		return -1;
	}
	if(sign) {// 统一翻译成x.pr
		x.pr = dlink_getplace_n(list, x.n);
	}
	return dlink_basic_fillnode(x.pr);
}

// 打印全部链表数据
// 返回值：1（空链表等错误返回-1）
int dlink_printlist(intdlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist now.\n");
		return -1;
	}
	node *t=list->head;
	int count=0;
	if(t==NULL) {
		if (dlink_error_output) printf("The dlinklist is empty.\n");
		return -1;
	}
	printf("Here are contents of the dlinklist now:\n");
	while(t!=NULL) {
		printf("%d: ", ++count);
		DIY_printnode(t);// 具体根据节点实际状态填写DIY宏
		t = t->link2;
	}
	return 1;
}


// 4.其他（供其他函数调用的基础功能函数等）

// 控制是否可视化打印错误信息
void dlink_set_error_output(int enable) {// 1→打印；0→不打印
	dlink_error_output = enable ? 1 : 0;
	return;
}

// 向某地址为q的节点写入0
static void dlink_basic_fillnode_0(node *q) {
	if(q==NULL) {
		if (dlink_error_output) printf("Can't find the target node.\n");
		return;
	}
	DIY_fillnode_0(q);// 具体根据节点实际状态填写DIY宏
	return;
}

// 向某地址为q的节点录入数据
// 返回值：本节点成功录入数据个数（读取到文件尾时返回-1）
static int dlink_basic_fillnode(node *q) {
	if(q==NULL) {
		if (dlink_error_output) printf("Can't find the target node.\n");
		return -1;
	}
	return DIY_fillnode(q);// 具体根据节点实际状态填写DIY宏
}



// 依照某成员值对链表节点排序
intdlink* dlink_sort(intdlink* list) {
	// 待完善............（思路：归并排序）
	
	
	return list;
}




