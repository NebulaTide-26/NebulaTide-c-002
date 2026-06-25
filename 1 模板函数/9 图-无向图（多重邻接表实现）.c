/* 数据结构-图-无向图（多重邻接表结构） 函数实现
基本信息类型：intudgraph（undirected graph）
点节点类型：udgv_node（undirected graph vertex node）
边节点类型：udge_node（undirected graph edge node）

注：
1.若同时建立多种无向图，需建立多种node变量类型 和 多个基本信息变量（并设定平行的不同的函数名称）
（即：一种基本信息结构体、多种node、多套函数）
2.多重邻接表：一个边节点同时处于AB两点的边链中
3.不考虑自环化、平行多边情况；兼容非联通图（udgraph_DFS、udgraph_BFS除外）
4.对于顶点 or 边的其他信息：手动建立1 or 2维数组存储
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
typedef struct udge_Node udge_node;// 由于两个节点相互嵌套定义，需要先做前向声明再填充内容
typedef struct udgv_Node {
	int n;							// 点序号
	udge_node *firstedge;			// 该点的第一个边
	struct udgv_Node *link1;		// 链表中前一个点
	struct udgv_Node *link2;		// 链表中后一个点
	int mark;						// 用作遍历标记（默认写0）
} udgv_node;
struct udge_Node {
	int weight;						// 权值（默认为1）
	udgv_node *v1;					// 一个顶点
	udgv_node *v2;					// 另一个顶点
	struct udge_Node *v1link;		// v1顶点的下一个边
	struct udge_Node *v2link;		// v2顶点的下一个边
	int mark;						// 用作遍历标记（默认写0）
};

// 基本信息结构体
typedef struct {
	udgv_node *head;				// 点链表头结点
	udgv_node *tail;				// 点链表尾结点
	int size_v;						// 点总数
	int size_e;						// 边总数
} intudgraph;

// 最短路径问题函数输出结构体
typedef struct {
	udgv_node *start;				// 路径起始顶点
	udgv_node *end;					// 路径终止顶点
	int length;						// 路径长度
	int *road;						// 路径顶点序号序列
} intudgraph_shortest_road;



// 全局变量udgraph_error_output：控制是否打印错误信息（默认打印）
static int udgraph_error_output = 1;



// 1.图的结构控制
intudgraph* udgraph_intgraph();												// 创建图（空图）
udge_node* udgraph_addedge(intudgraph *graph, int v1, int v2, int weight);	// 添加一条边（若顶点序号是新的，就同步添加点节点）
int udgraph_addedges(intudgraph *graph, int e, int (*a)[3]);				// 批量添加边（及对应节点）
int udgraph_deleteedge(intudgraph *graph, udge_node *e);					// 删除地址为e的边（及删边导致的独立节点）
int udgraph_deletevertex(intudgraph *graph, udgv_node *v);					// 删除地址为v的顶点
int udgraph_deletegraph(intudgraph *graph);									// 删除全部节点，删除图信息
intudgraph* udgraph_copy(intudgraph *graph1);								// 复制图graph1

// 2.节点、图相关信息获取
udgv_node* udgraph_getplace_v(intudgraph *graph, int n);					// 获取图graph中序号为n的顶点地址
udge_node* udgraph_getplace_e(intudgraph *graph, int n1, int n2);			// 获取图graph中序号为n1、n2的顶点边的地址
udgv_node** udgraph_DFS(intudgraph *graph, udgv_node *start, int option);	// 深度优先遍历（DFS）（依据遍历起始点start，输出顶点地址数组）
udgv_node** udgraph_BFS(intudgraph *graph, udgv_node *start, int option);	// 广度优先遍历（BFS）（依据遍历起始点start，输出顶点地址数组）
udge_node** udgraph_get_edgelist(intudgraph *graph);						// 获取边节点数组（依次按weight、v1顶点序号、v2顶点序号升序排列）
udge_node** udgraph_MST_Prim(intudgraph *graph);							// 求最小生成树（Prim算法）
udge_node** udgraph_MST_Kruskal(intudgraph *graph);							// 求最小生成树（Kruskal算法）
intudgraph_shortest_road* udgraph_shortest_path_Dijkstra(intudgraph *graph, udgv_node *start);
																			// 求最短路径（Dijkstra算法，计算从start到所有其他节点的最短路径）
intudgraph_shortest_road** udgraph_shortest_path_Floyd(intudgraph *graph);	// 求最短路径（Floyd算法，计算图中任意两点的最短路径）

// 3.图的数据控制
void udgraph_set_mark_v(intudgraph *graph);									// 所有顶点的mark字段归0
void udgraph_set_mark_e(intudgraph *graph);									// 所有边的mark字段归0

// 4.其他（供其他函数调用的basic函数等）
void udgraph_set_error_output(int enable);									// 控制是否可视化打印错误信息
static udgv_node* udgraph_basic_copyvnode(udgv_node *dest, udgv_node *src);	// （basic）将顶点节点src的数据复制给顶点节点dest
static udge_node* udgraph_basic_copyenode(udge_node *dest, udge_node *src);	// （basic）将边节点src的数据复制给边节点dest
int udgraph_basic_DFS_cmp(const void *a, const void *b);					// （basic）DFS函数专用的qsort比较函数
int udgraph_basic_BFS_cmp(const void *a, const void *b);					// （basic）BFS函数专用的qsort比较函数
int udgraph_basic_edge_cmp(const void *a, const void *b);					// （basic）生成MST函数、获取边节点数组函数专用的qsort比较函数



int main() {
	
	
	
}



// 自定义函数

// 1.图的结构控制
/* 创建图（空图）
   返回值：图信息结构体指针（失败返回NULL）
*/
intudgraph* udgraph_intgraph() {// 顶点数，边数，边数组
	intudgraph *graph = (intudgraph*)malloc(sizeof(intudgraph));
	if(!graph) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space for the new udgraph.\n");
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
   
   函数行为约定：v1、v2填写顶点序号，weight可选（默认写1）
*/
udge_node* udgraph_addedge(intudgraph *graph, int v1, int v2, int weight) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	if(v1 == v2) {
	    if(udgraph_error_output) printf("[Illegal operation] Self-loop is not allowed.\n");
		return NULL;
	}
	
	udge_node *e = (udge_node*)malloc(sizeof(udge_node));
	if(!e) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
		return NULL;
	}
	e->mark = 0;
	e->weight = weight;
	
	// 特殊处理空图的边插入
	if(graph->head == NULL) {// 空图
		udgv_node *x1 = (udgv_node*)malloc(sizeof(udgv_node));
		if(!x1) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
			free(e);
			return NULL;
		}
		udgv_node *x2 = (udgv_node*)malloc(sizeof(udgv_node));
		if(!x2) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
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
		x1->n = v1;
		x2->n = v2;
		x1->firstedge = x2->firstedge = e;
		x1->mark = x2->mark = 0;
		e->v1 = x1;
		e->v1link = NULL;
		e->v2 = x2;
		e->v2link = NULL;
		graph->size_e = 1;
		graph->size_v = 2;
		return e;
	}
	udgv_node *t = graph->head, *t1 = NULL, *t2 = NULL;
	
	// 扫描v1、v2在不在顶点链里，并获取其指针
	while(t != NULL) {
		if(t->n == v1) {// v1顶点已在链表中
			t1 = t;
		} else if(t->n == v2) {// v2顶点已在链表中
			t2 = t;
		}
		t = t->link2;
	}
	
	// 分四类处理边添加（方便空间不足时的重置操作）
	if(t1 == NULL && t2 == NULL) {// 都不在顶点链中
		udgv_node *x1 = (udgv_node*)malloc(sizeof(udgv_node));
		if(!x1) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
			free(e);
			return NULL;
		}
		udgv_node *x2 = (udgv_node*)malloc(sizeof(udgv_node));
		if(!x2) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
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
		x1->firstedge = e;
		x2->firstedge = e;
		x1->n = v1;
		x2->n = v2;
		x1->mark = x2->mark = 0;
		e->v1 = x1;
		e->v1link = NULL;
		e->v2 = x2;
		e->v2link = NULL;
		graph->size_e++;
		graph->size_v += 2;
		return e;
	} else if(t1 == NULL) {// 仅v1不在顶点链中
		udgv_node *x1 = (udgv_node*)malloc(sizeof(udgv_node));
		if(!x1) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
			free(e);
			return NULL;
		}
		graph->tail->link2 = x1;
		x1->link1 = graph->tail;
		graph->tail = x1;
		x1->link2 = NULL;
		x1->n = v1;
		x1->firstedge = e;
		x1->mark = 0;
		e->v1 = x1;
		e->v1link = NULL;
		e->v2 = t2;
		e->v2link = t2->firstedge;
		t2->firstedge = e;
		graph->size_e++;
		graph->size_v++;
		return e;
	} else if(t2 == NULL) {// 仅v2不在顶点链中
		udgv_node *x2 = (udgv_node*)malloc(sizeof(udgv_node));
		if(!x2) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to add the new edge.\n");
			free(e);
			return NULL;
		}
		graph->tail->link2 = x2;
		x2->link1 = graph->tail;
		graph->tail = x2;
		x2->link2 = NULL;
		x2->n = v2;
		x2->firstedge = e;
		x2->mark = 0;
		e->v2 = x2;
		e->v2link = NULL;
		e->v1 = t1;
		e->v1link = t1->firstedge;
		t1->firstedge = e;
		graph->size_e++;
		graph->size_v++;
		return e;
	} else {// 都在顶点链中
		e->v1 = t1;
		e->v1link = t1->firstedge;
		t1->firstedge = e;
		e->v2 = t2;
		e->v2link = t2->firstedge;
		t2->firstedge = e;
		graph->size_e++;
		return e;
	}
}

