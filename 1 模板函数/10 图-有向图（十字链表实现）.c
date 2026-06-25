/* 数据结构-图-有向图（十字链表结构） 函数实现
基本信息类型：intdgraph（directed graph）
点节点类型：dgv_node（directed graph vertex node）
边节点类型：dge_node（directed graph edge node）

注：
1.若同时建立多种有向图，需建立多种node变量类型 和 多个基本信息变量（并设定平行的不同的函数名称）
（即：一种基本信息结构体、多种node、多套函数）
2.十字链表：每个顶点挂有in、out两条边链；每条边记录其 from、to 节点，及其 from节点的下一条“出边”、to节点的下一条“入边”
3.不考虑自环化、平行多边情况；兼容非强联通图（udgraph_DFS、udgraph_BFS除外）
4.对于顶点 or 边的其他信息：手动建立1 or 2维数组存储
5.相较于无向图，有向图没有“最小生成树”的说法。其替代品是“最小树形图”（朱刘算法），这版模板暂时没写
*/

/* 手动DIY目录：

*/

/* 错误说明：
[Invalid function argument]	非法函数参数
[Memory exceeded]			MLE
[Input error]				读取输入失败
[Illegal operation]			非法操作
[Other errors]				其他（非连通图）
*/



#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>



// 点（vertex）和边（edge）节点结构
typedef struct dge_Node dge_node;// 由于两个节点相互嵌套定义，需要先做前向声明再填充内容
typedef struct dgv_Node {
	int n;							// 点序号
	dge_node *firstin;				// 该点的第一个入边
	dge_node *firstout;				// 该点的第一个出边
	struct dgv_Node *link1;			// 链表中前一个点
	struct dgv_Node *link2;			// 链表中后一个点
	int mark;						// 用作遍历标记（默认写0）
} dgv_node;
struct dge_Node {
	int weight;						// 权值（默认为1）
	dgv_node *from;					// 箭头尾巴连接的节点
	dgv_node *to;					// 箭头指向的节点
	struct dge_Node *from_next_out;		// from节点的下一条“出边”
	struct dge_Node *to_next_in;		// to节点的下一条“入边”
	int mark;						// 用作遍历标记（默认写0）
};

// 基本信息结构体
typedef struct {
	dgv_node *head;					// 点链表头结点
	dgv_node *tail;					// 点链表尾结点
	int size_v;						// 点总数
	int size_e;						// 边总数
} intdgraph;

// 最短路径问题函数输出结构体
typedef struct {
	dgv_node *start;				// 路径起始顶点
	dgv_node *end;					// 路径终止顶点
	int length;						// 路径长度
	int *road;						// 路径顶点序号序列
} intdgraph_shortest_road;



// 全局变量dgraph_error_output：控制是否打印错误信息（默认打印）
static int dgraph_error_output = 1;



// 1.图的结构控制
intdgraph* dgraph_intgraph();												// 创建图（空图）
dge_node* dgraph_addedge(intdgraph *graph, int from, int to, int weight);	// 添加一条边（若顶点序号是新的，就同步添加点节点）
int dgraph_addedges(intdgraph *graph, int e, int (*a)[3]);					// 批量添加边（及对应节点）
int dgraph_deleteedge(intdgraph *graph, dge_node *e);						// 删除地址为e的边（及删边导致的独立节点）
int dgraph_deletevertex(intdgraph *graph, dgv_node *v);						// 删除地址为v的顶点
int dgraph_deletegraph(intdgraph *graph);									// 删除全部节点，删除图信息
intdgraph* dgraph_copy(intdgraph *graph1);									// 复制图graph1

// 2.节点、图相关信息获取
dgv_node* dgraph_getplace_v(intdgraph *graph, int n);						// 获取图graph中序号为n的顶点地址
dge_node* dgraph_getplace_e(intdgraph *graph, int from, int to);			// 获取图graph中 从序号from指向序号to的 边的地址
dgv_node** dgraph_DFS(intdgraph *graph, dgv_node *start, int option);		// 深度优先遍历（DFS）（依据遍历起始点start，输出顶点地址数组）
dgv_node** dgraph_BFS(intdgraph *graph, dgv_node *start, int option);		// 广度优先遍历（BFS）（依据遍历起始点start，输出顶点地址数组）
dge_node** dgraph_get_edgelist(intdgraph *graph);							// 获取边节点数组（按权值升序排列）
intdgraph_shortest_road* dgraph_shortest_path_Dijkstra(intdgraph *graph, dgv_node *start);
																			// 求最短路径（Dijkstra算法，计算从start到所有其他节点的最短路径）
intdgraph_shortest_road** dgraph_shortest_path_Floyd(intdgraph *graph);		// 求最短路径（Floyd算法，计算图中任意两点的最短路径）

// 3.图的数据控制
void dgraph_set_mark_v(intdgraph *graph);									// 所有顶点的mark字段归0
void dgraph_set_mark_e(intdgraph *graph);									// 所有边的mark字段归0

// 4.其他（供其他函数调用的basic函数等）
void dgraph_set_error_output(int enable);									// 控制是否可视化打印错误信息
static dgv_node* dgraph_basic_copyvnode(dgv_node *dest, dgv_node *src);		// （basic）将顶点节点src的数据复制给顶点节点dest
static dge_node* dgraph_basic_copyenode(dge_node *dest, dge_node *src);		// （basic）将边节点src的数据复制给边节点dest
int dgraph_basic_DFS_cmp(const void *a, const void *b);						// （basic）DFS函数专用的qsort比较函数
int dgraph_basic_BFS_cmp(const void *a, const void *b);						// （basic）BFS函数专用的qsort比较函数
int dgraph_basic_edge_cmp(const void *a, const void *b);					// （basic）生成MST函数、获取边节点数组函数专用的qsort比较函数



