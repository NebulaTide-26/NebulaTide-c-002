/* 数据结构-树-普通树（三重链表结构） 函数实现
基本信息类型：inttree
节点类型：node
若同时建立多个树，需建立多个node变量类型 和 多个基本信息变量，并将所有函数的node分别换成对应词（并设定平行的不同函数名称）
（即：一种基本信息结构体、多种node、多套函数）
*/

/* 手动DIY目录：
DIY_data
DIY_fillnode_0(k)		
DIY_fillnode(k)			
DIY_search(k, x)		（若需调用tree_search时）
*/

/* 错误说明：
[Invalid function argument]	非法函数参数
[Memory exceeded]			MLE
[Input error]				读取输入失败
[Illegal operation]			非法操作
[Other errors]				其他（空树）
*/



#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define DIY_data				int num
// 用于自定义节点的数据结构

#define DIY_fillnode_0(k)		k->num = 0
// 以k作为节点指针；用于填写成员赋值语句

#define DIY_fillnode(k)			scanf("%d", &k->num)
// 以k作为节点指针；不带“;”的scanf()语句

//#define DIY_printnode(k)		??????（暂时用不到）
// 以k作为节点指针；不带“;”的printf()语句

#define DIY_search(k, x)		k->num==x
// 以k作为节点指针；要能直接填写在if()的()里（注意：仅用于简单的节点数据。对于多数据节点，可能要手动修改tree_search函数！！！！！！）



// 树节点结构（具体根据需要填写DIY宏）
typedef struct Node {
	DIY_data;
	struct Node *child;
	struct Node *parent;
	struct Node *brother;
} node;

// 基本信息结构体
typedef struct {
	node *root;						// 根节点
	int size;						// 树节点个数
} inttree;

// 层序遍历结构体
typedef struct Node_level{
	node *root;						// 根节点地址
	node **traverse;				// 存储层序遍历顺序的数组（队列）
	struct Node_level *link1;
	struct Node_level *link2;
} node_level;

// 层序遍历总目录结构体（采用链表结构）
typedef struct {
	node_level *head;
	node_level *tail;
	int count;
} intlevel;

// 全局变量level：层序遍历总目录
static intlevel _level = {NULL, NULL, 0};
#define level (&_level)

// 全局变量tree_error_output：控制是否打印错误信息（默认打印）
static int tree_error_output = 1;



// 1.树结构控制
inttree* tree_inttree();										// 创建树（仅有根节点，data设为0）
node* tree_addnode_root(inttree *tree);							// 为空树tree加入一个空根节点
node* tree_addnode(inttree *tree, int target, node *p);			// 为节点p添加一个空child or brother节点
int tree_deletenode(inttree *tree, int target, node *p);		// 节点删除函数
int tree_deletetree(inttree *tree);								// 删除全部节点，删除树信息
inttree* tree_copy(inttree* tree1);								// 复制tree1

// 2.节点位置、树相关信息获取
int tree_checkplace(inttree *tree, node *p);					// 可视化检查节点指针p是否合法
node* tree_traverse_pre(inttree *tree, node *p);				// 先根遍历（根据当前接收的节点，输出下一个节点地址）
node* tree_traverse_post(inttree *tree, node *p);				// 后根遍历（根据当前接收的节点，输出下一个节点地址）
node* tree_traverse_post_start(inttree *tree, node *p);				// （后跟遍历辅助函数）定位到以p为根节点的子树的左下角
node* tree_traverse_level(inttree *tree, node *p);				// 层序遍历（根据当前接收的节点，输出下一个节点地址）
int tree_traverse_clean(inttree *tree);							// （层序遍历辅助函数）强制清除tree的层序遍历资源（若tree=NULL，全部清除）
node* tree_search(inttree *tree, int x);						// 查找数据
int tree_countdeep_node(inttree *tree, node *p);				// 计算节点p的深度
int tree_countdeep(inttree *tree);								// 计算整个tree的深度

// 3.节点信息录入&输出
void tree_fillnode_0(inttree *tree, node *p);					// 向节点p写入0
int tree_fillnode(inttree *tree, node *p);						// 向节点p录入数据

//int tree_printtree(inttree *tree);								// 打印一整棵树（填写DIY_printnode时尽量只取某一数字成员）（复杂且不急用，待完善）