/* 批量添加边（及对应节点）
   返回值：成功添加边的个数（错误返回-1）
   
   函数行为约定： 输入的“边信息”为二维数组int (*)[3]（第一维大小取决于e的大小），描述每条边的两个顶点序号以及权值（没有就都写1）
*/
int udgraph_addedges(intudgraph *graph, int e, int (*a)[3]) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return -1;
	}
	
	int count = 0;
	for(int i = 0; i < e; i++) {
		if(udgraph_addedge(graph, a[i][0], a[i][1], a[i][2]) == NULL) {
			return count;// 若从这里返回，将触发udgraph_addedge的错误报告机制
		}
		count++;
	}
	return e;
}

/* 删除地址为e的边（及删边导致的独立节点）
   返回值：1（失败返回-1）
*/
int udgraph_deleteedge(intudgraph *graph, udge_node *e) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return -1;
	}
	if(e == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udge_node pointer is NULL.\n");
		return -1;
	}
	
	udgv_node *a, *b;// 边e的两个顶点a、b
	a = e->v1;
	b = e->v2;
	if(e->v1link == NULL && a->firstedge == e) {// 如果a只有一条边：同步删掉a
		// 删掉节点a
		if(graph->size_v == 1) {
			graph->head = graph->tail = NULL;
		} else if(graph->head == a) {
			graph->head = a->link2;
			a->link2->link1 = NULL;
		} else if(graph->tail == a) {
			graph->tail = a->link1;
			a->link1->link2 = NULL;
		} else {
			a->link1->link2 = a->link2;
			a->link2->link1 = a->link1;
		}
		free(a);
		graph->size_v--;
	} else {// 从a的边链中把e删除
		if(a->firstedge == e) {
			a->firstedge = e->v1link;
		} else {
			udge_node *t = a->firstedge;
			while(t != NULL) {
				if(t->v1 == a) {
					if(t->v1link == e) {
						t->v1link = e->v1link;
						break;
					}
					t = t->v1link;
				} else {
					if(t->v2link == e) {
						t->v2link = e->v1link;
						break;
					}
					t = t->v2link;
				}
			}
		}
	}
	if(e->v2link == NULL && b->firstedge == e) {// 如果b只有一条边：同步删掉b
		// 删掉节点b
		if(graph->size_v == 1) {
			graph->head = graph->tail = NULL;
		} else if(graph->head == b) {
			graph->head = b->link2;
			b->link2->link1 = NULL;
		} else if(graph->tail == b) {
			graph->tail = b->link1;
			b->link1->link2 = NULL;
		} else {
			b->link1->link2 = b->link2;
			b->link2->link1 = b->link1;
		}
		free(b);
		graph->size_v--;
	} else {// 从b的边链中把e删除
		if(b->firstedge == e) {
			b->firstedge = e->v2link;
		} else {
			udge_node *t = b->firstedge;
			while(t != NULL) {
				if(t->v1 == b) {
					if(t->v1link == e) {
						t->v1link = e->v2link;
						break;
					}
					t = t->v1link;
				} else {
					if(t->v2link == e) {
						t->v2link = e->v2link;
						break;
					}
					t = t->v2link;
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
int udgraph_deletevertex(intudgraph *graph, udgv_node *v) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return -1;
	}
	if(v == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgv_node pointer is NULL.\n");
		return -1;
	}
	
	if(v->firstedge == NULL) {// 单独处理“无边顶点”（只是比较安全的写法，理论上不可能出现孤立点）
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
	
	for(udge_node *t = v->firstedge, *t0; t != NULL;) {// 删到v的最后一个边时会自动删掉顶点v
		if(t->v1 == v) {
			t0 = t;
			t = t->v1link;
			udgraph_deleteedge(graph, t0);
		} else {
			t0 = t;
			t = t->v2link;
			udgraph_deleteedge(graph, t0);
		}
	}
	return 1;
}

/* 删除全部节点，删除图信息
   返回值：1（失败返回-1）
*/
int udgraph_deletegraph(intudgraph *graph) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return -1;
	}
	
	while(graph->head != NULL) {
		udgraph_deletevertex(graph, graph->head);
	}
	free(graph);
	return 1;
}

/* 复制图graph1
   返回值：复制出来的图的信息结构体指针（失败返回NULL）
   
   函数行为约定：为简便，暂时不保证graph2和graph1的边链顺序一致。不会影响任何图性质的运用，但可能导致在某些刁钻题目出错。注意！
*/
intudgraph* udgraph_copy(intudgraph *graph1) {
	if(graph1 == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	
	intudgraph *graph2 = (intudgraph*)malloc(sizeof(intudgraph));
	if(!graph2) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space for the new udgraph.\n");
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
		udgv_node *a1;
		udgv_node *a2;
	} int0;
	int0 *list = (int0*)malloc(sizeof(int0) * graph1->size_v);
	int count0 = 0;// 记录当前list中信息个数
	if(!list) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to copy the udgraph.\n");
		free(graph2);
		return NULL;
	}
	udgv_node *l0 = NULL;
	for(udgv_node *t = graph1->head; t != NULL; t = t->link2) {
		udgv_node *k = (udgv_node*)malloc(sizeof(udgv_node));		
		if(!k) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space for the new udgraph.\n");
			if(graph2->size_v > 0) {
				for(udgv_node *t1 = list[0].a2; t1 != NULL;) {
					udgv_node *t2 = t1;
					t1 = t1->link2;
					free(t2);
				}
			}
			free(list);
			free(graph2);
			return NULL;
		}
		k->firstedge = NULL;
		k->link1 = l0;
		if(l0 != NULL) {
			l0->link2 = k;
		}
		k->link2 = NULL;
		udgraph_basic_copyvnode(k, t);
		l0 = k;
		list[graph2->size_v].a1 = t;// 记录映射关系
		list[graph2->size_v++].a2 = k;// 记录映射关系
	}
	graph2->head = list[0].a2;
	graph2->tail = list[graph2->size_v - 1].a2;
	
	// 遍历所有顶点的边序列，并复制（仅在边节点v1与当前顶点相符时复制，避免重复）
	// 注：为了方便操作，在复制边链时全部采用“头插法”，但将导致新边链和原边链顺序相反（并不影响图结构），遇到刁钻的题目可能会出错
	for(udgv_node *t0 = graph1->head; t0 != NULL; t0 = t0->link2) {
		for(udge_node *t = t0->firstedge; t != NULL;) {
			if(t->v1 == t0) {// 复制该边，头插入t0的边链中
				udge_node *k = (udge_node*)malloc(sizeof(udge_node));
				if(!k) {
					if (udgraph_error_output) printf("[Memory exceeded] There is not enough space for the new udgraph.\n");
					udgraph_deletegraph(graph2);// 对已复制的边和顶点进行回滚
					free(list);
					return NULL;
				}
				udgraph_basic_copyenode(k, t);
				// 运用映射表，将k头插入graph2的两个顶点边序列中
				udgv_node *x1, *x2;
				for(int i = 0, sign = 0; i < graph2->size_v && sign < 2; i++) {
					if(list[i].a1 == t->v1) {
						x1 = list[i].a2;
						sign++;
					} else if(list[i].a1 == t->v2) {
						x2 = list[i].a2;
						sign++;
					}
				}
				// 完成两次头插
				k->v1 = x1;
				k->v1link = x1->firstedge;
				x1->firstedge = k;
				k->v2 = x2;
				k->v2link = x2->firstedge;
				x2->firstedge = k;
				
				t = t->v1link;
				graph2->size_e++;
			} else {
				t = t->v2link;
			}
		}
	}
	
	free(list);
	return graph2;
}