int main() {
	
	
	
}



// 自定义函数

// 1.图的结构控制
/* 创建图（空图）
   返回值：图信息结构体指针（失败返回NULL）
*/
intdgraph* dgraph_intgraph() {// 顶点数，边数，边数组
	intdgraph *graph = (intdgraph*)malloc(sizeof(intdgraph));
	if(!graph) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space for the new dgraph.\n");
		return NULL;
	}
	graph->head = NULL;
	graph->tail = NULL;
	graph->size_v = 0;
	graph->size_e = 0;
	return graph;
}

/* 添加一条边（若顶点序号是新的，就同步添加点节点）
   返回值：新边节点地址（失败返回NULL）
   
   函数行为约定：from、to填写顶点序号，weight可选（默认写1）
*/
dge_node* dgraph_addedge(intdgraph *graph, int from, int to, int weight) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return NULL;
	}
	if(from == to) {
	    if(dgraph_error_output) printf("[Illegal operation] Self-loop is not allowed.\n");
		return NULL;
	}
	
	dge_node *e = (dge_node*)malloc(sizeof(dge_node));
	if(!e) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
		return NULL;
	}
	e->mark = 0;
	e->weight = weight;
	
	
	// 特殊处理空图的边插入
	if(graph->head == NULL) {// 空图
		dgv_node *x1 = (dgv_node*)malloc(sizeof(dgv_node));
		if(!x1) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
			free(e);
			return NULL;
		}
		dgv_node *x2 = (dgv_node*)malloc(sizeof(dgv_node));
		if(!x2) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
			free(e);
			free(x1);
			return NULL;
		}
		graph->head = x1;
		x1->link1 = NULL;
		x1->link2 = x2;
		x2->link1 = x1;
		x2->link2 = NULL;
		graph->tail = x2;
		x1->n = from;
		x2->n = to;
		x1->firstin = x2->firstout = NULL;
		x1->firstout = x2->firstin = e;
		x1->mark = x2->mark = 0;
		e->from = x1;
		e->to = x2;
		e->from_next_out = e->to_next_in = NULL;
		graph->size_e = 1;
		graph->size_v = 2;
		return e;
	}
	dgv_node *t = graph->head, *t1 = NULL, *t2 = NULL;
	
	// 扫描from、to在不在顶点链里，并获取其指针
	while(t != NULL) {
		if(t->n == from) {// v1顶点已在链表中
			t1 = t;
		} else if(t->n == to) {// v2顶点已在链表中
			t2 = t;
		}
		t = t->link2;
	}
	
	// 分四类处理边添加（方便空间不足时的重置操作）
	if(t1 == NULL && t2 == NULL) {// 都不在顶点链中
		dgv_node *x1 = (dgv_node*)malloc(sizeof(dgv_node));
		if(!x1) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
			free(e);
			return NULL;
		}
		dgv_node *x2 = (dgv_node*)malloc(sizeof(dgv_node));
		if(!x2) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
			free(e);
			free(x1);
			return NULL;
		}
		graph->tail->link2 = x1;
		x1->link1 = graph->tail;
		x1->link2 = x2;
		x2->link1 = x1;
		x2->link2 = NULL;
		graph->tail = x2;
		x1->firstin = x2->firstout = NULL;
		x1->firstout = x2->firstin = e;
		x1->n = from;
		x2->n = to;
		x1->mark = x2->mark = 0;
		e->from = x1;
		e->to = x2;
		e->from_next_out = e->to_next_in = NULL;
		graph->size_e++;
		graph->size_v += 2;
		return e;
	} else if(t1 == NULL) {// 仅v1不在顶点链中
		dgv_node *x1 = (dgv_node*)malloc(sizeof(dgv_node));
		if(!x1) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
			free(e);
			return NULL;
		}
		graph->tail->link2 = x1;
		x1->link1 = graph->tail;
		graph->tail = x1;
		x1->link2 = NULL;
		x1->n = from;
		x1->firstin = NULL;
		x1->firstout = e;
		x1->mark = 0;
		e->from = x1;
		e->from_next_out = NULL;
		e->to = t2;
		e->to_next_in = t2->firstin;
		t2->firstin = e;
		graph->size_e++;
		graph->size_v++;
		return e;
	} else if(t2 == NULL) {// 仅v2不在顶点链中
		dgv_node *x2 = (dgv_node*)malloc(sizeof(dgv_node));
		if(!x2) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
			free(e);
			return NULL;
		}
		graph->tail->link2 = x2;
		x2->link1 = graph->tail;
		graph->tail = x2;
		x2->link2 = NULL;
		x2->n = to;
		x2->firstin = e;
		x2->firstout = NULL;
		x2->mark = 0;
		e->from = t1;
		e->from_next_out = t1->firstout;
		t1->firstout = e;
		e->to = x2;
		e->to_next_in = NULL;
		graph->size_e++;
		graph->size_v++;
		return e;
	} else {// 都在顶点链中
		e->from = t1;
		e->from_next_out = t1->firstout;
		t1->firstout = e;
		e->to = t2;
		e->to_next_in = t2->firstin;
		t2->firstin = e;
		graph->size_e++;
		return e;
	}
}