// 4.其他（供其他函数调用的basic函数等）
void tree_set_error_output(int enable);							// 控制是否可视化打印错误信息
static void tree_basic_fillnode_0(node *p);						// （basic）向节点p写入0
static int tree_basic_fillnode(node *p);						// （basic）向节点p录入数据
static node* tree_basic_copynode(node *dest, node *src);		// （basic）将节点src的数据复制给节点dest
static node* tree_basic_copy(node *p, node *parent);			// （basic）递归复制节点p及其所有子树
static void tree_basic_deletenode(node *p);						// （basic）删除节点p的全部子树、brother（不调整任何tree的size）
static void tree_basic_traverse_level_deletenode(node_level *t);// （basic）删除层序遍历总目录中地址位t的项（及其内部引出的traverse数组）



int main() {
	
    
    
    return 0;
}



// 自定义函数

// 1.树结构控制
/* 创建树（仅有根节点，其data设为0）
   返回值：树信息结构体指针（失败返回NULL）
*/
inttree* tree_inttree() {
	// 创建树的信息并判断是否成功
	inttree *tree = (inttree*)malloc(sizeof(inttree));
	if(!tree) {
		if (tree_error_output) printf("[Memory exceeded] There is not enough space for the new tree.\n");
		return NULL;
	}
	node *root = (node*)malloc(sizeof(node));
	if(!root) {
		if (tree_error_output) printf("[Memory exceeded] There is not enough space for the new tree.\n");
		free(tree);
		return NULL;
	}
	root->brother = NULL;
	root->child = NULL;
	root->parent = NULL;
	tree->size = 1;
	tree->root = root;
	tree_fillnode_0(tree, root);
	return tree;
}

/* 为空树tree加入一个空根节点
   返回值：新节点地址（失败返回NULL）
   
   注：由于下一函数tree_addnode必须基于某个现有节点，该函数是对其的补充
*/
node* tree_addnode_root(inttree *tree) {
	if(tree==NULL) {
		if(tree_error_output) printf("[Invalid function argument] The target tree pointer is NULL.\n");
		return NULL;
	}
	if(tree->root!=NULL) {
		if(tree_error_output) printf("[Illegal operation] The target tree already has a root.\n");
		return NULL;
	}
	node *t = (node*)malloc(sizeof(node));
	if(!t) {
		if(tree_error_output) printf("[Memory exceeded] There is not enough space for the new node.\n");
		return NULL;
	}
	t->brother = NULL;
	t->child = NULL;
	t->parent = NULL;
	tree_basic_fillnode_0(t);
	tree->root = t;
	tree->size = 1;
	return t;
}

/* 为节点p添加一个空child or brother节点
   返回值：新节点地址（失败返回NULL）
   
   函数行为约定：添加brother默认加在紧邻p处，添加child默认加在其所有child之前
   target：-1=child，1=brother
*/
node* tree_addnode(inttree *tree, int target, node *p) {
	
	if(tree==NULL) {
		if (tree_error_output) printf("[Invalid function argument] The target tree pointer is NULL.\n");
		return NULL;
	}
	if(target!=-1&&target!=1) {
		if (tree_error_output) printf("[Invalid function argument] The \"target\" used to determine direction should be -1 or 1.\n");
		return NULL;
	}
	if(tree_checkplace(tree, p)==-1) {
		return NULL;
	}
	if(target==1 && p==tree->root) {
		if (tree_error_output) printf("[Illegal operation] Cannot add brother to the root node.\n");
		return NULL;
	}
	
	node *t = (node*)malloc(sizeof(node));
	if(t==NULL) {
		if (tree_error_output) printf("[Memory exceeded] There is not enough space for the new node.\n");
		return NULL;
	}
	tree_basic_fillnode_0(t);
	t->brother = NULL;
	t->child = NULL;
	t->parent = NULL;	
	tree->size++;
	switch(target) {
		case -1:// 添加为child
			if(p->child==NULL) {
				p->child = t;
				t->parent = p;
			} else {
				t->brother = p->child;
				p->child = t;
				t->parent = p;
			}
			break;
		case 1:// 添加为brother
			t->brother = p->brother;
			p->brother = t;
			t->parent = p->parent;
			break;
	}
	return t;
}

