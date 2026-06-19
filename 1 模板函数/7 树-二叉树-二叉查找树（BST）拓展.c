/* 数据结构-树-二叉树-二叉查找树（BST）拓展 函数实现
基本信息类型：intbitree
节点类型：binode
若同时建立多个二叉查找树，需建立多个binode变量类型 和 多个基本信息变量，并将所有函数的binode分别换成对应词（并设定平行的不同函数名称）
（即：一种基本信息结构体、多种binode、多套函数）
*/

/* 手动DIY目录：
DIY_data
DIY_fillnode_0(k)		
DIY_fillnode(k)			
DIY_printnode(k)		（若需调用bitree_printtree时）
DIY_print_len			（一般不用调）
DIY_search(k, x)		（若需调用bitree_search时）
DIY_compare(k1, k2)		（若需调用bitree_BST_compare时）
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

#define DIY_fillnode_0(k)		k->num = 0;
// 以k作为节点指针；用于填写成员赋值语句

#define DIY_fillnode(k)			scanf("%d", &k->num)
// 以k作为节点指针；不带“;”的scanf()语句

#define DIY_printnode(k)		printf("%d", k->num)
// 以k作为节点指针；不带“;”的printf()语句

#define DIY_print_len			8
// 打印每个节点时，占空间格数（含T/L/R:）

#define DIY_search(k, x)		k->num==x
// 以k作为节点指针；要能直接填写在if()的()里（仅用于简单的节点数据。对于多数据节点，可能要手动修改bitree_search函数！！！！！！）

#define DIY_compare(k1, k2)		k1->num - k2->num
// 以k1、k2为节点指针；保证“k1大”时值为正，“k2大”时值为负，“相等”时为0；用于填写赋值语句（仅用于简单比较。比较逻辑复杂时刻手动重写bitree_BST_compare函数）



// 树节点结构（具体根据需要填写DIY宏）
typedef struct biNode {
	DIY_data;
	struct biNode *left;
	struct biNode *right;
	struct biNode *parent;
} binode;

// 基本信息结构体
typedef struct {
	binode *root;					// 根节点
	int size;						// 树节点个数
} intbitree;

// 层序遍历结构体
typedef struct biNode_level{
	binode *root;					// 根节点地址
	binode **traverse;				// 存储层序遍历顺序的数组（队列）
	struct biNode_level *link1;
	struct biNode_level *link2;
} binode_level;

// 层序遍历总目录结构体（采用链表结构）
typedef struct {
	binode_level *head;
	binode_level *tail;
	int count;
} intlevel_bi;

// 全局变量level_bi：层序遍历总目录
static intlevel_bi _level_bi = {NULL, NULL, 0};
#define level_bi (&_level_bi)

// 全局变量bitree_error_output：控制是否打印错误信息（默认打印）
static int bitree_error_output = 1;



// 1.树结构控制
intbitree* bitree_inttree();										// 创建树（仅有根节点，data设为0）
binode* bitree_addnode_root(intbitree *tree);						// 为空树tree加入一个空根节点
binode* bitree_addnode(intbitree *tree, int target, binode *p);		// 为节点p添加一个空left or right节点
int bitree_deletenode(intbitree *tree, int target, binode *p);		// 节点删除函数
int bitree_deletetree(intbitree *tree);								// 删除全部节点，删除树信息
intbitree* bitree_copy(intbitree* tree1);							// 复制tree1

// 2.节点位置、树相关信息获取
int bitree_checkplace(intbitree *tree, binode *p);					// 可视化检查节点指针p是否合法
binode* bitree_traverse_pre(intbitree *tree, binode *p);			// 前序遍历（根左右）（根据当前接收的节点，输出下一个节点地址）
binode* bitree_traverse_in(intbitree *tree, binode *p);				// 中序遍历（左根右）（根据当前接收的节点，输出下一个节点地址）
binode* bitree_traverse_in_start(intbitree *tree, binode *p);		// （中序遍历辅助函数）定位到tree中以p为根节点的子树的中序遍历起点（左下角）
binode* bitree_traverse_post(intbitree *tree, binode *p);			// 后序遍历（左右根）（根据当前接收的节点，输出下一个节点地址）
binode* bitree_traverse_post_start(intbitree *tree, binode *p);		// （后序遍历辅助函数）定位到tree中以p为根节点的子树的后序遍历起点
binode* bitree_traverse_level(intbitree *tree, binode *p);			// 层序遍历（根据当前接收的节点，输出下一个节点地址）
int bitree_traverse_clean(intbitree *tree);							// （层序遍历辅助函数）强制清除tree的层序遍历资源（若tree=NULL，全部清除）
binode* bitree_search(intbitree *tree, int x);						// 查找数据
int bitree_countdeep_node(intbitree *tree, binode *p);				// 计算节点p的深度
int bitree_countdeep(intbitree *tree);								// 计算整个tree的深度
int bitree_isfull(intbitree *tree);									// 判断tree是否是“满二叉树”
int bitree_iscomplete(intbitree *tree);								// 判断tree是否是“完全二叉树”

// 3.节点信息录入&输出
void bitree_fillnode_0(intbitree *tree, binode *p);					// 向节点p写入0
int bitree_fillnode(intbitree *tree, binode *p);					// 向节点p录入数据
int bitree_printtree(intbitree *tree, int indentation, binode *p);	// 打印tree中以p为根的树（填写DIY_printnode时尽量只取某一数字成员）

// 4.二叉查找树（BST）相关函数
int bitree_BST_compare(binode *p1, binode *p2);						// 节点比大小（返回-2时代表错误！！！）
binode* bitree_BST_addnode(intbitree *tree, binode *p);				// 将节点p复制并按BST规则插入到tree中（左小右大）
int bitree_BST_deletenode(intbitree *tree, binode *p);				// BST规则下删除节点p
binode* bitree_BST_search(intbitree *tree, binode *p);				// 利用BST规则查找节点

// 5.其他（供其他函数调用的basic函数等）
void bitree_set_error_output(int enable);							// 控制是否可视化打印错误信息
static void bitree_basic_fillnode_0(binode *p);						// （basic）向节点p写入0
static int bitree_basic_fillnode(binode *p);						// （basic）向节点p录入数据
static binode* bitree_basic_copynode(binode *dest, binode *src);	// （basic）将节点src的数据复制给节点dest
static binode* bitree_basic_copy(binode *p, binode *parent);		// （basic）递归复制节点p及其所有子树
static void bitree_basic_deletenode(binode *p);						// （basic）删除节点p的全部子树（不调整任何tree的size）
static void bitree_basic_traverse_level_deletenode(binode_level *t);// （basic）删除层序遍历总目录中地址位t的项（及其内部引出的traverse数组）



int main () {
	
	
	
}



// 自定义函数

// 1.树结构控制
/* 创建树（仅有根节点，其data设为0）
   返回值：树信息结构体指针（失败返回NULL）
*/
intbitree* bitree_inttree() {
	// 创建树的信息并判断是否成功
	intbitree *tree = (intbitree*)malloc(sizeof(intbitree));
	if(!tree) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space for the new bitree.\n");
		return NULL;
	}
	binode *root = (binode*)malloc(sizeof(binode));
	if(!root) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space for the new bitree.\n");
		free(tree);
		return NULL;
	}
	root->left = NULL;
	root->parent = NULL;
	root->right = NULL;
	tree->root = root;
	tree->size = 1;
	bitree_basic_fillnode_0(root);
	return tree;
}