/* 批量添加边（及对应节点）
   返回值：成功添加边的个数（错误返回-1）
   
   函数行为约定： 输入的“边信息”为二维数组int (*)[3]（第一维大小取决于e的大小），描述每条边的from、to顶点序号以及权值（没有就都写1）
*/
int dgraph_addedges(intdgraph *graph, int e, int (*a)[3]) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return -1;
	}
	
	int count = 0;
	for(int i = 0; i < e; i++) {
		if(dgraph_addedge(graph, a[i][0], a[i][1], a[i][2]) == NULL) {
			return count;// 若从这里返回，将触发dgraph_addedge的错误报告机制
		}
		count++;
	}
	return e;
}

/* 删除地址为e的边（及删边导致的独立节点）
   返回值：1（失败返回-1）
*/
int dgraph_deleteedge(intdgraph *graph, dge_node *e) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return -1;
	}
	if(e == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dge_node pointer is NULL.\n");
		return -1;
	}
	
	dgv_node *from, *to;// 边e的两个顶点a、b
	from = e->from;
	to = e->to;
	if(from->firstin == NULL && (from->firstout == NULL || from->firstout->from_next_out == NULL)) {
		// 如果from没有入边 且 出边至多一条：同步删掉from（此处利用了短路求值）
		// 删掉节点from
		if(graph->size_v == 1) {
			graph->head = graph->tail = NULL;
		} else if(graph->head == from) {
			graph->head = from->link2;
			from->link2->link1 = NULL;
		} else if(graph->tail == from) {
			graph->tail = from->link1;
			from->link1->link2 = NULL;
		} else {
			from->link1->link2 = from->link2;
			from->link2->link1 = from->link1;
		}
		free(from);
		graph->size_v--;
	} else {// 从from的out边链中把e删除
		if(from->firstout == e) {
			from->firstout = e->from_next_out;
		} else {
			for(dge_node *t = from->firstout; t != NULL; t = t->from_next_out) {
				if(t->from_next_out == e) {
					t->from_next_out = e->from_next_out;
					break;
				}
			}
			
		}
	}
	if((to->firstin == NULL || to->firstin->to_next_in == NULL) && (to->firstout == NULL)) {
		// 如果to没有出边 且 入边至多一条：同步删掉to（此处利用了短路求值）
		// 删掉节点to
		if(graph->size_v == 1) {
			graph->head = graph->tail = NULL;
		} else if(graph->head == to) {
			graph->head = to->link2;
			to->link2->link1 = NULL;
		} else if(graph->tail == to) {
			graph->tail = to->link1;
			to->link1->link2 = NULL;
		} else {
			to->link1->link2 = to->link2;
			to->link2->link1 = to->link1;
		}
		free(to);
		graph->size_v--;
	} else {// 从to的in边链中把e删除
		if(to->firstin == e) {
			to->firstin = e->to_next_in;
		} else {
			for(dge_node *t = to->firstin; t != NULL; t = t->to_next_in) {
				if(t->to_next_in == e) {
					t->to_next_in = e->to_next_in;
					break;
				}
			}
		}
	}
	
	graph->size_e--;
	free(e);
	return 1;
}

/* 删除地址为v的顶点
   返回值：1（失败返回-1）
*/
int dgraph_deletevertex(intdgraph *graph, dgv_node *v) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return -1;
	}
	if(v == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgv_node pointer is NULL.\n");
		return -1;
	}
	
	if(v->firstin == NULL && v->firstout == NULL) {// 单独处理“无边顶点”（只是比较安全的写法，理论上不可能出现孤立点）
		if(graph->size_v == 1) {
			graph->head = graph->tail = NULL;
		} else if(graph->head == v) {
			graph->head = v->link2;
			v->link2->link1 = NULL;
		} else if(graph->tail == v) {
			graph->tail = v->link1;
			v->link1->link2 = NULL;
		} else {
			v->link1->link2 = v->link2;
			v->link2->link1 = v->link1;
		}
		free(v);
		graph->size_v--;
		return 1;
	}
	
	for(dge_node *t = v->firstin, *t0; t != NULL;) {
		t0 = t;
		t = t->to_next_in;
		dgraph_deleteedge(graph, t0);
	}
	for(dge_node *t = v->firstout, *t0; t != NULL;) {// 删到v的最后一个出边时会自动删掉顶点v
		t0 = t;
		t = t->from_next_out;
		dgraph_deleteedge(graph, t0);
	}
	return 1;
}

/* 删除全部节点，删除图信息
   返回值：1（失败返回-1）
*/
int dgraph_deletegraph(intdgraph *graph) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return -1;
	}
	
	while(graph->head != NULL) {
		dgraph_deletevertex(graph, graph->head);
	}
	free(graph);
	return 1;
}