/* 节点删除函数
   返回值：1（失败返回-1）
   
   target：-1=删除p的所有子树，0=删除p及其所有子树（若有）同时保留brother，1=删除单个右brother及其所有子树
*/
int tree_deletenode(inttree *tree, int target, node *p) {
	if(tree_checkplace(tree, p)==-1) {
		return -1;
	}
	if(target!=-1 && target!=0 && target!=1) {
		if(tree_error_output) printf("[Invalid function argument] The \"target\" used to select option should be -1 , 0 or 1.\n");
		return -1;
	}
	
	switch(target) {
		case -1:{
			if(p->child==NULL) {
				if(tree_error_output) printf("[Illegal operation] The node p doesn't have any child.\n");
				return -1;
			}
			node *p0 = p;// 记录子树根节点地址
			p = tree_traverse_post_start(tree, p);// 定位到子树的左下角
			node *t = p;// 配合free函数
			while(p!=p0) {
				t = p;
				p = tree_traverse_post(tree, t);// 后根遍历找到下一节点
				free(t);
				tree->size--;
			}
			p->child = NULL;
			break;
		}
		case 0:{
			node *p0 = p;// 记录子树根节点地址
			p = tree_traverse_post_start(tree, p);// 定位到子树的左下角
			node *t = p;// 配合free函数
			while(p!=p0) {
				t = p;
				p = tree_traverse_post(tree, t);// 后根遍历找到下一节点
				free(t);
				tree->size--;
			}
			// 处理对p本身的删除
			tree->size--;
			if(p==tree->root) {// 删成空树
				tree->root = NULL;
				free(p);
			} else if(p->parent->child==p) {// p是最大brother
				if(p->brother==NULL) {
					p->parent->child = NULL;
					free(p);
				} else {
					p->parent->child = p->brother;
					free(p);
				}
			} else {
				t = p->parent->child;
				while(t->brother!=p) {// t定位到p的哥哥节点
					t = t->brother;
				}
				t->brother = p->brother;
				free(p);
			}
			break;
		}
		case 1:{
			if(p->brother==NULL) {
				if(tree_error_output) printf("[Illegal operation] The node p doesn't have brother.\n");
				return -1;
			}
			tree_deletenode(tree, 0, p->brother);
			break;
		}
	}
	return 1;
}

/* 删除全部节点，删除树信息
   返回值：1（失败返回-1）
*/
int tree_deletetree(inttree *tree) {
	if(tree==NULL) {
		if (tree_error_output) printf("[Invalid function argument] The target tree pointer is NULL.\n");
		return -1;
	}
	// 处理层序遍历目录
	node_level *t = level->head;
	while(t) {
		if(tree->root==t->root) {// 当前树已在层序遍历中
			tree_basic_traverse_level_deletenode(t);
			break;
		}
		t = t->link2;
	}
	// 删除树
	if(tree->size>0) {
		tree_deletenode(tree, 0, tree->root);
	}
	free(tree);
	return 1;
}

/* 复制tree1
   返回值：复制出来的树的信息结构体指针（失败返回NULL）
*/
inttree* tree_copy(inttree* tree1) {
	if(tree1==NULL) {
		if (tree_error_output) printf("[Invalid function argument] The target tree pointer is NULL.\n");
		return NULL;
	}
	inttree *tree2 = (inttree*)malloc(sizeof(inttree));
	if(!tree2) {
		if (tree_error_output) printf("[Memory exceeded] There is not enough space for the new tree.\n");
		return NULL;
	}
	tree2->root = NULL;
	tree2->size = tree1->size;
	tree2->root = tree_basic_copy(tree1->root, NULL);// 调用basic函数对每个节点（及其子树）进行递归复制
	if(tree1->root!=NULL && tree2->root==NULL) {// 说明复制失败，malloc时空间不够
		free(tree2);
		return NULL;
	}
	return tree2;
}



// 2.节点位置、树相关信息获取
/* 可视化检查节点指针p是否合法
   返回值：1（位置非法返回-1）
   
   函数行为约定：
   仅进行简单O(1)级别检查，不检查地址是否一定在树中（若需检查，可手动回溯parent链与tree->root对比）
   返回1时确保：树存在且size≥1，指针非NULL
*/
int tree_checkplace(inttree *tree, node *p) {
	if(tree==NULL) {
		if (tree_error_output) printf("[Invalid function argument] The target tree pointer is NULL.\n");
		return -1;
	}
	if(p==NULL) {
		if (tree_error_output) printf("[Invalid function argument] The position should not be NULL.\n");
		return -1;
	}
	if(tree->size==0) {
		if (tree_error_output) printf("[Other errors] The target tree is empty.\n");
		return -1;
	}
	return 1;
}