/* 为空树tree加入一个空根节点
   返回值：新节点地址（失败返回NULL）
   
   注：由于下一函数bitree_addnode必须基于某个现有节点，该函数是对其的补充
*/
binode* bitree_addnode_root(intbitree *tree) {
	if(tree==NULL) {
		if(bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return NULL;
	}
	if(tree->root!=NULL) {
		if(bitree_error_output) printf("[Illegal operation] The target bitree already has a root.\n");
		return NULL;
	}
	binode *t = (binode*)malloc(sizeof(binode));
	if(!t) {
		if(bitree_error_output) printf("[Memory exceeded] There is not enough space for the new binode.\n");
		return NULL;
	}
	t->left = NULL;
	t->parent = NULL;
	t->right = NULL;
	bitree_basic_fillnode_0(t);
	tree->root = t;
	tree->size = 1;
	return t;
}

/* 为节点p添加一个空left or right节点
   返回值：新节点地址（失败返回NULL）
   
   函数行为约定：|target|=1：仅在left/right=NULL时进行添加；|target|=2：若p已有left/right子树，将其接在新left/right子树的left/right位置（不常用，仅为保持操作的任意性）
   target：-1/-2=left，+1/+2=right
*/
binode* bitree_addnode(intbitree *tree, int target, binode *p) {
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return NULL;
	}
	if(target!=-2&&target!=-1&&target!=1&&target!=2) {
		if (bitree_error_output) printf("[Invalid function argument] The \"target\" used to select option should be -2 , -1 , 1 or 2.\n");
		return NULL;
	}
	if(bitree_checkplace(tree, p)==-1) {
		return NULL;
	}
	
	binode *t = (binode*)malloc(sizeof(binode));
	if(t==NULL) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space for the new binode.\n");
		return NULL;
	}
	bitree_basic_fillnode_0(t);
	t->left = NULL;
	t->right = NULL;
	t->parent = p;
	tree->size++;
	
	if(target<0) {
		if(p->left==NULL) {
			p->left = t;
			return t;
		} else {
			switch(target) {
				case -1:{
					if (bitree_error_output) printf("[Illegal operation] The target binode already has a left child.\n");
					free(t);
					tree->size--;
					return NULL;
					break;
				}
				case -2:{
					p->left->parent = t;
					t->left = p->left;
					p->left = t;
					return t;
					break;
				}
			}
		}
	} else {
		if(p->right==NULL) {
			p->right = t;
			return t;
		} else {
			switch(target) {
				case 1:{
					if (bitree_error_output) printf("[Illegal operation] The target binode already has a right child.\n");
					free(t);
					tree->size--;
					return NULL;
					break;
				}
				case 2:{
					p->right->parent = t;
					t->right = p->right;
					p->right = t;
					return t;
					break;
				}
			}
		}
	}
	return NULL;
}

