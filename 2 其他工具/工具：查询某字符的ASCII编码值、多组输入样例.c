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

void jinzhi_to_any(long long in, char* s, int base);

int main()
{
	int base;
	scanf("%d", &base);
	
	char x;
	char s[50];
	while(scanf(" %c", &x) != EOF)//！！！“回车”也算字符！！！
	{
		jinzhi_to_any((long long )x, s, base);
		puts(s);
	}
	
	return 0;
}

// 自定义函数

void jinzhi_to_any(long long in, char* s, int base)// 被转换数字，存储区，目标进制
{
    // 处理0
    if (in == 0) {
        s[0] = '0';
        s[1] = '\0';
        return;
    }
    char buffer[65];  // 临时缓冲区
    int index = 64;   // 从末尾开始
    buffer[64] = '\0';  // 字符串结束符
    
    // 处理负数
    char sign = 0;
    if (in < 0) {
        sign = 1;
        in = -in;
    }
    
    // 转换过程
    do {
        int remainder = in % base;
        
        // 转换为字符
        char c;
        if (remainder < 10) {
            c = '0' + remainder;
        } else {
            c = 'A' + (remainder - 10);
        }
        
        buffer[--index] = c;  // 从后往前填充
        
        in /= base;
    } while (in > 0);
    
    // 添加负号（如果需要）
    if (sign) {
        buffer[--index] = '-';
    }
    
    // 复制结果到输出缓冲区
    strcpy(s, &buffer[index]);// 此时index恰位于缓冲区buffer的首个有效位置
}