// 2.节点、图相关信息获取
/* 获取图graph中序号为n的顶点地址
   返回值：目标顶点节点指针（找不到返回NULL）
*/
udgv_node* udgraph_getplace_v(intudgraph *graph, int n) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	
	for(udgv_node *t = graph->head; t != NULL; t = t->link2) {
		if(t->n == n) {
			return t;
		}
	}
	return NULL;
}

/* 获取图graph中序号为n1、n2的顶点边的地址
   返回值：目标边节点指针（找不到返回NULL）
*/
udge_node* udgraph_getplace_e(intudgraph *graph, int n1, int n2) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	
	udgv_node *t0 = udgraph_getplace_v(graph, n1);
	if(t0 == NULL) {
		return NULL;
	}
	for(udge_node *t = t0->firstedge; t != NULL;) {
		if(t->v1 == t0) {
			if(t->v2->n == n2) {
				return t;
			}
			t = t->v1link;
		} else {
			if(t->v1->n == n2) {
				return t;
			}
			t = t->v2link;
		}
	}
	return NULL;
}

/* 深度优先遍历（DFS）（依据遍历起始点start，输出顶点地址数组）
   返回值：顶点地址数组指针（错误返回NULL）
   
   option：0：仅完成基础DFS遍历规则 1：相同层级的节点按照“序号从小到大”访问，但时间复杂度较高
   
   注：暂时不考虑非连通图。万一需要，手动多调用几遍就ok了
*/
udgv_node** udgraph_DFS(intudgraph *graph, udgv_node *start, int option) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	if(start == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgv_node pointer is NULL.\n");
		return NULL;
	}
	if(option != 0 && option != 1) {
		if(udgraph_error_output) printf("[Invalid function argument] The \"option\" used to select option should be 0 or 1.\n");
		return NULL;
	}
	
	udgraph_set_mark_v(graph);
	udgv_node **list = (udgv_node**)malloc(sizeof(udgv_node*) * graph->size_v);// 输出空间
	if(!list) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to do the DFS.\n");
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		list[i] = NULL;
	}
	int count = 0;// 实时记录list中数据个数
	
	udgv_node **stuck = (udgv_node**)malloc(sizeof(udgv_node*) * graph->size_v);// 递归栈
	if(!stuck) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to do the DFS.\n");
		free(list);
		return NULL;
	}
	int top = -1;// 实时记录栈顶位置
	
	udgv_node **temp = (udgv_node**)malloc(sizeof(udgv_node*) * graph->size_v);// 用于临时存储当前节点的相邻节点
	if(!temp) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to do the DFS.\n");
		free(list);
		free(stuck);
		return NULL;
	}
	int count1 = 0;
	
	stuck[++top] = start;// 遍历起点
	start->mark = 1;
	while(top >= 0) {
		// 先把栈顶元素记录并出栈
		udgv_node *t0 = stuck[top--];
		list[count++] = t0;
		
		// 再遍历t0的所有相邻节点，将其中mark=0的部分标记访问并入栈
		count1 = 0;
		for(udge_node *t = t0->firstedge; t != NULL;) {
			if(t->v1 == t0) {
				if(t->v2->mark == 0) {
					temp[count1++] = t->v2;
					t->v2->mark = 1;
				}
				t = t->v1link;
			} else {
				if(t->v1->mark == 0) {
					temp[count1++] = t->v1;
					t->v1->mark = 1;
				}
				t = t->v2link;
			}
		}
		// 如果option=1，需要控制按序号从大到小依次入栈
		if(option) {
			qsort(temp, count1, sizeof(udgv_node*), udgraph_basic_DFS_cmp);
		}
		for(int i = 0; i < count1; i++) {
			stuck[++top] = temp[i];
		}
	}
	// 遍历结束时：top = -1
	
	free(temp);
	free(stuck);
	udgraph_set_mark_v(graph);
	return list;
}