/* 复制图graph1
   返回值：复制出来的图的信息结构体指针（失败返回NULL）
   
   函数行为约定：为简便，暂时不保证graph2和graph1的边链顺序一致。不会影响任何图性质的运用，但可能导致在某些刁钻题目出错。注意！
*/
intdgraph* dgraph_copy(intdgraph *graph1) {
	if(graph1 == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return NULL;
	}
	
	intdgraph *graph2 = (intdgraph*)malloc(sizeof(intdgraph));
	if(!graph2) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space for the new dgraph.\n");
		return NULL;
	}
	graph2->size_e = graph2->size_v = 0;
	graph2->head = graph2->tail = NULL;
	
	// 单独处理空图情况
	if(graph1->head == NULL) {
		return graph2;
	}
	
	// 复制顶点节点 并 构建新旧顶点映射表
	// 注：考虑到顶点序号可能不连续且很大，这里不拿顶点序号直接作为索引
	typedef struct {
		dgv_node *a1;
		dgv_node *a2;
	} int0;
	int0 *list = (int0*)malloc(sizeof(int0) * graph1->size_v);
	int count0 = 0;// 记录当前list中信息个数
	if(!list) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to copy the dgraph.\n");
		free(graph2);
		return NULL;
	}
	dgv_node *l0 = NULL;
	for(dgv_node *t = graph1->head; t != NULL; t = t->link2) {
		dgv_node *k = (dgv_node*)malloc(sizeof(dgv_node));		
		if(!k) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space for the new dgraph.\n");
			dgraph_deletegraph(graph2);
			free(list);
			return NULL;
		}
		k->firstin = k->firstout = NULL;
		k->link1 = l0;
		k->link2 = NULL;
		if(l0 == NULL) {
			graph2->head = k;
		} else {
			l0->link2 = k;
		}
		graph2->tail = k;
		dgraph_basic_copyvnode(k, t);
		l0 = k;
		list[graph2->size_v].a1 = t;// 记录映射关系
		list[graph2->size_v++].a2 = k;// 记录映射关系，并实时更新图大小（始终保持图结构合法，便于回滚操作）
	}
	
	// 遍历所有顶点的入边序列，并复制（都仅遍历入边，防止重复）
	// 注：为了方便操作，在复制边链时全部采用“头插法”，但将导致新边链和原边链顺序相反（并不影响图结构），遇到刁钻的题目可能会出错
	for(dgv_node *t0 = graph1->head; t0 != NULL; t0 = t0->link2) {// 遍历顶点（t0）
		for(dge_node *t = t0->firstin; t != NULL; t = t->to_next_in) {// 遍历入边（t）
			dge_node *k = (dge_node*)malloc(sizeof(dge_node));
			if(!k) {
				if (dgraph_error_output) printf("[Memory exceeded] There is not enough space for the new dgraph.\n");
				dgraph_deletegraph(graph2);// 对已复制的边和顶点进行回滚
				free(list);
				return NULL;
			}
			dgraph_basic_copyenode(k, t);
			// 运用映射表，将边k头插入graph2中x1的出边链 和 x2的入边链
			dgv_node *x1, *x2;
			for(int i = 0, sign = 0; i < graph2->size_v && sign < 2; i++) {
				if(list[i].a1 == t->from) {
					x1 = list[i].a2;
					sign++;
				} else if(list[i].a1 == t->to) {
					x2 = list[i].a2;
					sign++;
				}
			}
			k->from = x1;
			k->to = x2;
			k->from_next_out = x1->firstout;
			x1->firstout = k;
			k->to_next_in = x2->firstin;
			x2->firstin = k;
			graph2->size_e++;
		}
	}
	free(list);
	return graph2;
}



// 2.节点、图相关信息获取
/* 获取图graph中序号为n的顶点地址
   返回值：目标顶点节点指针（找不到返回NULL）
*/
dgv_node* dgraph_getplace_v(intdgraph *graph, int n) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return NULL;
	}
	
	for(dgv_node *t = graph->head; t != NULL; t = t->link2) {
		if(t->n == n) {
			return t;
		}
	}
	return NULL;
}

/* 获取图graph中 从序号from指向序号to的 边的地址
   返回值：目标边节点指针（找不到返回NULL）
*/
dge_node* dgraph_getplace_e(intdgraph *graph, int from, int to) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return NULL;
	}
	
	dgv_node *t0 = dgraph_getplace_v(graph, from);
	if(t0 == NULL) {
		return NULL;
	}
	for(dge_node *t = t0->firstout; t != NULL; t = t->from_next_out) {// 遍历顶点t0的出边链
		if(t->to->n == to) {
			return t;
		}
	}
	return NULL;
}

/* 深度优先遍历（DFS）（依据遍历起始点start，输出顶点地址数组）
   返回值：顶点地址数组指针（错误返回NULL）
   
   函数行为约定：graph必须是“强连通图”，即从任意一点可以走到所有其他顶点
   
   option：0：仅完成基础DFS遍历规则 1：相同层级的节点按照“序号从小到大”访问，但时间复杂度较高
*/
dgv_node** dgraph_DFS(intdgraph *graph, dgv_node *start, int option) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return NULL;
	}
	if(start == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgv_node pointer is NULL.\n");
		return NULL;
	}
	if(option != 0 && option != 1) {
		if(dgraph_error_output) printf("[Invalid function argument] The \"option\" used to select option should be 0 or 1.\n");
		return NULL;
	}
	
	dgraph_set_mark_v(graph);
	dgv_node **list = (dgv_node**)malloc(sizeof(dgv_node*) * graph->size_v);// 输出空间
	if(!list) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to do the DFS.\n");
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		list[i] = NULL;
	}
	int count = 0;// 实时记录list中数据个数
	
	dgv_node **stuck = (dgv_node**)malloc(sizeof(dgv_node*) * graph->size_v);// 递归栈
	if(!stuck) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to do the DFS.\n");
		free(list);
		return NULL;
	}
	int top = -1;// 实时记录栈顶位置
	
	dgv_node **temp = (dgv_node**)malloc(sizeof(dgv_node*) * graph->size_v);// 用于临时存储当前节点的相邻节点
	if(!temp) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to do the DFS.\n");
		free(list);
		free(stuck);
		return NULL;
	}
	int count1 = 0;
	
	stuck[++top] = start;// 遍历起点
	start->mark = 1;
	while(top >= 0) {
		// 先把栈顶元素记录并出栈
		dgv_node *t0 = stuck[top--];
		list[count++] = t0;
		
		// 再遍历t0的所有出边所连相邻节点，将其中mark=0的部分标记访问并入栈
		count1 = 0;
		for(dge_node *t = t0->firstout; t != NULL; t = t->from_next_out) {
			if(t->to->mark == 0) {
				temp[count1++] = t->to;
				t->to->mark = 1;
			}
		}
		// 如果option=1，需要控制按序号从大到小依次入栈
		if(option) {
			qsort(temp, count1, sizeof(dgv_node*), dgraph_basic_DFS_cmp);
		}
		for(int i = 0; i < count1; i++) {
			stuck[++top] = temp[i];
		}
	}
	// 遍历结束时：top = -1
	
	free(temp);
	free(stuck);
	dgraph_set_mark_v(graph);
	return list;
}