/* 节点删除函数
   返回值：1（失败返回-1）
   
   target：-1=删除p的所有left子树，0=删除p及其所有子树（若有），1=删除p的所有right子树
*/
int bitree_deletenode(intbitree *tree, int target, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
		return -1;
	}
	if(target!=-1 && target!=0 && target!=1) {
		if(bitree_error_output) printf("[Invalid function argument] The \"target\" used to select option should be -1 , 0 or 1.\n");
		return -1;
	}
	
	switch(target) {
		case -1:{
			if(p->left==NULL) {
				if(bitree_error_output) printf("[Illegal operation] The binode p doesn't have a left child.\n");
				return -1;
			} else {
				return bitree_deletenode(tree, 0, p->left);
			}
			break;
		}
		case 0:{
			binode *p0 = p;// 记录子树根节点地址
			p = bitree_traverse_post_start(tree, p);// 定位到子树的左下角
			binode *t = p;// 配合free函数
			while(p!=p0) {
				t = p;
				p = bitree_traverse_post(tree, t);// 后序遍历找到下一节点
				free(t);
				tree->size--;
			}
			// 处理对p本身的删除
			tree->size--;
			if(p==tree->root) {// 删成空树
				tree->root = NULL;
			} else if(p->parent->left==p) {
				p->parent->left = NULL;
			} else {
				p->parent->right = NULL;
			}
			free(p);
			break;
		}
		case 1:{
			if(p->right==NULL) {
				if(bitree_error_output) printf("[Illegal operation] The binode p doesn't have a right child.\n");
				return -1;
			} else {
				return bitree_deletenode(tree, 0, p->right);
			}
			break;
		}
	}
	return 1;
}

/* 删除全部节点，删除树信息
   返回值：1（失败返回-1）
*/
int bitree_deletetree(intbitree *tree) {
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return -1;
	}
	// 处理层序遍历目录
	binode_level *t = level_bi->head;
	while(t) {
		if(tree->root==t->root) {// 当前树已在层序遍历中
			bitree_basic_traverse_level_deletenode(t);
			break;
		}
		t = t->link2;
	}
	// 删除树
	if(tree->size>0) {
		bitree_deletenode(tree, 0, tree->root);
	}
	free(tree);
	return 1;
}