/* 广度优先遍历（BFS）（依据遍历起始点start，输出顶点地址数组）
   返回值：顶点地址数组指针（错误返回NULL）
   
   option：0：仅完成基础BFS遍历规则 1：相同层级的节点按照“序号从小到大”访问，但时间复杂度较高
   
   注：暂时不考虑非连通图。万一需要，手动多调用几遍就ok了
*/
udgv_node** udgraph_BFS(intudgraph *graph, udgv_node *start, int option) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	if(start == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgv_node pointer is NULL.\n");
		return NULL;
	}
	if(option != 0 && option != 1) {
		if(udgraph_error_output) printf("[Invalid function argument] The \"option\" used to select option should be 0 or 1.\n");
		return NULL;
	}
	
	udgraph_set_mark_v(graph);
	udgv_node **list = (udgv_node**)malloc(sizeof(udgv_node*) * graph->size_v);// 输出空间
	if(!list) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to do the BFS.\n");
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		list[i] = NULL;
	}
	int count = 0;// 实时记录list中数据个数
	
	udgv_node **queue = (udgv_node**)malloc(sizeof(udgv_node*) * graph->size_v);// 队列
	if(!queue) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to do the BFS.\n");
		free(list);
		return NULL;
	}
	int head = 0, tail = -1;// 实时记录队首、尾位置
	
	udgv_node **temp = (udgv_node**)malloc(sizeof(udgv_node*) * graph->size_v);// 用于临时存储当前节点的相邻节点
	if(!temp) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to do the BFS.\n");
		free(list);
		free(queue);
		return NULL;
	}
	int count1 = 0;
	
	queue[++tail] = start;// 遍历起点
	start->mark = 1;
	while(head <= tail) {
		// 先把队首元素记录并出栈
		udgv_node *t0 = queue[head++];
		list[count++] = t0;
		
		// 再遍历t0的所有相邻节点，将其中mark=0的部分标记访问并入队
		count1 = 0;
		for(udge_node *t = t0->firstedge; t != NULL;) {
			if(t->v1 == t0) {
				if(t->v2->mark == 0) {
					temp[count1++] = t->v2;
					t->v2->mark = 1;
				}
				t = t->v1link;
			} else {
				if(t->v1->mark == 0) {
					temp[count1++] = t->v1;
					t->v1->mark = 1;
				}
				t = t->v2link;
			}
		}
		// 如果option=1，需要控制按序号从小到大依次入队
		if(option) {
			qsort(temp, count1, sizeof(udgv_node*), udgraph_basic_BFS_cmp);
		}
		for(int i = 0; i < count1; i++) {
			queue[++tail] = temp[i];
		}
	}
	
	free(temp);
	free(queue);
	udgraph_set_mark_v(graph);
	return list;
}