/* 先根遍历（根据当前接收的节点，输出下一个节点地址）
   返回值：下一个节点地址（错误返回NULL）
*/
node* tree_traverse_pre(inttree *tree, node *p) {
	if(tree_checkplace(tree, p)==-1) {
		return NULL;
	}
	if(p->child!=NULL) return p->child;
	// 从p往上找第一个有brother的祖上节点（含p本身）
	while(p!=NULL) {
		if(p->brother!=NULL) return p->brother;
		p = p->parent;
	}
	// 若还没return：p一直向上找到了root节点，递归结束
	return NULL;
}

/* 后根遍历（根据当前接收的节点，输出下一个节点地址）
   返回值：下一个节点地址（错误 or 遍历结束返回NULL）
   
   注意：由于不是从根节点开始遍历，完整遍历时需要先手动（调用tree_traverse_post_start）定位到最左下方的节点处
*/
node* tree_traverse_post(inttree *tree, node *p) {
	if(tree_checkplace(tree, p)==-1) {
		return NULL;
	}
	if(p->brother==NULL) {
		if(p->parent==NULL) return NULL;// 已经遍历到root节点，递归结束
		return p->parent;
	}
	// 定位到p的brother子树的最左下角
	return tree_traverse_post_start(tree, p->brother);
}

/* （后跟遍历辅助函数）定位到tree中以p为根节点的子树的左下角
   返回值：左下角节点地址（错误返回NULL）
*/
node* tree_traverse_post_start(inttree *tree, node *p) {
	if(tree_checkplace(tree, p)==-1) {
		return NULL;
	}
	while(p->child!=NULL) {
		p = p->child;
	}
	return p;
}

/* 层序遍历（根据当前接收的节点，输出下一个节点地址）
   返回值：下一个节点地址（错误 or 遍历结束返回NULL）
*/
node* tree_traverse_level(inttree *tree, node *p) {
	if(tree_checkplace(tree, p)==-1) {
		return NULL;
	}
	node_level *t = NULL;
	for(t = level->head; t!=NULL; t = t->link2) {
		if(tree->root==t->root) {// 当前树已在层序遍历中
			break;// 此时t即为相应层序遍历结构体
		}
	}
	if(t!=NULL) {// 当前树已在层序遍历中
		for(int i=0; i<tree->size-1; i++) {// “i<tree->size-1”：p不是最后一个节点
				if(t->traverse[i]==p) {
					return t->traverse[i+1];
				}
			}
		// 此时必应当遍历结束，需注销相应的层序遍历结构体(t)
		tree_basic_traverse_level_deletenode(t);
		return NULL;
	} else {
		t = (node_level*)malloc(sizeof(node_level));
		if(!t) {
			if (tree_error_output) printf("[Memory exceeded] There are not enough space to do the level-order traverse.\n");
			return NULL;
		}
		// 构建层序遍历序列
		t->root = tree->root;
		t->traverse = (node**)malloc(sizeof(node*)*tree->size);// 层序遍历数组（只进不出的队列）
		if(!t->traverse) {
			if (tree_error_output) printf("[Memory exceeded] There are not enough space to do the level-order traverse.\n");
			free(t);
			return NULL;
		}
		t->traverse[0] = tree->root;
		for(int k1 = 0, k2 = 1; k1<tree->size; k1++) {// k1k2：记录（所谓）队列出、入队位置
			if(t->traverse[k1]->child!=NULL) {// traverse[k1]有child：将所有child入队
				for(node *t0 = t->traverse[k1]->child; t0!=NULL; t0 = t0->brother) {
					t->traverse[k2++] = t0;
				}
			}
		}
		
		// 添加新节点
		if(level->count==0) {// 添加第一个结点
			level->head = t;
			level->tail = t;
			t->link1 = NULL;
			t->link2 = NULL;
			level->count++;
		} else {
			t->link1 = level->tail;
			t->link2 = NULL;
			level->tail->link2 = t;
			level->tail = t;
			level->count++;
		}
		
		// 找到该输出哪个节点
		for(int i=0; i<tree->size-1; i++) {// “i<tree->size-1”：p不是最后一个节点
			if(t->traverse[i]==p) {
				return t->traverse[i+1];
			}
		}
		// 此时必应当遍历结束，需注销相应的层序遍历结构体(t)
		tree_basic_traverse_level_deletenode(t);
		return NULL;
	}
}

