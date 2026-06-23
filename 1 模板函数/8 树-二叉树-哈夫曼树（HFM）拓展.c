/* 数据结构-树-二叉树-哈夫曼树（HFM）拓展 函数实现
基本信息类型：intbitree
节点类型：binode
若同时建立多种哈夫曼树，需建立多种node变量类型 和 多个基本信息变量（并设定平行的不同的函数名称）
（即：一种基本信息结构体、多种binode、多套函数）
*/

/* 手动DIY目录：
DIY_data
DIY_fillnode_0(k)		
DIY_fillnode(k)			
DIY_printnode(k)		（若需调用bitree_printtree时）
DIY_print_len			（一般不用调）
DIY_search(k, x)		（若需调用bitree_search时）
DIY_HFM_size			（一般不用调）
*/

/* 错误说明：
[Invalid function argument]	非法函数参数
[Memory exceeded]			MLE
[Input error]				读取输入失败
[Illegal operation]			非法操作
[Other errors]				其他（空树、哈夫曼密码表残缺）
*/



#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define DIY_data				unsigned char letter;int weight// e.g.字符，哈夫曼权重
// 用于自定义节点的数据结构

#define DIY_fillnode_0(k)		k->letter = 0;k->weight = 0
// 以k作为节点指针；用于填写成员赋值语句

#define DIY_fillnode(k)			scanf("%c%d", &k->letter, &k->weight)
// 以k作为节点指针；不带“;”的scanf()语句

#define DIY_printnode(k)		printf("%c %d", k->letter, k->weight)
// 以k作为节点指针；不带“;”的printf()语句

#define DIY_print_len			8
// 打印每个节点时，占空间格数（含T/L/R:）

#define DIY_search(k, x)		k->letter==x
// 以k作为节点指针；要能直接填写在if()的()里（仅用于简单的节点数据。对于多数据节点，可能要手动修改bitree_search函数！！！！！！）

#define DIY_HFM_size			300
// 哈夫曼编码表的大小，设为300足以满足对全部ASCLL字符的编码



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

// 字符串哈夫曼编码（字节流）存储结构体
typedef struct {
	unsigned char *data;			// 01字节流
	int len_byte;					// 字节数组长度
	int len_bit;					// 实际有效字节长度
} HFM_coding;

// 全局变量level_bi：层序遍历总目录
static intlevel_bi _level_bi = {NULL, NULL, 0};
#define level_bi (&_level_bi)

// 全局变量bitree_error_output：控制是否打印错误信息（默认打印）
static int bitree_error_output = 1;



// 1.树的结构控制
intbitree* bitree_inttree();										// 创建树（仅有根节点，data设为0）
binode* bitree_addnode_root(intbitree *tree);						// 为空树tree加入一个空根节点
binode* bitree_addnode(intbitree *tree, int target, binode *p);		// 为节点p添加一个空left or right节点
int bitree_deletenode(intbitree *tree, int target, binode *p);		// 节点删除函数
int bitree_deletetree(intbitree *tree);								// 删除全部节点，删除树信息
intbitree* bitree_copy(intbitree* tree1);							// 复制tree1

// 2.节点、树相关信息获取
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

// 3.树的数据控制
void bitree_fillnode_0(intbitree *tree, binode *p);					// 向节点p写入0
int bitree_fillnode(intbitree *tree, binode *p);					// 向节点p录入数据
int bitree_printtree(intbitree *tree, int indentation, binode *p);	// 打印tree中以p为根的树（填写DIY_printnode时尽量只取某一数字成员）

// 4.哈夫曼树（HFM）相关函数
intbitree* bitree_HFM_inttree(binode **a, int len);					// 根据节点指针数组a创建哈夫曼树
char** bitree_HFM_gencode(intbitree *tree);							// 根据哈夫曼树生成哈夫曼密码表（字符串数组）
int bitree_HFM_deletecode(char **password);							// 删除哈夫曼密码表（字符串指针数组及对应字符串）
HFM_coding* bitree_HFM_encoding(char *s, char **password);			// 将字符串s根据哈夫曼密码表password进行字节流形式编码
char* bitree_HFM_decoding(HFM_coding *save, intbitree *tree);		// 将字符串的哈夫曼编码（字节流）save根据对应的哈夫曼树tree解码成字符串
char* bitree_HFM_coding_tostring(HFM_coding *save);					// 以字符串形式转存字节流形式的哈夫曼编码save