/* 获取边节点数组（依次按weight、v1顶点序号、v2顶点序号升序排列）
   返回值：边地址数组指针（错误返回NULL）
*/
udge_node** udgraph_get_edgelist(intudgraph *graph) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	if(graph->head == NULL) {
		if(udgraph_error_output) printf("[Illegal operation] The target udgraph is empty.\n");
		return NULL;
	}
	
	udge_node **list = (udge_node**)malloc(sizeof(udge_node*) * graph->size_e);
	if(!list) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space for the edgelist.\n");
		return NULL;
	}
	int count = 0;
	
	for(udgv_node *t0 = graph->head; t0 != NULL; t0 = t0->link2) {
		for(udge_node *t = t0->firstedge; t != NULL;) {
			if(t->v1 == t0) {
				list[count++] = t;
				t = t->v1link;
			} else {
				t = t->v2link;
			}
		}
	}
	qsort(list, graph->size_e, sizeof(udge_node*), udgraph_basic_edge_cmp);
	return list;
}

/* 求最小生成树（Prim算法）
   返回值：被选入树中的边指针的数组，依次按weight、v1顶点序号、v2顶点序号升序排列
   
   注：更适合稠密图
   
   原理：
   lowcost数组：记录各顶点到当前生成树的最小边权，起点为0，未连接为∞
   closest数组：记录使lowcost最小的那条原图边
   （另：）映射数组list：记录形式序号（1~graph->size_v）所代表的顶点地址
   每次循环，从当前顶点出发更新lowcost、closest数组，并找到lowcost最小的顶点对应边，加入tree中
*/
udge_node** udgraph_MST_Prim(intudgraph *graph) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	if (graph->size_v < 2) {
		if (udgraph_error_output) printf("[Illegal operation] Cannot create an MST from a udgraph with less than 2 vertexs.\n");
		return NULL;
	}
	
	udgv_node **list = (udgv_node**)malloc(sizeof(udgv_node*) * graph->size_v);// 临时顶点地址索引（0~size_v-1），同步记录在mark中
	if(!list) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to create the MST.\n");
		return NULL;
	}
	udgv_node *t = graph->head;
	for(int i = 0; t != NULL; i++, t = t->link2) {
		list[i] = t;
		t->mark = i;
	}
	
	int *lowcost = (int*)malloc(sizeof(int) * graph->size_v);
	if(!lowcost) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to create the MST.\n");
		free(list);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		lowcost[i] = INT_MAX;
	}
	
	udge_node **closest = (udge_node**)malloc(sizeof(udge_node*) * graph->size_v);
	if(!closest) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to create the MST.\n");
		free(list);
		free(lowcost);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		closest[i] = NULL;
	}
	
	udge_node **tree = (udge_node**)malloc(sizeof(udge_node*) * (graph->size_v - 1));// 小结论：最小生成树的边数 = 顶点数 - 1
	if(!tree) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space for the MST.\n");
		free(list);
		free(lowcost);
		free(closest);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	int sum = 0;// 实时记录已经加入到tree中的节点数目
	
	// 生成树
	t = graph->head;
	lowcost[0] = 0;
	while(sum < graph->size_v - 1) {
		// 遍历t的（还不在tree中的）边，更新lowcost和closest（边在tree中以lowcost[形式序号] = 0表示）
		for(udge_node *t1 = t->firstedge; t1 != NULL;) {
			if(t1->v1 == t && lowcost[t1->v2->mark] != 0) {// t1->v2->mark为当前边另一顶点的形式序号
				if(t1->weight < lowcost[t1->v2->mark]) {
					lowcost[t1->v2->mark] = t1->weight;
					closest[t1->v2->mark] = t1;
				}
			} else if(lowcost[t1->v1->mark] != 0) {// t1->v1->mark为当前边另一顶点的形式序号
				if(t1->weight < lowcost[t1->v1->mark]) {
					lowcost[t1->v1->mark] = t1->weight;
					closest[t1->v1->mark] = t1;
				}
			}
			t1 = (t1->v1 == t) ? t1->v1link : t1->v2link;
		}
		
		// 找到下一个要被加入tree的点的形式序号
		int temp, min = INT_MAX;
		for(int i = 0; i < graph->size_v; i++) {
			if(lowcost[i] < min && lowcost[i] != 0) {
				temp = i;
				min = lowcost[i];
			}
		}
		if (min == INT_MAX) {
			if (udgraph_error_output) printf("[Other errors] The target udgraph is not a connected graph.\n");
			free(list);
			free(lowcost);
			free(closest);
			free(tree);
			udgraph_set_mark_v(graph);
			return NULL;
		}
		
		// 填写tree，更新“当前节点”位置（并更新其lowcost）
		tree[sum++] = closest[temp];// 将边放入tree中
		lowcost[temp] = 0;// 将顶点标记为“已放入tree”
		
		t = list[temp];// 定位到下一个顶点地址
	}
	
	free(list);
	free(lowcost);
	free(closest);
	udgraph_set_mark_v(graph);
	qsort(tree, graph->size_v - 1, sizeof(udge_node*), udgraph_basic_edge_cmp);
	return tree;
}