/* 广度优先遍历（BFS）（依据遍历起始点start，输出顶点地址数组）
   返回值：顶点地址数组指针（错误返回NULL）
   
   函数行为约定：graph必须是“强连通图”，即从任意一点可以走到所有其他顶点
   
   option：0：仅完成基础BFS遍历规则 1：相同层级的节点按照“序号从小到大”访问，但时间复杂度较高
*/
dgv_node** dgraph_BFS(intdgraph *graph, dgv_node *start, int option) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return NULL;
	}
	if(start == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgv_node pointer is NULL.\n");
		return NULL;
	}
	if(option != 0 && option != 1) {
		if(dgraph_error_output) printf("[Invalid function argument] The \"option\" used to select option should be 0 or 1.\n");
		return NULL;
	}
	
	dgraph_set_mark_v(graph);
	dgv_node **list = (dgv_node**)malloc(sizeof(dgv_node*) * graph->size_v);// 输出空间
	if(!list) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to do the BFS.\n");
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		list[i] = NULL;
	}
	int count = 0;// 实时记录list中数据个数
	
	dgv_node **queue = (dgv_node**)malloc(sizeof(dgv_node*) * graph->size_v);// 队列
	if(!queue) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to do the BFS.\n");
		free(list);
		return NULL;
	}
	int head = 0, tail = -1;// 实时记录队首、尾位置
	
	dgv_node **temp = (dgv_node**)malloc(sizeof(dgv_node*) * graph->size_v);// 用于临时存储当前节点的相邻节点
	if(!temp) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to do the BFS.\n");
		free(list);
		free(queue);
		return NULL;
	}
	int count1 = 0;
	
	queue[++tail] = start;// 遍历起点
	start->mark = 1;
	while(head <= tail) {
		// 先把队首元素记录并出栈
		dgv_node *t0 = queue[head++];
		list[count++] = t0;
		
		// 再遍历t0的所有相邻节点，将其中mark=0的部分标记访问并入队
		count1 = 0;
		for(dge_node *t = t0->firstout; t != NULL; t = t->from_next_out) {
			if(t->to->mark == 0) {
				temp[count1++] = t->to;
				t->to->mark = 1;
			}
		}
		// 如果option=1，需要控制按序号从小到大依次入队
		if(option) {
			qsort(temp, count1, sizeof(dgv_node*), dgraph_basic_BFS_cmp);
		}
		for(int i = 0; i < count1; i++) {
			queue[++tail] = temp[i];
		}
	}
	
	free(temp);
	free(queue);
	dgraph_set_mark_v(graph);
	return list;
}

/* 获取边节点数组（依次按weight、from顶点序号、to顶点序号升序排列）
   返回值：边地址数组指针（错误返回NULL）
*/
dge_node** dgraph_get_edgelist(intdgraph *graph) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return NULL;
	}
	if(graph->head == NULL) {
		if(dgraph_error_output) printf("[Illegal operation] The target dgraph is empty.\n");
		return NULL;
	}
	
	dge_node **list = (dge_node**)malloc(sizeof(dge_node*) * graph->size_e);
	if(!list) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space for the edgelist.\n");
		return NULL;
	}
	int count = 0;
	
	for(dgv_node *t0 = graph->head; t0 != NULL; t0 = t0->link2) {
		for(dge_node *t = t0->firstin; t != NULL; t = t->to_next_in) {
			list[count++] = t;
		}
	}
	qsort(list, graph->size_e, sizeof(dge_node*), dgraph_basic_edge_cmp);
	return list;
}