/* 复制tree1
   返回值：复制出来的树的信息结构体指针（失败返回NULL）
*/
intbitree* bitree_copy(intbitree* tree1) {
	if(tree1==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return NULL;
	}
	intbitree *tree2 = (intbitree*)malloc(sizeof(intbitree));
	if(!tree2) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space for the new bitree.\n");
		return NULL;
	}
	tree2->root = NULL;
	tree2->size = tree1->size;
	tree2->root = bitree_basic_copy(tree1->root, NULL);// 调用basic函数对每个节点（及其子树）进行递归复制
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
int bitree_checkplace(intbitree *tree, binode *p) {
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return -1;
	}
	if(p==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The position should not be NULL.\n");
		return -1;
	}
	if(tree->size==0) {
		if (bitree_error_output) printf("[Other errors] The target bitree is empty.\n");
		return -1;
	}
	return 1;
}

/* 前序遍历（根左右）（根据当前接收的节点，输出下一个节点地址）
   返回值：下一个节点地址（错误返回NULL）
*/
binode* bitree_traverse_pre(intbitree *tree, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
		return NULL;
	}
	if(p->left!=NULL) return p->left;
	if(p->right!=NULL) return p->right;
	// 从p往上找第一个有right的本身是left的祖上节点（含p本身）
	while(p->parent!=NULL) {
		if(p->parent->left==p && p->parent->right!=NULL) return p->parent->right;
		p = p->parent;
	}
	// 若还没return：p一直向上找到了root节点，递归结束
	return NULL;
}

/* 中序遍历（左根右）（根据当前接收的节点，输出下一个节点地址）
   返回值：下一个节点地址（错误 or 遍历结束返回NULL）
   
   注意：由于不是从根节点开始遍历，完整遍历时需要先手动（调用bitree_traverse_in_start）定位到中序首节点处
*/
binode* bitree_traverse_in(intbitree *tree, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
		return NULL;
	}
	if(p->right!=NULL) {// 有right：去right的最左下
		return bitree_traverse_in_start(tree, p->right);
	} else {// 无right
		if(p->parent==NULL) {// 极端情况：p为根节点且无right子树
			return NULL;
		} else if(p->parent->left==p) {// p是left
			return p->parent;
		} else {// p是right
			while(p->parent!=NULL && p->parent->left!=p) {// 向上找第一个是left的节点；此处利用逻辑运算的短路特性，避免了空指针解引用风险
				p = p->parent;
			}
			return p->parent;// 包含遍历终止
		}
	}
}

/* （中序遍历辅助函数）定位到tree中以p为根节点的子树的中序遍历起点（左下角）
   返回值：左下角节点地址（错误返回NULL）
   
   注：中序、后序遍历起点可能不一样！
*/
binode* bitree_traverse_in_start(intbitree *tree, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
		return NULL;
	}
	while(p->left!=NULL) {
		p = p->left;
	}
	return p;
}

/* 后序遍历（左右根）（根据当前接收的节点，输出下一个节点地址）
   返回值：下一个节点地址（错误 or 遍历结束返回NULL）
   
   注意：由于不是从根节点开始遍历，完整遍历时需要先手动（调用bitree_traverse_post_start）定位到后序首节点处
*/
binode* bitree_traverse_post(intbitree *tree, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
		return NULL;
	}
	
	if(p->parent==NULL) {
		return NULL;// 已经遍历到root节点，递归结束
	}
	if(p->parent->left==p) {// p是left节点
		if(p->parent->right==NULL) {
			return p->parent;
		} else {// 定位到p->parent->right子树的后序遍历起点
			return bitree_traverse_post_start(tree, p->parent->right);
		}
	} else {// p是right节点
		return p->parent;
	}
}

/* （后序遍历辅助函数）定位到tree中以p为根节点的子树的后序遍历起点
   返回值：左下角节点地址（错误返回NULL）
   
   注：中序、后序遍历起点可能不一样！
*/
binode* bitree_traverse_post_start(intbitree *tree, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
		return NULL;
	}
	while(p->left!=NULL || p->right!=NULL) {
		if(p->left!=NULL) {
			p = p->left;
		} else {
			p = p->right;
		}
	}
	return p;
}