/* 求最小生成树（Kruskal算法）
   返回值：被选入树中的边指针的数组，依次按weight、v1顶点序号、v2顶点序号升序排列
   
   注：更适合稀疏图
   
   原理：
   从graph中选择一条当前未选择过的、且边上的权值最小的边加入tree。若加入tree后使得tree未产生回路，则本次选择有效；如使得tree产生回路，则本次选择无效，
   放弃本次选择的边。重复上述选择过程直到tree中包含了graph的n-1条边，此时的tree为graph的最小生成树。
*/
udge_node** udgraph_MST_Kruskal(intudgraph *graph) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	if (graph->size_v < 2) {
		if (udgraph_error_output) printf("[Illegal operation] Cannot create an MST from a udgraph with less than 2 vertexs.\n");
		return NULL;
	}
	
	udgv_node *t = graph->head;
	for(int i = 0; t != NULL; i++, t = t->link2) {// 借用顶点的mark字段设定临时顶点地址索引（0~size_v-1）
		t->mark = i;
	}
	
	int *parent = (int*)malloc(sizeof(int) * graph->size_v);// 记录每个顶点所在集合的代表顶点形式序号
	if(!parent) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to create the MST.\n");
		udgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		parent[i] = i;// 初始，每个顶点的代表就是自己
	}
	
	udge_node **edgelist = udgraph_get_edgelist(graph);// 按权值由小到大排列的边数组
	if(!edgelist) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to create the MST.\n");
		free(parent);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	
	udge_node **tree = (udge_node**)malloc(sizeof(udge_node*) * (graph->size_v - 1));// 小结论：最小生成树的边数 = 顶点数 - 1
	if(!tree) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space for the MST.\n");
		free(parent);
		free(edgelist);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	int sum = 0;// 实时记录已经加入到tree中的节点数目
	
	for(int i = 0; sum < graph->size_v - 1 && i < graph->size_e; i++) {// 核心思路：“并查集”，快速判断新边会不会导致回路
		udgv_node *x1 = edgelist[i]->v1;
		udgv_node *x2 = edgelist[i]->v2;
		// 判断x1、x2在不在同一集合内
		int m1 = x1->mark, m2 = x2->mark, tm;
		tm = m1;
		while(parent[tm] != tm) {
			tm = parent[tm];
		}
		parent[m1] = tm;// “路径压缩”，即把集合末端顶点直接挂在代表顶点下
		m1 = tm;
		tm = m2;
		while(parent[tm] != tm) {
			tm = parent[tm];
		}
		parent[m2] = tm;
		m2 = tm;
		// 依照代表顶点m1、m2情况，进一步操作
		if(m1 != m2) {// 不在同一集合内，添加该边
			tree[sum++] = edgelist[i];
			// 将m1、m2集合合并
			parent[m2] = m1;
		}
	}
	if(sum < graph->size_v - 1) {
		if (udgraph_error_output) printf("[Other errors] The target udgraph is not a connected graph.\n");
		free(parent);
		free(edgelist);
		free(tree);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	
	free(parent);
	free(edgelist);
	// 在udgraph_get_edgelist函数里已经排过序了
	udgraph_set_mark_v(graph);
	return tree;
}