// 5.其他（供其他函数调用的basic函数等）
void bitree_set_error_output(int enable);							// 控制是否可视化打印错误信息
static void bitree_basic_fillnode_0(binode *p);						// （basic）向节点p写入0
static int bitree_basic_fillnode(binode *p);						// （basic）向节点p录入数据
static binode* bitree_basic_copynode(binode *dest, binode *src);	// （basic）将节点src的数据复制给节点dest
static binode* bitree_basic_copy(binode *p, binode *parent);		// （basic）递归复制节点p及其所有子树
static void bitree_basic_deletenode(binode *p);						// （basic）删除节点p的全部子树（不调整任何tree的size）
static void bitree_basic_traverse_level_deletenode(binode_level *t);// （basic）删除层序遍历总目录中地址位t的项（及其内部引出的traverse数组）
int bitree_basic_HFM_cmp(const void *a, const void *b);				// （basic）HFM函数专用的qsort比较函数
int bitree_basic_HFM_gencode(char **password, char *path, int path_len, binode *p);
																	// （basic）根据哈夫曼树tree，利用path数组，递归填写password字符串数组



int main () {
	
	
	
}



// 自定义函数

// 1.树的结构控制
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



// 2.节点、树相关信息获取
/* 可视化检查节点指针p是否合法
   返回值：1（位置非法返回-1）
   
   函数行为约定：
   1.仅进行简单O(1)级别检查，不检查地址是否一定在树中（若需检查，可手动回溯parent链与tree->root对比）
   2.返回1时确保：树存在且size≥1，指针非NULL
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
   
   注：由于不是从根节点开始遍历，完整遍历时需要先手动（调用bitree_traverse_in_start）定位到中序首节点处
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
   
   注：由于不是从根节点开始遍历，完整遍历时需要先手动（调用bitree_traverse_post_start）定位到后序首节点处
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



// 3.树的数据控制
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



// 4.哈夫曼树（HFM）相关函数
// 注：使用HFM函数时，默认节点DIY_data = unsigned char letter;int weight。否则需要手动修改HFM函数
/* 根据节点指针数组a创建哈夫曼树
   返回值：树信息结构体指针（失败返回NULL）
   
   函数行为约定：所传入的节点指针数组a应该是malloc来的，函数将直接复用，不再重新malloc叶节点；
				 传入的数组不必有序；
				 构造哈夫曼树时，（等效为）初始按weight、ASCLL双关键词升序，新权重节点插入升序权重序列中时，出现相同权重时，插入到其后（稳定排序）；
				 左右child默认：weight大的在右（具体可修改“t->left = stack[size-1];这几句”）
*/
intbitree* bitree_HFM_inttree(binode **a, int len) {// 节点指针数组，节点指针数组长度
	if(len<=0) {
		if (bitree_error_output) printf("[Invalid function argument] The length of leaf array should >= 1.\n");
		return NULL;
	}
	if(a==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target leaf pointer array is NULL.\n");
		return NULL;
	}
	
	// 创建树的信息并判断是否成功
	intbitree *tree = (intbitree*)malloc(sizeof(intbitree));
	if(!tree) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space for the new bitree.\n");
		return NULL;
	}
	
	// 特判len=1情况
	if(len==1) {
		tree->root = a[0];
		a[0]->left = a[0]->parent = a[0]->right = NULL;
		tree->size = 1;
		return tree;
	}
	
	// 构建临时节点指针栈（数组），用来生成哈夫曼树
	int size = len;// 实时stack元素数
	int size1 = 0;// 实时stack1元素数
	binode **stack = (binode**)malloc(sizeof(binode*)*len);// ！！！该数组仅存放指针，不拷贝节点信息
	binode **stack1 = (binode**)malloc(sizeof(binode*)*(len-1));// 存放inttree函数内部malloc的节点的指针，用于空间不足时进行回滚
	if(!stack) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space to create the new bitree.\n");
		free(tree);
		return NULL;
	}
	if(!stack1) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space to create the new bitree.\n");
		free(tree);
		free(stack);
		return NULL;
	}
	for(int i=0; i<=len-1; i++) {
		stack[i] = a[i];
		stack[i]->left = stack[i]->parent = stack[i]->right = NULL;
	}
	qsort(stack, len, sizeof(binode*), bitree_basic_HFM_cmp);// 按权重降序排列
	while(size>1) {// 新生成的权重节点插入到降序权重序列中时，出现相同权重时，插入到其前（稳定排序）
		binode *t = (binode*)malloc(sizeof(binode));// 新malloc出非叶节点
		if(!t) {
			if (bitree_error_output) printf("[Memory exceeded] There is not enough space for the new bitree.\n");
			free(stack);
			free(tree);
			if(size1>0) {
				for(int i=0; i<=size1-1; i++) {
					free(stack1[i]);
				}
			}
			free(stack1);
			return NULL;
		}
		stack1[size1++] = t;
		t->letter = '#';// 表示内部生成节点，方便print调试
		t->left = stack[size-1];
		t->right = stack[size-2];
		t->left->parent = t;
		t->right->parent = t;
		t->weight = t->left->weight + t->right->weight;
		stack[size-2] = t;
		size--;
		for(int i=size-1; i>0; i--) {// 按权重降序排列
			binode *t1;
			if(stack[i]->weight >= stack[i-1]->weight) {
				t1 = stack[i-1];
				stack[i-1] = stack[i];
				stack[i] = t1;
			} else break;
		}
	}
	tree->root = stack[0];
	free(stack);
	free(stack1);
	tree->size = 2*len-1;
	return tree;
}
 