/* 层序遍历（根据当前接收的节点，输出下一个节点地址）
   返回值：下一个节点地址（错误 or 遍历结束返回NULL）
*/
binode* bitree_traverse_level(intbitree *tree, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
		return NULL;
	}
	binode_level *t = NULL;
	for(t = level_bi->head; t!=NULL; t = t->link2) {
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
		bitree_basic_traverse_level_deletenode(t);
		return NULL;
	} else {
		t = (binode_level*)malloc(sizeof(binode_level));// 创建新的binode_level节点
		if(!t) {
			if (bitree_error_output) printf("[Memory exceeded] There are not enough space to do the level-order traverse.\n");
			return NULL;
		}
		// 构建层序遍历序列
		t->root = tree->root;
		t->traverse = (binode**)malloc(sizeof(binode*)*tree->size);// 层序遍历数组（只进不出的队列）
		if(!t->traverse) {
			if (bitree_error_output) printf("[Memory exceeded] There are not enough space to do the level-order traverse.\n");
			free(t);
			return NULL;
		}
		t->traverse[0] = tree->root;
		for(int k1 = 0, k2 = 1; k1<tree->size; k1++) {// k1k2：记录（所谓）队列出、入队位置
			if(t->traverse[k1]->left!=NULL) {// traverse[k1]有left child
				t->traverse[k2++] = t->traverse[k1]->left;
			}
			if(t->traverse[k1]->right!=NULL) {// traverse[k1]有right child
				t->traverse[k2++] = t->traverse[k1]->right;
			}
		}
		
		// 添加新节点
		if(level_bi->count==0) {// 添加第一个结点
			level_bi->head = t;
			level_bi->tail = t;
			t->link1 = NULL;
			t->link2 = NULL;
			level_bi->count++;
		} else {
			t->link1 = level_bi->tail;
			t->link2 = NULL;
			level_bi->tail->link2 = t;
			level_bi->tail = t;
			level_bi->count++;
		}
		
		// 找到该输出哪个节点
		for(int i=0; i<tree->size-1; i++) {// “i<tree->size-1”：p不是最后一个节点
			if(t->traverse[i]==p) {
				return t->traverse[i+1];
			}
		}
		// 此时必应当遍历结束，需注销相应的层序遍历结构体(t)
		bitree_basic_traverse_level_deletenode(t);
		return NULL;
	}
}

/* （层序遍历辅助函数）强制清除tree的层序遍历资源（若tree=NULL，全部清除）
   返回值：所清除的层序遍历目录条数（即从全局链表中删除了几个binode_level节点）（错误返回-1）s
*/
int bitree_traverse_clean(intbitree *tree) {
	if(!tree) {// 全部清除
		int temp = level_bi->count;
		while(level_bi->count>0) {
			bitree_basic_traverse_level_deletenode(level_bi->head);
		}
		return temp;
	} else {
		for(binode_level *t = level_bi->head; t!=NULL; t = t->link2) {
			if(t->root==tree->root) {
				bitree_basic_traverse_level_deletenode(t);
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
binode* bitree_search(intbitree *tree, int x) {
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return NULL;
	}
	if(tree->size==0) return NULL;
	binode *t = tree->root;
    do {
        if (DIY_search(t, x)) {// 根据所查找成员&节点结构DIY
        	return t;
		}
        t = bitree_traverse_pre(tree, t);
    } while (t != NULL);
    return NULL;
}

/* 计算节点p的深度
   返回值：节点p深度（错误返回-1）（根深度为1）
*/
int bitree_countdeep_node(intbitree *tree, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
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
int bitree_countdeep(intbitree *tree) {
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return -1;
	}
	int deep = 0;
	for(binode *t = tree->root; t!=NULL;) {
		if(t->left==NULL && t->right==NULL) {// 如果是叶节点
			int temp = bitree_countdeep_node(tree, t);
			deep = deep<temp? temp : deep;// 取max
		}
		t = bitree_traverse_pre(tree, t);
	}
	return deep;
}

/* 判断tree是否是“满二叉树”
   返回值：是=1.不是=0（错误返回-1）
*/
int bitree_isfull(intbitree *tree) {
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return -1;
	}
	if(tree->size==0) return 1;// 规定
	int deep = bitree_countdeep(tree);
	if(deep == -1) return -1;
	
	return (tree->size == (1<<deep)-1);// yes~位运算！总算用上一回
}

/* 判断tree是否是“完全二叉树”
   返回值：是=1.不是=0（错误返回-1）
*/
int bitree_iscomplete(intbitree *tree) {
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return -1;
	}
	if(tree->size==0) return 1;// 规定
	
	
	binode **queue = (binode**)malloc(sizeof(binode*)*tree->size);
	if(!queue) {
		if (bitree_error_output) printf("[Memory exceeded] There are not enough space to check complete binary tree.\n");
		return -1;
	}
	int in = 0, out = 0;
	int sign = 0;// 未到子树残缺节点
	queue[in++] = tree->root;
	while(out<in) {
		binode *t = queue[out++];
		
		
		if(t->left!=NULL && t->right!=NULL) {
			if(sign!=0) {
				free(queue);
				return 0;
			} else {
				queue[in++] = t->left;
				queue[in++] = t->right;
			}
		} else if(t->left!=NULL && t->right==NULL) {
			if(sign==0) {
				sign = 1;
				queue[in++] = t->left;
			} else {
				free(queue);
				return 0;
			}
		} else if(t->left==NULL && t->right!=NULL) {
			free(queue);
			return 0;
		} else {
			if(sign==0) {
				sign = 1;
			}
		}
	}
	free(queue);
	return 1;
}



// 3.节点信息录入&输出
/* 向节点p写入0
*/
void bitree_fillnode_0(intbitree *tree, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
		return;
	}
	bitree_basic_fillnode_0(p);
	return;
}