/* 求最短路径（Dijkstra算法，计算从start到所有其他节点的最短路径）
   返回值：intudgraph_shortest_road数组（该数组的序号与实际顶点序号无关）
   
   函数行为约定：
   1.要求所有edge的weight非负
   2.输出的结果数组包含从start到start的路径，仅为易于操作
   
   注：更适合稀疏图
   
   原理：
   distance数组：记录各顶点到start顶点的最小加权路径长度，起点为0，未连接为∞
   prev数组：记录形式序号i的顶点在路径中的前一顶点地址
   visited数组：标记“已经计算完毕”的顶点（鉴于mark字段被占用了）
   （另：）映射数组list：记录形式序号（1~graph->size_v）所代表的顶点地址
   每次循环，从当前顶点出发更新distance、prev数组，并找到distance最小的顶点，进行visited标记
*/
intudgraph_shortest_road* udgraph_shortest_path_Dijkstra(intudgraph *graph, udgv_node *start) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	if(graph->head == NULL) {
		if(udgraph_error_output) printf("[Illegal operation] The target udgraph is empty.\n");
		return NULL;
	}
	if(start == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgv_node pointer is NULL.\n");
		return NULL;
	}
	
	// malloc出 list、distance、prev、visited、out 5个数组
	udgv_node **list = (udgv_node**)malloc(sizeof(udgv_node*) * graph->size_v);// 临时顶点地址索引（0~size_v-1），同步记录在mark中
	if(!list) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		return NULL;
	}
	udgv_node *t = graph->head;
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
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		distance[i].sum = INT_MAX;
		distance[i].n = 0;
	}
	
	udgv_node **prev = (udgv_node**)malloc(sizeof(udgv_node*) * graph->size_v);
	if(!prev) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		free(distance);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		prev[i] = NULL;
	}
	
	int *visited = (int*)malloc(sizeof(int) * graph->size_v);
	if(!visited) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		free(distance);
		free(prev);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		visited[i] = 0;
	}
	
	intudgraph_shortest_road *out = (intudgraph_shortest_road*)malloc(sizeof(intudgraph_shortest_road) * graph->size_v);
	if(!out) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		free(distance);
		free(prev);
		free(visited);
		udgraph_set_mark_v(graph);
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
		for(udge_node *t1 = t->firstedge; t1 != NULL;) {
			if(t1->v1 == t && visited[t1->v2->mark] != 1) {// t1->v2->mark为当前边另一顶点的形式序号
				if(distance[t->mark].sum + t1->weight < distance[t1->v2->mark].sum) {
					distance[t1->v2->mark].sum = distance[t->mark].sum + t1->weight;
					distance[t1->v2->mark].n = distance[t->mark].n + 1;
					prev[t1->v2->mark] = t;
				}
			} else if(visited[t1->v1->mark] != 1) {// t1->v1->mark为当前边另一顶点的形式序号
				if(distance[t->mark].sum + t1->weight < distance[t1->v1->mark].sum) {
					distance[t1->v1->mark].sum = distance[t->mark].sum + t1->weight;
					distance[t1->v1->mark].n = distance[t->mark].n + 1;
					prev[t1->v1->mark] = t;
				}
			}
			t1 = (t1->v1 == t) ? t1->v1link : t1->v2link;
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
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
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
		udgv_node **stack = (udgv_node**)malloc(sizeof(udgv_node*) * (distance[i].n + 1));
		if(!stack) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
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
	udgraph_set_mark_v(graph);
	return out;
}

