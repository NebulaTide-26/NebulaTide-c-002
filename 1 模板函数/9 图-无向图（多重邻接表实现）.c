/* 数据结构-图-无向图（多重邻接表结构） 函数实现
基本信息类型：intudgraph（undirected graph）
点节点类型：udgv_node（undirected graph vertex node）
边节点类型：udge_node（undirected graph edge node）
若同时建立多种无向图，需建立多种node变量类型 和 多个基本信息变量（并设定平行的不同的函数名称）
（即：一种基本信息结构体、多种node、多套函数）
*/

/* 手动DIY目录：
DIY_data
DIY_fillnode_0(k)		
DIY_fillnode(k)			
DIY_printnode(k)		（若需调用bitree_printtree时）
DIY_print_len			（一般不用调）
DIY_search(k, x)		（若需调用bitree_search时）
*/

/* 错误说明：
[Invalid function argument]	非法函数参数
[Memory exceeded]			MLE
[Input error]				读取输入失败
[Illegal operation]			非法操作
[Other errors]				其他（）
*/



#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define DIY_data					int num
// 用于自定义节点的数据结构

#define DIY_fillnode_0(k)			k->num = 0
// 以k作为节点指针；用于填写成员赋值语句

#define DIY_fillnode(k)				scanf("%d", &k->num)
// 以k作为节点指针；不带“;”的scanf()语句

#define DIY_printnode(k)			printf("%d", k->num)
// 以k作为节点指针；不带“;”的printf()语句

#define DIY_print_len				8
// 打印每个节点时，占空间格数（含T/L/R:）

#define DIY_search(k, x)			k->num==x
// 以k作为节点指针；要能直接填写在if()的()里（注意：仅用于简单的节点数据。对于多数据节点，可能要手动修改bitree_search函数！！！！！！）


// 点（vertex）和边（edge）节点结构（具体根据需要填写DIY宏）
typedef struct udge_Node udge_node;// 由于两个节点相互嵌套定义，需要先做前向声明再填充内容
typedef struct udgv_Node {
//	DIY_data_vertex;
	int num;
	udge_node *firstedge;			// 该点的第一个边
	struct udgv_Node *link1;		// 链表中前一个点
	struct udgv_Node *link2;		// 链表中后一个点
} udgv_node;
struct udge_Node {
//	DIY_data_edge;
	int mark;						// ？？？
	udgv_node *v1;					// 一个端点
	udgv_node *v2;					// 另一个端点
	struct udge_Node *v1link;		// v1端点的下一个边
	struct udge_Node *v2link;		// v2端点的下一个边
};

// 基本信息结构体
typedef struct {
	udgv_node *head;				// 点链表头结点
	udgv_node *tail;				// 点链表尾结点
	int size_v;						// 点总数
	int size_e;						// 边总数
} intudgraph;



// 全局变量udgraph_error_output：控制是否打印错误信息（默认打印）
static int udgraph_error_output = 1;



// 1.树结构控制

// 2.节点位置、树相关信息获取

// 3.节点信息录入&输出

// 4.其他（供其他函数调用的basic函数等）



int main() {
    
}



// 自定义函数

// 1.图结构控制