/* 根据哈夫曼树生成哈夫曼密码表（字符串指针数组）
   返回值：所生成的哈夫曼密码表的指针，为char**
   
   函数行为约定：构建的密码表password[i]代表ASCLL=i字符的哈夫曼字符串，未出现的字符处写NULL；默认传入的树是合法哈夫曼树
*/
char** bitree_HFM_gencode(intbitree *tree) {
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target HFM tree pointer is NULL.\n");
		return NULL;
	}
	if(tree->size==0) {
		if (bitree_error_output) printf("[Other errors] The target HFM tree is empty.\n");
		return NULL;
	}
	
	char **password = (char**)malloc(sizeof(char*)*DIY_HFM_size);// 密码表地址（字符串指针的数组）
	if(!password) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space to create the HFM password list.\n");
		return NULL;
	}
	for(int i=0; i<DIY_HFM_size; i++) {
		password[i] = NULL;// 全部置为NULL，方便空间不足时回滚
	}
	// 特判：size=1的树
	if(tree->size==1) {
		char *t = (char*)malloc(sizeof(char)*2);
		if(!t) {
			if (bitree_error_output) printf("[Memory exceeded] There is not enough space to create the HFM password list.\n");
			free(password);
			return NULL;
		}
		t[0] = '0';
		t[1] = '\0';
		password[tree->root->letter] = t;
		return password;
	}
	
	char path[DIY_HFM_size] = {};// 哈夫曼树的极端可能最大层数
	if(bitree_basic_HFM_gencode(password, path, 0, tree->root)==-1) {
		for(int i=0; i<DIY_HFM_size; i++) {
			if(password[i]!=NULL) free(password[i]);
		}
		free(password);
		return NULL;
	}
	
	return password;
}

/* 删除哈夫曼密码表（字符串指针数组及对应字符串）
   返回值：返回值：1（失败返回-1）
*/
int bitree_HFM_deletecode(char **password) {
	if(password==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target password list pointer is NULL.\n");
		return -1;
	}
	
	for(int i=0; i<DIY_HFM_size; i++) {
		if(password[i]!=NULL) {
			free(password[i]);
		}
	}
	free(password);
	return 1;
}