/* 求最短路径（Floyd算法，计算图中任意两点的最短路径）
   返回值：二维intudgraph_shortest_road数组（该数组的序号与实际顶点序号无关）
   
   注：更适合稠密图
   
   原理：
   dist[i][j]：从顶点i到顶点j的当前最短距离
   path[i][j]：在i→j的最短路径上，顶点j的前驱顶点的形式序号（用于之后重建路径）
   
*/
intudgraph_shortest_road** udgraph_shortest_path_Floyd(intudgraph *graph) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return NULL;
	}
	if(graph->head == NULL) {
		if(udgraph_error_output) printf("[Illegal operation] The target udgraph is empty.\n");
		return NULL;
	}
	
	// malloc出 list、distance（二维）、path（二维）、out（二维） 4个数组（由于dp方式的不同，不需要visited数组）
	udgv_node **list = (udgv_node**)malloc(sizeof(udgv_node*) * graph->size_v);// 临时顶点地址索引（0~size_v-1），同步记录在mark中
	if(!list) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		return NULL;
	}
	udgv_node *t = graph->head;
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
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	int **path = (int**)malloc(sizeof(int*) * graph->size_v);
	if(!path) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		free(distance);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		distance[i] = (intdistance*)malloc(sizeof(intdistance) * graph->size_v);
		if(!distance[i]) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
			for(int j = 0; j < i; j++) {
				free(distance[j]);
			}
			free(list);
			free(distance);
			udgraph_set_mark_v(graph);
			return NULL;
		}
	}
	for(int i = 0; i < graph->size_v; i++) {
		path[i] = (int*)malloc(sizeof(int) * graph->size_v);
		if(!path[i]) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
			for(int j = 0; j < i; j++) {
				free(path[j]);
			}
			free(list);
			for(int j = 0; j < graph->size_v; j++) {
				free(distance[j]);
			}
			free(distance);
			udgraph_set_mark_v(graph);
			return NULL;
		}
	}
	
	intudgraph_shortest_road **out = (intudgraph_shortest_road**)malloc(sizeof(intudgraph_shortest_road*) * graph->size_v);
	if(!out) {
		if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
		free(list);
		for(int j = 0; j < graph->size_v; j++) {
			free(distance[j]);
		}
		free(distance);
		for(int j = 0; j < graph->size_v; j++) {
			free(path[j]);
		}
		free(path);
		udgraph_set_mark_v(graph);
		return NULL;
	}
	for(int i = 0; i < graph->size_v; i++) {
		out[i] = (intudgraph_shortest_road*)malloc(sizeof(intudgraph_shortest_road) * graph->size_v);
		if(!out[i]) {
			if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
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
			udgraph_set_mark_v(graph);
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
	for(udgv_node *t = graph->head; t != NULL; t = t->link2) {
		for(udge_node *t1 = t->firstedge; t1 != NULL;) {
			if(t1->v1 == t) {
				distance[t->mark][t1->v2->mark].sum = t1->weight;
				distance[t->mark][t1->v2->mark].n = 1;
				path[t->mark][t1->v2->mark] = t->mark;
				t1 = t1->v1link;
			} else {
				distance[t->mark][t1->v1->mark].sum = t1->weight;
				distance[t->mark][t1->v1->mark].n = 1;
				path[t->mark][t1->v1->mark] = t->mark;
				t1 = t1->v2link;
			}
		}
	}
	
	// Floyd算法的核心三重循环（DP）：在仅可采用0~k作为途经点时，尝试dp优化从i到j的最短路径
	// 内层循环检查!=INT_MAX：利用短路求值，防止相加爆int
	// dp设计：
	for(int k = 0; k < graph->size_v; k++) {// 此时，仅0~k可作为途经点（循环前都是直接连线）
		for(int i = 0; i < graph->size_v; i++) {
			for(int j = 0; j < graph->size_v; j++) {
				// 进入此循环时，如果distance[i][k]、distance[k][j]不为INT_MAX，那必是在之前算出的，满足“仅采用0~k-1作为途经点”
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
				if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
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
				udgraph_set_mark_v(graph);
				return NULL;
			}
			int top = -1;
			out[i][j].road = (int*)malloc(sizeof(int) * (distance[i][j].n + 1));
			if(!out[i][j].road) {
				if (udgraph_error_output) printf("[Memory exceeded] There is not enough space to count the shortest road.\n");
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
				udgraph_set_mark_v(graph);
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
	udgraph_set_mark_v(graph);
	return out;
}



// 3.图的数据控制
/* 所有顶点的mark字段归0
*/
void udgraph_set_mark_v(intudgraph *graph) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return;
	}
	
	for(udgv_node *t = graph->head; t != NULL; t = t->link2) {
		t->mark = 0;
	}
	return;
}

/* 所有边的mark字段归0
*/
void udgraph_set_mark_e(intudgraph *graph) {
	if(graph == NULL) {
		if(udgraph_error_output) printf("[Invalid function argument] The target udgraph pointer is NULL.\n");
		return;
	}
	
	for(udgv_node *t = graph->head; t != NULL; t = t->link2) {
		for(udge_node *k = t->firstedge; k != NULL;) {
			k->mark = 0;
			if(k->v1 == t) {
				k = k->v1link;
			} else {
				k = k->v2link;
			}
		}
	}
	return;
}



// 4.其他（供其他函数调用的basic函数等）
// 注：basic函数不做可靠的错误检查，仅做部分错误反馈，尽量不要手动调用
/* 控制是否可视化打印错误信息
*/
void udgraph_set_error_output(int enable) {// 1→打印；0→不打印
	udgraph_error_output = enable ? 1 : 0;
	return;
}

/* （basic）将顶点节点src的数据复制给顶点节点dest
   返回值：dest节点地址
   用途：配合udgraph_copy函数
*/
static udgv_node* udgraph_basic_copyvnode(udgv_node *dest, udgv_node *src) {
	udge_node *first = dest->firstedge;
	udgv_node *link1 = dest->link1;
	udgv_node *link2 = dest->link2;
	*dest = *src;
	dest->firstedge = first;
	dest->link1 = link1;
	dest->link2 = link2;
	return dest;
}

/* （basic）将边节点src的数据复制给边节点dest
   返回值：dest节点地址
   用途：配合udgraph_copy函数
*/
static udge_node* udgraph_basic_copyenode(udge_node *dest, udge_node *src) {
	udgv_node *v1 = dest->v1;
	udge_node *v1link = dest->v1link;
	udgv_node *v2 = dest->v2;
	udge_node *v2link = dest->v2link;
	*dest = *src;
	dest->v1 = v1;
	dest->v1link = v1link;
	dest->v2 = v2;
	dest->v2link = v2link;
	return dest;
}

/* （basic）DFS函数专用的qsort比较函数
   返回值：y序号大时返回正数，x序号大时返回负数，相等时返回0
   用途：配合udgraph_DFS函数，供qsort调用，用于option=1情况
   
   注：作为cmp导入qsort将按顶点序号大小降序排列
*/
int udgraph_basic_DFS_cmp(const void *a, const void *b) {
	udgv_node *x = *(udgv_node**)a;
	udgv_node *y = *(udgv_node**)b;
	return y->n - x->n;
}

/* （basic）BFS函数专用的qsort比较函数
   返回值：x序号大时返回正数，y序号大时返回负数，相等时返回0
   用途：配合udgraph_BFS函数，供qsort调用，用于option=1情况
   
   注：作为cmp导入qsort将按顶点序号大小升序排列
*/
int udgraph_basic_BFS_cmp(const void *a, const void *b) {
	udgv_node *x = *(udgv_node**)a;
	udgv_node *y = *(udgv_node**)b;
	return x->n - y->n;
}

/* （basic）生成MST函数、获取边节点数组函数专用的qsort比较函数
   返回值：x序号大时返回正数，y序号大时返回负数，相等时返回0
   用途：配合udgraph_get_edgelist、udgraph_MST_Prim、udgraph_MST_Kruskal函数，供qsort调用
   
   注：作为cmp导入qsort将依次按weight、v1顶点序号、v2顶点序号升序排列
*/
int udgraph_basic_edge_cmp(const void *a, const void *b) {
	udge_node *x = *(udge_node**)a;
	udge_node *y = *(udge_node**)b;
	if(x->weight != y->weight) return x->weight - y->weight;
	if(x->v1->n != y->v1->n) return x->v1->n - y->v1->n;
	return x->v2->n - y->v2->n;
}