/* 向节点p录入数据
   返回值：本节点成功录入数据个数（读取到文件尾or错误时返回-1）
*/
int bitree_fillnode(intbitree *tree, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
		return -1;
	}
	int t;
	if((t = bitree_basic_fillnode(p))<=0) {// 数据不足or输入的数据类型错误
		if (bitree_error_output) printf("[Input error] Failed to input data.\n");
		return -1;
	}
	return t;
}

/* 打印tree中以p为根的树（填写DIY_printnode时尽量只取某一数字成员）
   返回值：1（空树等错误返回-1）
*/
int bitree_printtree(intbitree *tree, int indentation, binode *p) {// indentation：缩进值，初始调用应为0
	if(bitree_checkplace(tree, p)==-1) {
		return -1;
	}
	if(p == NULL) {
		if(bitree_error_output) printf("[Other errors] The target bitree is empty.\n");
		return -1;
	}
	
	if(p->right != NULL) {
		bitree_printtree(tree, indentation + 1, p->right);// 默认含回车
	}
	// 缩进
	for(int i=1; i<=indentation; i++) {
		for(int j=1; j<=DIY_print_len; j++) {
			printf(" ");
		}
	}
	// 打印节点
	if(p == tree->root) {
		printf("-T:");
	} else if(p == p->parent->right) {
		printf("/R:");
	} else {
		printf("\\L:");
	}
	DIY_printnode(p);
	printf("\n");
	
	if(p->left != NULL) {
		bitree_printtree(tree, indentation + 1, p->left);// 默认含回车
	}
	return 1;
}



// 4.二叉查找树（BST）相关函数
/* 节点比大小（返回-2时代表错误！！！）
   返回值：p1大时返回1，p2大时返回-1，相等时返回0（错误返回-2！！！）
*/
int bitree_BST_compare(binode *p1, binode *p2) {
	if(p1==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target binode 1 pointer is NULL.\n");
		return -2;
	}
	if(p2==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target binode 2 pointer is NULL.\n");
		return -2;
	}
	
	int x = DIY_compare(p1, p2);
	if(x>0) return 1;
	if(x<0) return -1;
	else return 0;
}