/* 将字符串s根据哈夫曼密码表password进行字节流形式编码
   返回值：哈夫曼编码（字节流形式）存储结构体指针（错误返回NULL）
*/
HFM_coding* bitree_HFM_encoding(char *s, char **password) {
	if(s==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target ASCLL string pointer is NULL.\n");
		return NULL;
	}
	if(password==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target password list pointer is NULL.\n");
		return NULL;
	}
	
	HFM_coding *save = (HFM_coding*)malloc(sizeof(HFM_coding));
	if(!save) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space to do the HFM encoding.\n");
		return NULL;
	}
	int sum = 0;
	for(int i = 0; i<strlen(s); i++) {
		sum += strlen(password[(int)s[i]]);
	}
	sum = (sum + 7) / 8 + 1;// 向上取整，结尾留一个空位标记结束
	save->data = (unsigned char*)malloc(sum);
	if(!save->data) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space to do the HFM encoding.\n");
		return NULL;
	}
	memset(save->data, 0, sum);// 归0
	save->len_bit = 0;// 实时记录当前已填充的位数
	for(int i=0; i<strlen(s); i++) {// 填充data
		if(password[(int)s[i]]==NULL) {
			if (bitree_error_output) printf("[Other errors] The letter \"%c\" was not registered in the password list.\n", s[i]);
			free(save->data);
			free(save);
			return NULL;
		}
		int tlen = strlen(password[(int)s[i]]);
		for(int j=0; j<tlen; j++) {
			if(password[(int)s[i]][j]=='1') {
				save->data[save->len_bit / 8] |= 1 << (7 - (save->len_bit % 8));
			}
			save->len_bit++;
		}
	}
	save->len_byte = (save->len_bit + 7) / 8;
	
	return save;
}

/* 将字符串的哈夫曼编码（字节流）save根据对应的哈夫曼树tree解码成字符串
   返回值：解码结果字符串指针（错误返回NULL）
*/
char* bitree_HFM_decoding(HFM_coding *save, intbitree *tree) {
	if(save==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target HFM_coding pointer is NULL.\n");
		return NULL;
	}
	if(tree==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target HFM tree pointer is NULL.\n");
		return NULL;
	}
	if(tree->size==0) {
		if (bitree_error_output) printf("[Other errors] The target HFM tree is empty.\n");
		return NULL;
	}
	
	// 特判：只有根的哈夫曼树解码
	if(tree->size==1) {
		char *s = (char*)malloc(save->len_bit+1);
		if(!s) {
			if (bitree_error_output) printf("[Memory exceeded] There is not enough space to do the HFM decoding.\n");
			return NULL;
		}
		for(int i=0; i<save->len_bit; i++) {
			s[i] = tree->root->letter;
		}
		s[save->len_bit] = '\0';
		return s;
	}
	
	// 解析save->data的完整填充部分
	char *s = (char*)malloc(1), *t0;// t0用于临时接受realloc的指针
	s[0] = '\0';
	int len = 1;// 实时记录当前s数组长度（含存放末尾'\0'的位置）
	binode *t = tree->root;
	for(int i = 0; i < save->len_bit / 8; i++) {
		for(int j = 7; j >= 0; j--) {
			int temp = (save->data[i] & (1 << j)) >> j;
			if(temp == 1) {
				t = t->right;
			} else {
				t = t->left;
			}
			if(t == NULL) {// 哈夫曼树or编码有问题
				if (bitree_error_output) printf("[Other errors] This HFM coding does not match the HFM tree.\n");
				free(s);
				return NULL;
			}
			if(t->left == NULL && t->right == NULL) {
				t0 = (char*)realloc(s, len + 1);// 将s扩充一个字节，并写'\0'
				if(!t0) {
					if (bitree_error_output) printf("[Memory exceeded] There is not enough space to do the HFM decoding.\n");
					free(s);
					return NULL;
				}
				s = t0;
				s[len] = '\0';
				s[len - 1] = t->letter;
				t = tree->root;
				len++;
			}
		}
	}
	
	// 解析save->data的末尾不足8位部分
	if(save->len_bit % 8 != 0) {
		for(int i = 7; i >= 8 - save->len_bit % 8; i--) {
			int temp = (save->data[save->len_bit / 8] & (1 << i)) >> i;
			if(temp == 1) {
				t = t->right;
			} else {
				t = t->left;
			}
			if(t == NULL) {// 哈夫曼树or编码有问题
				if (bitree_error_output) printf("[Other errors] This HFM coding does not match the HFM tree.\n");
				free(s);
				return NULL;
			}
			if(t->left == NULL && t->right == NULL) {
				t0 = (char*)realloc(s, len + 1);// 将s扩充一个字节，并写'\0'
				if(!t0) {
					if (bitree_error_output) printf("[Memory exceeded] There is not enough space to do the HFM decoding.\n");
					free(s);
					return NULL;
				}
				s = t0;
				s[len] = '\0';
				s[len - 1] = t->letter;
				t = tree->root;
				len++;
			}
		}
	}
	return s;
}