/* （层序遍历辅助函数）强制清除tree的层序遍历资源（若tree=NULL，全部清除）
   返回值：所清除的层序遍历目录条数（即从全局链表中删除了几个node_level节点）（错误返回-1）s
*/
int tree_traverse_clean(inttree *tree) {
	if(!tree) {// 全部清除
		int temp = level->count;
		while(level->count>0) {
			tree_basic_traverse_level_deletenode(level->head);
		}
		return temp;
	} else {
		for(node_level *t = level->head; t!=NULL; t = t->link2) {
			if(t->root==tree->root) {
				tree_basic_traverse_level_deletenode(t);
				return 1;
			}
		}
		return -1;
	}
}

/* 查找数据
   返回值：目标节点指针（找不到返回NULL）
   
   tips：本函数作为格式模板，仅设定一个搜索参量x，实际可手动添加
*/
node* tree_search(inttree *tree, int x) {
	if(tree==NULL) {
		if (tree_error_output) printf("[Invalid function argument] The target tree pointer is NULL.\n");
		return NULL;
	}
	if(tree->size==0) return NULL;
	node *t = tree->root;
    do {
        if (DIY_search(t, x)) {// 根据所查找成员&节点结构DIY
        	return t;
		}
        t = tree_traverse_pre(tree, t);
    } while (t != NULL);
    return NULL;
}

/* 计算节点p的深度
   返回值：节点p深度（错误返回-1）（根深度为1）
*/
int tree_countdeep_node(inttree *tree, node *p) {
	if(tree_checkplace(tree, p)==-1) {
		return -1;
	}
	int count = 0;
	while(p!=NULL) {
		p = p->parent;
		count++;
	}
	return count;
}

/* 计算整个tree的深度
   返回值：整个tree的深度（错误返回-1）
   
   函数行为约定：根深度为1；空树返回0
*/
int tree_countdeep(inttree *tree) {
	if(tree==NULL) {
		if (tree_error_output) printf("[Invalid function argument] The target tree pointer is NULL.\n");
		return -1;
	}
	int deep = 0;
	for(node *t = tree->root; t!=NULL;) {
		if(t->child==NULL) {// 如果是叶节点
			int temp = tree_countdeep_node(tree, t);
			deep = deep<temp? temp : deep;// 取max
		}
		t = tree_traverse_pre(tree, t);
	}
	return deep;
}



// 3.节点信息录入&输出
/* 向节点p写入0
*/
void tree_fillnode_0(inttree *tree, node *p) {
	if(tree_checkplace(tree, p)==-1) {
		return;
	}
	tree_basic_fillnode_0(p);
	return;
}

/* 向节点p录入数据
   返回值：本节点成功录入数据个数（读取到文件尾or错误时返回-1）
*/
int tree_fillnode(inttree *tree, node *p) {
	if(tree_checkplace(tree, p)==-1) {
		return -1;
	}
	int t;
	if((t = tree_basic_fillnode(p))<=0) {// 数据不足or输入的数据类型错误
		if (tree_error_output) printf("[Input error] Failed to input data.\n");
		return -1;
	}
	return t;
}

///* 打印一整棵树（填写DIY_printnode时尽量只取某一数字成员）（待完善）
//   返回值：1（空树等错误返回-1）
//*/
//int tree_printtree(inttree *tree) {
//	if(tree==NULL) {
//		if (tree_error_output) printf("[Invalid function argument] The target tree pointer is NULL.\n");
//		return -1;
//	}
//	// 待完善......
//	return 1;
//}



// 4.其他（供其他函数调用的basic函数等）
// 注：basic函数不做可靠的错误检查，仅做部分错误反馈，尽量不要手动调用
/* 控制是否可视化打印错误信息
*/
void tree_set_error_output(int enable) {// 1→打印；0→不打印
	tree_error_output = enable ? 1 : 0;
	return;
}

