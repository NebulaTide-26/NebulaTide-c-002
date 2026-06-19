#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>// qsort，malloc
#include <time.h>
//#include <windows.h>//（windows特有库）sleep函数等（含max、min）
//#include <conio.h>//（windows特有库）getch函数等
#include <errno.h>// 检查strtol转换溢出问题→"errno"

#define eps 1e-8
#define array_len(x) (sizeof(x)/sizeof(x[0]))//计算数组长度

//查询某字符的ASCII编码值、多组输入样例、以指定进制输出
//输入的数据为x
//手动输入时：以Ctrl+Z结尾



int main()
{
	
	char x;
	while(scanf("%c", &x)!=EOF) {
		printf("%d\n", x);
	}
	return 0;
}

// 自定义函数