/* 以字符串形式转存字节流形式的哈夫曼编码save
   返回值：哈夫曼编码（字符串形式）的字符串指针（错误返回NULL）
*/
char* bitree_HFM_coding_tostring(HFM_coding *save) {
	if(save==NULL) {
		if (bitree_error_output) printf("[Invalid function argument] The target HFM_coding pointer is NULL.\n");
		return NULL;
	}
	if(save->data == NULL || save->len_bit == 0) {
		if (bitree_error_output) printf("[Invalid function argument] The target HFM_coding is invalid.\n");
		return NULL;
	}
	
	char *s = (char*)malloc(save->len_bit+1);// +1用于存储字符串结尾标记
	if(!s) {
		if (bitree_error_output) printf("[Memory exceeded] There is not enough space to do the HFM decoding.\n");
		return NULL;
	}
	int count = 0;// 下一个数字该填在s数组的哪个地方
	// 翻译save->data的完整填充部分
	for(int i = 0; i < save->len_bit / 8; i++) {
		for(int j = 7; j >= 0; j--) {
			s[count++] = ((save->data[i] & (1 << j)) >> j) + '0';
		}
	}
	
	// 翻译save->data的末尾不足8位部分
	if(save->len_bit % 8 != 0) {
		for(int i = 7; i >= 8 - save->len_bit % 8; i--) {
			s[count++] = ((save->data[save->len_bit / 8] & (1 << i)) >> i) + '0';
		}
	}
	s[count] = '\0';
	return s;
}



// 5.其他（供其他函数调用的basic函数等）
// 注：basic函数不做可靠的错误检查，仅做部分错误反馈，尽量不要手动调用
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
   
   注：为浅拷贝，若data区存在指针，可能会导致double-free，调用时小心
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
   
   注：若节点p为NULL，返回值也为NULL，请注意与malloc失败的区分！
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

/* （basic）HFM函数专用的qsort比较函数
   返回值：y权重大时返回正数，x权重大时返回负数，相等时返回0
   用途：配合bitree_HFM_inttree函数，供qsort调用
   
   注：作为cmp导入qsort将按权重降序排列
*/
int bitree_basic_HFM_cmp(const void *a, const void *b) {
	binode *x, *y;
	x = *(binode**)a;
	y = *(binode**)b;
	int x1, y1;
	if(x->weight < y->weight || (x->weight==y->weight && x->letter < y->letter)) {
		return 1;
	} else return -1;
}

/* （basic）根据哈夫曼树tree，利用path数组，递归填写password字符串数组
   返回值：1（失败返回-1）
*/
int bitree_basic_HFM_gencode(char **password, char *path, int path_len, binode *p) {
	if(p->left==NULL && p->right==NULL) {
		char *t = (char*)malloc(path_len+1);// +1用来存放末尾'\0'
		if(!t) {
			if (bitree_error_output) printf("[Memory exceeded] There is not enough space to create the HFM password list.\n");
			return -1;
		}
		path[path_len] = 0;// 保险起见手动添加结束符
		strcpy(t, path);
		password[p->letter] = t;
		return 1;
	}
	if(p->left!=NULL) {
		path[path_len] = '0';
		if(bitree_basic_HFM_gencode(password, path, path_len+1, p->left)==-1) {
			return -1;
		}
		path[path_len] = 0;
	}
	if(p->right!=NULL) {
		path[path_len] = '1';
		if(bitree_basic_HFM_gencode(password, path, path_len+1, p->right)==-1) {
			return -1;
		}
		path[path_len] = 0;
	}
	return 1;
}