/* 求最短路径（Dijkstra算法，计算从start到所有其他节点的最短路径）
   返回值：intdgraph_shortest_road数组（该数组的序号与实际顶点序号无关）
   
   函数行为约定：
   1.要求所有edge的weight非负
   2.输出的结果数组包含从start到start的路径，仅为易于操作
   3.若某点不可达，距离记为 INT_MAX
   
   注：更适合稀疏图
   
   原理：
   distance数组：记录各顶点到start顶点的最小加权路径长度，起点为0，未连接为∞
   prev数组：记录形式序号i的顶点在路径中的前一顶点地址
   visited数组：标记“已经计算完毕”的顶点（鉴于mark字段被占用了）
   （另：）映射数组list：记录形式序号（1~graph->size_v）所代表的顶点地址
   每次循环，从当前顶点出发更新distance、prev数组，并找到distance最小的顶点，进行visited标记
*/
intdgraph_shortest_road* dgraph_shortest_path_Dijkstra(intdgraph *graph, dgv_node *start) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return NULL;
	}
	if(graph->head == NULL) {
		if(dgraph_error_output) printf("[Illegal operation] The target dgraph is empty.\n");
		return NULL;
	}
	if(start == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgv_node pointer is NULL.\n");
		return NULL;
	}
	
	// malloc出 list、distance、prev、visited、out 5个数组
	dgv_node **list = (dgv_node**)malloc(sizeof(dgv_node*) * graph->size_v);// 临时顶点地址索引（0~size_v-1），同步记录在mark中
	if(!list) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		return NULL;
	}
	dgv_node *t = graph->head;
	for(int i = 0; t != NULL; i++, t = t->link2) {
		list[i] = t;
		t->mark = i;
	}
	
	typedef struct {
		int sum;	// 总加权路径长度
		int n;		// 总途经边数
	} intdistance;
	intdistance *distance = (intdistance*)malloc(sizeof(intdistance) * graph->size_v);
	if(!distance) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		dgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		distance[i].sum = INT_MAX;
		distance[i].n = 0;
	}
	
	dgv_node **prev = (dgv_node**)malloc(sizeof(dgv_node*) * graph->size_v);
	if(!prev) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		free(distance);
		dgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		prev[i] = NULL;
	}
	
	int *visited = (int*)malloc(sizeof(int) * graph->size_v);
	if(!visited) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		free(distance);
		free(prev);
		dgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		visited[i] = 0;
	}
	
	intdgraph_shortest_road *out = (intdgraph_shortest_road*)malloc(sizeof(intdgraph_shortest_road) * graph->size_v);
	if(!out) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		free(distance);
		free(prev);
		free(visited);
		dgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		out[i].start = start;
		out[i].end = NULL;
		out[i].length = INT_MAX;
		out[i].road = NULL;
	}
	int sum = 0;// 实时记录已经计算完的节点数，用作程序终止指标
	
	// 核心计算模块儿
	t = start;
	distance[start->mark].sum = 0;
	distance[start->mark].n = 0;
	prev[start->mark] = start;
	visited[start->mark] = 1;
	sum++;
	while(sum < graph->size_v) {
		// 遍历t的所有（未遍历完成的）相邻顶点，更新distance
		for(dge_node *t1 = t->firstout; t1 != NULL; t1 = t1->from_next_out) {
			if(visited[t1->to->mark] == 0) {// t1->to->mark为当前边另一顶点的形式序号
				if(distance[t->mark].sum + t1->weight < distance[t1->to->mark].sum) {
					distance[t1->to->mark].sum = distance[t->mark].sum + t1->weight;
					distance[t1->to->mark].n = distance[t->mark].n + 1;
					prev[t1->to->mark] = t;
				}
			}
		}
		// 找到“1个当前距离start最近的顶点”标记为计算完毕
		int temp, min = INT_MAX;
		for(int i = 0; i < graph->size_v; i++) {
			if(distance[i].sum < min && visited[i] == 0) {
				temp = i;
				min = distance[i].sum;
			}
		}
		if (min == INT_MAX) break;// 此时：剩余的所有顶点均不可达
		visited[temp] = 1;
		sum++;
		// 填写out放到最后再统一完成，方便回滚
		
		t = list[temp];// 定位到下一个顶点地址
	}
	
	// 填写out数组
	for(int i = 0; i < graph->size_v; i++) {// 填写结束于形式序号i的路径
		out[i].end = list[i];
		out[i].length = distance[i].sum;
		if (distance[i].sum == INT_MAX) {// 处理非连通图造成的不可达顶点
		    out[i].road = NULL;
		    continue;
		}
		out[i].road = (int*)malloc(sizeof(int) * (distance[i].n + 1));
		if(!out[i].road) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
			// 回滚所有已分配road
			for(int j = 0; j < i; j++) {
				free(out[j].road);
			}
			free(list);
			free(distance);
			free(prev);
			free(visited);
			free(out);
			return NULL;
		}
		dgv_node **stack = (dgv_node**)malloc(sizeof(dgv_node*) * (distance[i].n + 1));
		if(!stack) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
			// 回滚所有已分配road
			for(int j = 0; j <= i; j++) {
				free(out[j].road);
			}
			free(list);
			free(distance);
			free(prev);
			free(visited);
			free(out);
			return NULL;
		}
		int top = -1;
		int k1 = i;
		stack[++top] = list[k1];// 尾顶点
		while(k1 != start->mark) {
			stack[++top] = prev[k1];
			k1 = prev[k1]->mark;
		}
		int count1 = 0;// 实时记录road中元素个数
		while(top >= 0) {
			out[i].road[count1++] = stack[top--]->n;// 这时用的是实际的真实顶点序号n
		}
		free(stack);
	}
	
	free(list);
	free(distance);
	free(prev);
	free(visited);
	dgraph_set_mark_v(graph);
	return out;
}

