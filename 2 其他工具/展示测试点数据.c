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

// 程序基本信息备注

// 自定义函数声明

// 变量数组

// 基本量数组

int main() {
	char c;
	freopen("article.txt", "r", stdin);
	
	
	while(scanf("%c", &c)!=EOF) {
		printf("%c", c);
	}
	
	
	
return 0;
}

// 自定义函数