/* 将节点p复制并按BST规则插入到tree中（左小右大）
   返回值：tree中新建节点地址（重复时返回重复节点地址）（错误返回NULL）
   
   调用方式：先在main中定义一个临时节点p，再导入bitree_BST_addnode中
*/
binode* bitree_BST_addnode(intbitree *tree, binode *p) {
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return NULL;
	}
	if(p==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target binode pointer is NULL.\n");
		return NULL;
	}
	
	binode *t = (binode*)malloc(sizeof(binode));
	if(!t) {
		if(bitree_error_output) printf("[Memory exceeded] There is not enough space for the new binode.\n");
		return NULL;
	}
	t->left = NULL;
	t->parent = NULL;
	t->right = NULL;
	bitree_basic_copynode(t, p);
	
	if(tree->size==0) {// 树原来为空
		tree->root = t;
		tree->size++;
		return t;
	}
	binode *k = tree->root;
	while(1) {
		switch(bitree_BST_compare(t, k)) {
			case 1:{// t大
				if(k->right==NULL) {
					k->right = t;
					t->parent = k;
					tree->size++;
					return t;
				} else {
					k = k->right;
				}
				break;
			}
			case 0:{
				free(t);
				return k;
				break;
			}
			case -1:{// t小
				if(k->left==NULL) {
					k->left = t;
					t->parent = k;
					tree->size++;
					return t;
				} else {
					k = k->left;
				}
				break;
			}
			case -2:{
				free(t);
				return NULL;
				break;
			}
		}
	}
}

/* BST规则下删除节点p
   返回值：1（失败返回-1）
   
   函数行为约定：
   1.若被删除结点为叶结点，则直接删除
   2.若被删除结点无左子树，则用右子树的根结点取代被删除结点（无右子树同理）
   3.若被删除结点的左、右子树都存在，则将被删除结点的右子树中值最小的结点（or左子树中值最大的节点）拿来，取代被删除结点
*/
int bitree_BST_deletenode(intbitree *tree, binode *p) {
	if(bitree_checkplace(tree, p)==-1) {
		return -1;
	}
	
	if(p->left==NULL&&p->right==NULL) {// 叶节点
		if(p==tree->root) {
			tree->root = NULL;
		} else {
			if(p->parent->left==p) {
				p->parent->left = NULL;
			} else {
				p->parent->right = NULL;
			}
		}
		tree->size--;
		free(p);
	} else if(p->left==NULL && p->right!=NULL) {// 有右无左
		if(p==tree->root) {
			tree->root = p->right;
			tree->root->parent = NULL;
		} else {
			if(p->parent->left==p) {
				p->parent->left = p->right;
				p->right->parent = p->parent;
			} else {
				p->parent->right = p->right;
				p->right->parent = p->parent;
			}
		}
		tree->size--;
		free(p);
	} else if(p->left!=NULL && p->right==NULL) {// 有左无右
		if(p==tree->root) {
			tree->root = p->left;
			tree->root->parent = NULL;
		} else {
			
			if(p->parent->left==p) {
				p->parent->left = p->left;
				p->left->parent = p->parent;
			} else {
				p->parent->right = p->left;
				p->left->parent = p->parent;
			}
		}
		tree->size--;
		free(p);
	} else {// 有左有右→找右小
		binode *t = p->right;
		while(t->left!=NULL) {
			t = t->left;
		}
		// 用t取代p
		bitree_basic_copynode(p, t);
		bitree_BST_deletenode(tree, t);// 自带size--和free(t)
	}
	return 1;
}

/* 利用BST规则查找节点
   返回值：目标节点指针（找不到返回NULL）
   
   调用方式：先在main中定义一个临时节点p，再导入bitree_BST_search中
*/
binode* bitree_BST_search(intbitree *tree, binode *p) {
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target bitree pointer is NULL.\n");
		return NULL;
	}
	if(p==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target binode pointer is NULL.\n");
		return NULL;
	}
	
	binode *t = tree->root;
	while(1) {
		switch(bitree_BST_compare(p, t)) {
			case 0:{
				return t;
				break;
			}
			case 1:{// p大
				if(t->right==NULL) {
					return NULL;
				}
				t = t->right;
				break;
			}
			case -1:{// p小
				if(t->left==NULL) {
					return NULL;
				}
				t = t->left;
				break;
			}
			case -2:{
				return NULL;
				break;
			}
		}
	}
}



// 5.其他（供其他函数调用的basic函数等）
/* 控制是否可视化打印错误信息
*/
void bitree_set_error_output(int enable) {// 1→打印；0→不打印
	bitree_error_output = enable ? 1 : 0;
	return;
}

/* （basic）向节点p写入0
   用途：无需填写tree指针的节点归零
*/
static void bitree_basic_fillnode_0(binode *p) {
	DIY_fillnode_0(p);// 具体根据节点实际结构填写DIY宏
}