/* 求最短路径（Floyd算法，计算图中任意两点的最短路径）
   返回值：二维intdgraph_shortest_road数组（该数组的序号与实际顶点序号无关）
   
   函数行为约定：若某点不可达，距离记为 INT_MAX
   
   注：更适合稠密图
   
   原理：
   dist[i][j]：从顶点i到顶点j的当前最短距离
   path[i][j]：在i→j的最短路径上，顶点j的前驱顶点的形式序号（用于之后重建路径）
   
*/
intdgraph_shortest_road** dgraph_shortest_path_Floyd(intdgraph *graph) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return NULL;
	}
	if(graph->head == NULL) {
		if(dgraph_error_output) printf("[Illegal operation] The target dgraph is empty.\n");
		return NULL;
	}
	
	// malloc出 list、distance（二维）、path（二维）、out（二维） 4个数组（由于dp方式的不同，不需要visited数组）
	dgv_node **list = (dgv_node**)malloc(sizeof(dgv_node*) * graph->size_v);// 临时顶点地址索引（0~size_v-1），同步记录在mark中
	if(!list) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		return NULL;
	}
	dgv_node *t = graph->head;
	for(int i = 0; t != NULL; i++, t = t->link2) {
		list[i] = t;
		t->mark = i;
	}
	
	typedef struct {
		int sum;	// 总加权路径长度
		int n;		// 总途径边数
	} intdistance;
	intdistance **distance = (intdistance**)malloc(sizeof(intdistance*) * graph->size_v);
	if(!distance) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		dgraph_set_mark_v(graph);
		return NULL;
	}
	int **path = (int**)malloc(sizeof(int*) * graph->size_v);
	if(!path) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		free(distance);
		dgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		distance[i] = (intdistance*)malloc(sizeof(intdistance) * graph->size_v);
		if(!distance[i]) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
			for(int j = 0; j < i; j++) {
				free(distance[j]);
			}
			free(list);
			free(distance);
			dgraph_set_mark_v(graph);
			return NULL;
		}
	}
	for(int i = 0; i < graph->size_v; i++) {
		path[i] = (int*)malloc(sizeof(int) * graph->size_v);
		if(!path[i]) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
			for(int j = 0; j < i; j++) {
				free(path[j]);
			}
			free(list);
			for(int j = 0; j < graph->size_v; j++) {
				free(distance[j]);
			}
			free(distance);
			dgraph_set_mark_v(graph);
			return NULL;
		}
	}
	
	intdgraph_shortest_road **out = (intdgraph_shortest_road**)malloc(sizeof(intdgraph_shortest_road*) * graph->size_v);
	if(!out) {
		if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		for(int j = 0; j < graph->size_v; j++) {
			free(distance[j]);
		}
		free(distance);
		for(int j = 0; j < graph->size_v; j++) {
			free(path[j]);
		}
		free(path);
		dgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		out[i] = (intdgraph_shortest_road*)malloc(sizeof(intdgraph_shortest_road) * graph->size_v);
		if(!out[i]) {
			if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
			for(int j = 0; j < i; j++) {
				free(out[j]);
			}
			free(list);
			for(int j = 0; j < graph->size_v; j++) {
				free(distance[j]);
			}
			free(distance);
			for(int j = 0; j < graph->size_v; j++) {
				free(path[j]);
			}
			free(path);
			free(out);
			dgraph_set_mark_v(graph);
			return NULL;
		}
		for(int j = 0; j < graph->size_v; j++) {
			out[i][j].start = out[i][j].end = NULL;
			out[i][j].road = NULL;
			out[i][j].length = INT_MAX;
		}
	}
	
	// 根据图信息，初始填写distance和path
	for(int i = 0; i < graph->size_v; i++) {
		for(int j = 0; j < graph->size_v; j++) {
			if(i == j) {
				distance[i][j].sum = 0;
				distance[i][j].n = 0;
			} else {
				distance[i][j].sum = INT_MAX;
				distance[i][j].n = 0;
			}
			path[i][j] = -1;
		}
	}
	for(dgv_node *t = graph->head; t != NULL; t = t->link2) {
		for(dge_node *t1 = t->firstin; t1 != NULL; t1 = t1->to_next_in) {
			distance[t1->from->mark][t->mark].sum = t1->weight;
			distance[t1->from->mark][t->mark].n = 1;
			path[t1->from->mark][t->mark] = t1->from->mark;
		}
	}
	
	// Floyd算法的核心三重循环（DP）：在仅可采用0~k作为途经点时，尝试dp优化从i到j的最短路径
	// 内层循环检查!=INT_MAX：利用短路求值，防止相加爆int
	// dp设计：
	for(int k = 0; k < graph->size_v; k++) {// 此时，仅0~k可作为途经点（循环前都是直接连线）
		for(int i = 0; i < graph->size_v; i++) {
			for(int j = 0; j < graph->size_v; j++) {
				// 进入此循环时，如果distance[i][k]、distance[k][j]不为INT_MAX，那必是在之前算出的，满足“仅采用0~k-1作为途经点
				if(distance[i][k].sum != INT_MAX && distance[k][j].sum != INT_MAX && distance[i][k].sum + distance[k][j].sum < distance[i][j].sum) {
					distance[i][j].sum = distance[i][k].sum + distance[k][j].sum;
					distance[i][j].n = distance[i][k].n + distance[k][j].n;
					path[i][j] = path[k][j];
				}
			}
		}
	}
	
	// 填写out数组
	for(int i = 0; i < graph->size_v; i++) {
		for(int j = 0; j < graph->size_v; j++) {
			out[i][j].start = list[i];
			out[i][j].end = list[j];
			out[i][j].length = distance[i][j].sum;
			if(distance[i][j].sum == INT_MAX) {
				out[i][j].road = NULL;
				continue;
			}
			if(i == j) {
				out[i][j].start = out[i][j].end = list[i];
				out[i][j].length = 0;
				out[i][j].road = NULL;
				continue;
			}
			
			int *stack = (int*)malloc(sizeof(int) * (distance[i][j].n + 1));
			if(!stack) {
				if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
				free(list);
				for(int k = 0; k < graph->size_v; k++) {
					free(distance[k]);
				}
				free(distance);
				for(int k = 0; k < graph->size_v; k++) {
					free(path[k]);
				}
				free(path);
				for(int k = 0; k < graph->size_v; k++) {
					free(out[k]);
				}
				free(out);
				dgraph_set_mark_v(graph);
				return NULL;
			}
			int top = -1;
			out[i][j].road = (int*)malloc(sizeof(int) * (distance[i][j].n + 1));
			if(!out[i][j].road) {
				if (dgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
				for(int ki = 0; ki <= i; ki++) {// out.road属于第三级malloc挂载，free是需要双循环
					for(int kj = 0; kj < graph->size_v; kj++) {
						if(ki < i || (ki == i && kj < j)) {
							free(out[ki][kj].road);
						}
					}
				}
				free(list);
				for(int k = 0; k < graph->size_v; k++) {
					free(distance[k]);
				}
				free(distance);
				for(int k = 0; k < graph->size_v; k++) {
					free(path[k]);
				}
				free(path);
				for(int k = 0; k < graph->size_v; k++) {
					free(out[k]);
				}
				free(out);
				free(stack);
				dgraph_set_mark_v(graph);
				return NULL;
			}
			int count1 = 0;// 实时记录road中元素数
			
			int k1 = j;
			stack[++top] = k1;
			while(k1 != i) {
				stack[++top] = k1 = path[i][k1];
			}
			while(top >= 0) {
				out[i][j].road[count1++] = list[stack[top--]]->n;// 这时用的是实际的真实顶点序号n
			}
			free(stack);
		}
	}
	
	free(list);
	for(int k = 0; k < graph->size_v; k++) {
	free(distance[k]);
	}
	free(distance);
	for(int k = 0; k < graph->size_v; k++) {
		free(path[k]);
	}
	free(path);
	dgraph_set_mark_v(graph);
	return out;
}



