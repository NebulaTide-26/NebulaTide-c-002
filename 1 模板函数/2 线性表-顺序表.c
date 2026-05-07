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
数据结构-线性表-顺序表 函数实现
基本信息类型：intseq

若同时建立多个顺序表，仅需建立多个基本信息变量

*/

// 自动确定顺序表元素类型，需建立elemtype数组确定
#define seqelem1 int
#define seqelem2 student

// 基本信息结构体
typedef struct {
	void *start;// （不定型）顺序表首地址
	int elemsize;// 元素占字节数大小（与“不定型”呼应）
	int len;// 当前已使用的元素空间数
	int maxsize;// 当前已申请的元素空间数
	
} intseq, *intseq_p;

intseq seqlist1;// 存储本顺序表的基本信息

int main() {
	// 创建新顺序表
	intseq *list1 = seq_intlist(list1, 100, );
	
	seq_intlist(seqlist1, 100, sizeof(seqelem1));
	
	
	
return 0;
}

// 自定义函数

// 创建新顺序表 并 返回该链表的基本信息结构体
void seq_intlist(intseq, int size, int elemsize) {// 元素数，单元素占用空间数(通过sizeof和宏定义获取)
	intseq list;
	if(!(list.start = calloc(size, elemsize))) {// 空间不足，表建立失败
		printf("There is not enough space for the new seqlist.\n");
		exit(-1);
	}
	
}