/* （basic）向节点p录入数据
   返回值：本节点成功录入数据个数（读取到文件尾时返回-1）
   用途：无需填写tree指针的节点数据录入
*/
static int bitree_basic_fillnode(binode *p) {
	return DIY_fillnode(p);// 具体根据节点实际结构填写DIY宏
}

/* （basic）将节点src的数据复制给节点dest
   返回值：dest节点地址
   用途：配合bitree_basic_copy函数，取代DIY_copy宏
   
   注意：为浅拷贝，若data区存在指针，可能会导致double-free，调用时小心
*/
static binode* bitree_basic_copynode(binode *dest, binode *src) {
	binode *parent = dest->parent;
	binode *left = dest->left;
	binode *right = dest->right;
	*dest = *src;
	dest->right = right;
	dest->left = left;
	dest->parent = parent;
	return dest;
}

/* （basic）递归复制节点p及其所有子树
   返回值：复制出来的节点p的对应节点（失败返回NULL）
   用途：配合bitree_copy函数
   
   注意：若节点p为NULL，返回值也为NULL，请注意与malloc失败的区分！
*/
static binode* bitree_basic_copy(binode *p, binode *parent) {// 复制信息源的节点p地址，新树中对应节点的->parent地址
	if(p==NULL) return NULL;
	binode *t = (binode*)malloc(sizeof(binode));
	if(!t) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space for the new bitree.\n");
		return NULL;
	}
	
	t->right = NULL;
	t->left = NULL;
	t->parent = parent;
	bitree_basic_copynode(t, p);
	if(p->left!=NULL) {
		t->left = bitree_basic_copy(p->left, t);
		if(t->left==NULL) {// 说明复制失败，malloc时空间不够
			free(t);
			return NULL;
		}
	}
	if(p->right!=NULL) {
		t->right = bitree_basic_copy(p->right, t);
		if(t->right==NULL) {// 说明复制失败，malloc时空间不够
			bitree_basic_deletenode(t);// 回滚掉可能已经复制出的left子树
			free(t);
			return NULL;
		}
	}
	return t;
}

/* （basic）删除节点p的全部子树（不调整任何tree的size）
   用途：配合bitree_basic_copy函数，当内存不足时对已复制节点进行回滚
*/
static void bitree_basic_deletenode(binode *p) {
	binode *p0 = p;// 记录子树根节点地址
	while(p->left!=NULL || p->right!=NULL) {// 定位到子树的后序遍历起点
		if(p->left!=NULL) {
			p = p->left;
		} else {
			p = p->right;
		}
	}
	binode *t = p;// 配合free函数
	while(p!=p0) {// 后序遍历删除p的全部子树
		t = p;
		if(p->parent==NULL) {
			p = NULL;// 已经遍历到root节点，递归结束
		} else if(p->parent->left==p) {// p是left节点
			if(p->parent->right==NULL) {
				p = p->parent;
			} else {// 定位到p->parent->right子树的后序遍历起点
				p = p->parent->right;
				while(p->left!=NULL || p->right!=NULL) {
					if(p->left!=NULL) {
						p = p->left;
					} else {
						p = p->right;
					}
				}
			}
		} else {// p是right节点
			p = p->parent;
		}
		free(t);
	}
	return;
}

/* （basic）删除层序遍历总目录中地址位t的项（及其内部引出的traverse数组）
   用途：配合bitree_traverse_level函数
*/
static void bitree_basic_traverse_level_deletenode(binode_level *t) {
	if(level_bi->count==1) {// level链表中仅一个节点
		free(t->traverse);
		free(t);
		level_bi->count--;
		level_bi->head = NULL;
		level_bi->tail = NULL;
	} else if(t==level_bi->head) {
		level_bi->head = t->link2;
		level_bi->head->link1 = NULL;
		free(t->traverse);
		free(t);
		level_bi->count--;
	} else if(t==level_bi->tail) {
		level_bi->tail = t->link1;
		level_bi->tail->link2 = NULL;
		free(t->traverse);
		free(t);
		level_bi->count--;
	} else {
		t->link1->link2 = t->link2;
		t->link2->link1 = t->link1;
		free(t->traverse);
		free(t);
		level_bi->count--;
	}
	return;
}