/* （basic）向节点p写入0
   用途：无需填写tree指针的节点归零
*/
static void tree_basic_fillnode_0(node *p) {
	DIY_fillnode_0(p);// 具体根据节点实际结构填写DIY宏
}

/* （basic）向节点p录入数据
   返回值：本节点成功录入数据个数（读取到文件尾时返回-1）
   用途：无需填写tree指针的节点数据录入
*/
static int tree_basic_fillnode(node *p) {
	return DIY_fillnode(p);// 具体根据节点实际结构填写DIY宏
}

/* （basic）将节点src的数据复制给节点dest
   返回值：dest节点地址
   用途：配合tree_copy函数，取代DIY_copy宏
   
   注意：为浅拷贝，若data区存在指针，可能会导致double-free，调用时小心
*/
static node* tree_basic_copynode(node *dest, node *src) {
	node *parent = dest->parent;
	node *child = dest->child;
	node *brother = dest->brother;
	*dest = *src;
	dest->brother = brother;
	dest->child = child;
	dest->parent = parent;
	return dest;
}

/* （basic）递归复制节点p及其所有子树
   返回值：复制出来的节点p的对应节点（失败返回NULL）
   用途：配合tree_copy函数
   
   注意：若节点p为NULL，返回值也为NULL，请注意与malloc失败的区分！
*/
static node* tree_basic_copy(node *p, node *parent) {// 复制信息源的节点p地址，新树中对应节点的->parent地址
	if(p==NULL) return NULL;
	node *t = (node*)malloc(sizeof(node));
	if(!t) {
		if (tree_error_output) printf("[Memory exceeded] There is not enough space for the new tree.\n");
		return NULL;
	}
	
	t->brother = NULL;
	t->child = NULL;
	t->parent = parent;
	tree_basic_copynode(t, p);
	if(p->child!=NULL) {
		t->child = tree_basic_copy(p->child, t);
		if(t->child==NULL) {// 说明复制失败，malloc时空间不够
			free(t);
			return NULL;
		}
	}
	if(p->brother!=NULL) {
		t->brother = tree_basic_copy(p->brother, parent);
		if(t->brother==NULL) {// 说明复制失败，malloc时空间不够
			tree_basic_deletenode(t);// 回滚掉可能已经复制出的child子树
			free(t);
			return NULL;
		}
	}
	return t;
}

/* （basic）删除节点p的全部子树、brother（不调整任何tree的size）
   用途：配合tree_copy函数，当内存不足时对已复制节点进行回滚
*/
static void tree_basic_deletenode(node *p) {
	node *p0 = p;// 记录子树根节点地址
	while(p->child!=NULL) {// 定位到子树的左下角
		p = p->child;
	}
	node *t = p;// 配合free函数
	while(p!=p0) {// 后跟遍历删除p的全部子树
		t = p;
		if(p->brother==NULL) {
			if(p->parent==NULL) {
				p = NULL;// 已经遍历到root节点，递归结束
			} else {
				p = p->parent;
			}
		} else {// 定位到p的brother子树的最左下角
			p = p->brother;
			while(p->child!=NULL) {// 定位到子树的左下角
				p = p->child;
			}
		}
		free(t);
	}
	if(p0->brother!=NULL) {// 递归删除p的所有brother及brother子树
		tree_basic_deletenode(p0->brother);
		free(p0->brother);
	}
	return;
}

/* （basic）删除层序遍历总目录中地址位t的项（及其内部引出的traverse数组）
   用途：配合tree_traverse_level函数
*/
static void tree_basic_traverse_level_deletenode(node_level *t) {
	if(level->count==1) {// level链表中仅一个节点
		free(t->traverse);
		free(t);
		level->count--;
		level->head = NULL;
		level->tail = NULL;
	} else if(t==level->head) {
		level->head = t->link2;
		level->head->link1 = NULL;
		free(t->traverse);
		free(t);
		level->count--;
	} else if(t==level->tail) {
		level->tail = t->link1;
		level->tail->link2 = NULL;
		free(t->traverse);
		free(t);
		level->count--;
	} else {
		t->link1->link2 = t->link2;
		t->link2->link1 = t->link1;
		free(t->traverse);
		free(t);
		level->count--;
	}
	return;
}