// 3.图的数据控制
/* 所有顶点的mark字段归0
*/
void dgraph_set_mark_v(intdgraph *graph) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return;
	}
	
	for(dgv_node *t = graph->head; t != NULL; t = t->link2) {
		t->mark = 0;
	}
	return;
}

/* 所有边的mark字段归0
*/
void dgraph_set_mark_e(intdgraph *graph) {
	if(graph == NULL) {
		if(dgraph_error_output) printf("[Invalid function argument] The target dgraph pointer is NULL.\n");
		return;
	}
	
	for(dgv_node *t = graph->head; t != NULL; t = t->link2) {
		for(dge_node *k = t->firstin; k != NULL; k = k->to_next_in) {
			k->mark = 0;
		}
	}
	return;
}



// 4.其他（供其他函数调用的basic函数等）
// 注：basic函数不做可靠的错误检查，仅做部分错误反馈，尽量不要手动调用
/* 控制是否可视化打印错误信息
*/
void dgraph_set_error_output(int enable) {// 1→打印；0→不打印
	dgraph_error_output = enable ? 1 : 0;
	return;
}

/* （basic）将顶点节点src的数据复制给顶点节点dest
   返回值：dest节点地址
   用途：配合dgraph_copy函数
*/
static dgv_node* dgraph_basic_copyvnode(dgv_node *dest, dgv_node *src) {
	dge_node *firstin = dest->firstin;
	dge_node *firstout = dest->firstout;
	dgv_node *link1 = dest->link1;
	dgv_node *link2 = dest->link2;
	*dest = *src;
	dest->firstin = firstin;
	dest->firstout = firstout;
	dest->link1 = link1;
	dest->link2 = link2;
	return dest;
}

/* （basic）将边节点src的数据复制给边节点dest
   返回值：dest节点地址
   用途：配合dgraph_copy函数
*/
static dge_node* dgraph_basic_copyenode(dge_node *dest, dge_node *src) {
	dgv_node *from = dest->from;
	dgv_node *to = dest->to;
	dge_node *fromlink = dest->from_next_out;
	dge_node *tolink = dest->to_next_in;
	*dest = *src;
	dest->from = from;
	dest->from_next_out = fromlink;
	dest->to = to;
	dest->to_next_in = tolink;
	return dest;
}

/* （basic）DFS函数专用的qsort比较函数
   返回值：y序号大时返回正数，x序号大时返回负数，相等时返回0
   用途：配合dgraph_DFS函数，供qsort调用，用于option=1情况
   
   注：作为cmp导入qsort将按顶点序号大小降序排列
*/
int dgraph_basic_DFS_cmp(const void *a, const void *b) {
	dgv_node *x = *(dgv_node**)a;
	dgv_node *y = *(dgv_node**)b;
	return y->n - x->n;
}

/* （basic）BFS函数专用的qsort比较函数
   返回值：x序号大时返回正数，y序号大时返回负数，相等时返回0
   用途：配合dgraph_BFS函数，供qsort调用，用于option=1情况
   
   注：作为cmp导入qsort将按顶点序号大小升序排列
*/
int dgraph_basic_BFS_cmp(const void *a, const void *b) {
	dgv_node *x = *(dgv_node**)a;
	dgv_node *y = *(dgv_node**)b;
	return x->n - y->n;
}

/* （basic）生成MST函数、获取边节点数组函数专用的qsort比较函数
   返回值：x序号大时返回正数，y序号大时返回负数，相等时返回0
   用途：配合dgraph_get_edgelist、dgraph_MST_Prim、dgraph_MST_Kruskal函数，供qsort调用
   
   注：作为cmp导入qsort将依次按weight、from顶点序号、to顶点序号升序排列
*/
int dgraph_basic_edge_cmp(const void *a, const void *b) {
	dge_node *x = *(dge_node**)a;
	dge_node *y = *(dge_node**)b;
	if (x->weight != y->weight) return x->weight - y->weight;
	if (x->from->n != y->from->n) return x->from->n - y->from->n;
	return x->to->n - y->to->n;
}


