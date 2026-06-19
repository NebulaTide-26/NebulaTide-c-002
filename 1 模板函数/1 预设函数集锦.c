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



// 需要用到的结构体
// 几何计算-点
typedef struct Point {
	double x, y, z;
} intpoint;



// all预设函数声明：
// 1.常规算术计算
int gcd(int a, int b);													// 辗转相除计算最大公约数（以int为例）
int lcm(int a, int b);													// 辗转相除计算最小公倍数（以int为例）
long long super_pow_mod(long long base, long long power, long long mod);// 快速幂乘法-带mod版
long long super_pow(long long base, long long power);					// 快速幂乘法-不带mod版
int sushu(long long x);													// 判断某数是否为素数

// 2.字符串相关
void a_to_A(char* s);													// 大小写转换-小写to大写
void A_to_a(char* s);													// 大小写转换-大写to小写
void deASCLL(char* s, long long n);										// 字符串数字去ASCII编码（其余不变）
int kmp(char *s, char *p);												// kmp字符串匹配函数
void basic_kmp_getNext(char *p, int next[], int m);						// （basic）kmp函数辅助函数
void string_wash(char *s, int option);									// 字符串清洗

// 3.几何计算
double distance_3d(intpoint a, intpoint b);								// 三维空间两点距离计算
double fcos_3jiao(double d12, double d23, double d13);					// 给出三角形三边长，算第三个参量边所对的内角余弦值
double shoelace_area(intpoint *p, int n);								// 给出n边形顶点坐标，计算面积

// 4.计算机数据处理（位运算等）
int* zhankai(long long x, int wei);										// 获取32or64位二进制数的每一位，记入数组中
long long dezhankai(const int *s, int wei);								// 将二进制展开的数组还原为十进制数
unsigned int rotate(unsigned int x, int n, int target);					// 循环移位
long long jinzhi_to_10(const char* s, int base);						// 2~36进制（非负整数）转10进制
char* jinzhi_to_any(long long in, int base);							// 10进制（整数）转2~36进制

// 5.特殊算术计算
char* infix_to_postfix(char *s);										// 算术（中缀）表达式->后缀表达式（含括号、^）
double count_postfix(char *s);											// 后缀表达式计算



// 6.其他
int day_to_week(int y, int m, int d);									// 给定日期计算星期几（y年m月d日）（兼容1582.10.4及以前）
int next_permutation(int *a, int n);									// 全排列函数（O(n × n!)）




// 1.常规算术计算
/* 辗转相除计算最大公约数（以int为例）
*/
int gcd(int a, int b) {
	int t;
	while(b!=0)
	{
		t=a%b;
		a=b;
		b=t;
	}
	return a;
}

/* 辗转相除计算最小公倍数（以int为例）
*/
int lcm(int a, int b) {
	int a1, b1;
	a1=a;
	b1=b;
	
	int t;
	while(b1!=0)
	{
		t=a1%b1;
		a1=b1;
		b1=t;
	}
	
	return a * b / a1;
}

/* 快速幂乘法
*/
//带mod版：
long long super_pow_mod(long long base, long long power, long long mod) {
	long long result = 1;   
	while (power > 0)           
	{
		if (power & 1)         							
			result = result * base % mod;   
			// 根据公式每个项都取余数后在再做累乘
		base = base * base % mod ;   
			// 根据公式每个项都取余数后在再做平方操作      						
		power >>= 1;         						
	}
	return result % mod;// 根据公式在最后的结果上再来一次取余数
}
//不带mod版：
long long super_pow(long long base, long long power) {
	long long result = 1;
	while (power > 0)           //指数大于0进行指数折半，底数变其平方的操作
	{
		if (power & 1)			//指数为奇数，power & 1这相当于power % 2 == 1
			result *= base;     //分离出当前项并累乘后保存
		power >>= 1;			//指数折半,power >>= 1这相当于power /= 2;
		base *= base;           //底数变其平方
	}
	return result;              //返回最终结果
}

/* 判断某数是否为素数
   返回值：是：1 否：0
*/
int sushu(long long x) {
	if(x == 1)
		return 0;
	else if(x == 2 || x == 3)
		return 1;
	else if(x %6 != 5 && x %6 != 1)
		return 0;
	else
	{
		for(int i=5; i*i <= x; i+=6)
		{
			if(x % i == 0 || x % (i+2) == 0)
				return 0;
		}
		return 1;
	}
}



// 2.字符串相关
/* 字符串大小写转换（其余不变）
*/
//小写to大写：
void a_to_A(char* s) {
	long long n=strlen(s);
	for(int i=0; i<=n-1; i++)
	{
		if(s[i]>='a'&&s[i]<='z')
			s[i]-=('a'-'A');
	}
	return;
}
//大写to小写：
void A_to_a(char* s) {
	long long n=strlen(s);
	for(int i=0; i<=n-1; i++)
	{
		if(s[i]>='A'&&s[i]<='Z')
			s[i]+=('a'-'A');
	}
	return;
}

/* 字符串数字去ASCII编码（其余不变）
   
   注：可能导致数组末尾识别错误！！！！！
*/
void deASCLL(char* s, long long n) {// n=strlen(s)
	for(int i=0; i<=n-1; i++) {
		if(s[i]>='0'&&s[i]<='9')
			s[i]-='0';
	}
}

/* 低时间复杂度字符串匹配（kmp及其辅助函数basic_kmp_getNext）
   返回值：模式串在主串中第一次出现位置的数组序号（匹配失败、模式串为空等错误返回-1）
*/
int kmp(char *s, char *p) {// 主串，模式串
    int n = strlen(s);        // 主串长度
    int m = strlen(p);        // 模式串长度
    if (m == 0) return -1;
	
    // 动态分配 next 数组
    int *next = (int *)malloc(m * sizeof(int));
    if (!next) return -1;     // 内存分配失败，指针next值为NULL
	
    basic_kmp_getNext(p, next, m);      // 调用函数，构建next数组
	
    int i = 0;  // 主串序号
    int j = 0;  // 模式串序号

    while (i < n && j < m) {
        if (j == -1 || s[i] == p[j]) {
            // j=-1代表当前主串位置与p[0]都无法匹配
            ++i;
            ++j;// 若原来j=-1，此时恰指向p[0]
        } else {
            // 匹配失败时，模式串指针跳转到 next[j] 指示的位置
            j = next[j];
        }
    }

    free(next);// 释放 next 数组

    if (j == m) {
        // 匹配成功，返回主串中起始位置
        return i - j;
    } else {
        return -1;// 匹配失败
    }
}
void basic_kmp_getNext(char *p, int next[], int m) {// 模式串，next数组，当前计算的next数组下标
    // next[0] 固定为 -1，表示第一个字符失配时，模式串指针退无可退，通过-1使i得以++
    next[0] = -1;
    int k = -1;          // k 代表当前已匹配的最长相同前后缀长度（同时也是索引）
    int j = 0;           // j 遍历模式串，用来计算 next[j+1]

    while (j < m - 1) {  // 只需要计算到 next[m-1] 即可，因为 next[m] 不会用到
        if (k == -1 || p[j] == p[k]) {
            // 若 k 为 -1，说明前缀已经匹配到头，此时可以扩展
            // 若 p[j] == p[k]，说明可以增加相同前后缀长度
            ++k;
            ++j;
            next[j] = k;      // 记录 next[j] 为当前最长相同前后缀长度 k
        } else {
            // 字符不匹配，需要回退 k 到之前的最长相同前后缀位置
            k = next[k];
        }
    }
}

/* 字符串清洗
   
   函数行为约定：去除首位空格及内部“多余空格”
   option：0：保留词间空格 1：去除所有空格
*/
void string_wash(char *s, int option) {
	int len = strlen(s);
	char *t = (char*)malloc(len+1);
	int i = 0, j = 0;
	while(s[i] == ' ') i++;// 跳过前导空格
	
	switch(option) {
		case 0:{
			for(; i<len; i++) {
				if(j == 0) {
					t[j++] = s[i];
					continue;
				}
				// 此时j>=1
				if(s[i] == ' ' && t[j-1] == ' ') {
					continue;
				}
				t[j++] = s[i];
			}
			t[j] = '\0';
			break;
		}
		case 1:{
			for(; i<len; i++) {
				if(j == 0) {
					t[j++] = s[i];
					continue;
				}
				// 此时j>=1
				if(s[i] == ' ') {
					continue;
				}
				t[j++] = s[i];
			}
			t[j] = '\0';
			break;
		}
	}
	strcpy(s, t);
	free(t);
	if(strlen(s) == 0) {
		return;
	}
	int tail = strlen(s);
	while (tail > 0 && s[tail - 1] == ' ') {
		s[--tail] = '\0';
	}
	return;
}



// 3.几何计算
/* 三维空间两点距离计算
*/
double distance_3d(intpoint a, intpoint b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

/* 给出三角形三边长，算第三个参量边所对的内角余弦值
*/
double fcos_3jiao(double d12, double d23, double d13) {
	return (d12*d12 + d23*d23 - d13*d13) / (2*d12*d23);
}

/* 给出n边形顶点坐标，计算面积
   返回值：面积（错误返回-1.0）
   
   函数行为约定：传入的点z分量为0（不用z）
*/
double shoelace_area(intpoint *p, int n) {// n：边数，于points数组项数对应
    if (n < 3) return 0.0;// 不是多边形
	
    double s = 0.0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;  // 下一个顶点（最后一个点i对应的j是p[0]）
        s += p[i].x * p[j].y - p[j].x * p[i].y;// 二维向量叉乘公式
    }
    s = fabs(s) / 2.0;
    return s;
}



// 4.计算机数据处理（位运算等）
/* 获取32or64位二进制数的每一位，记入数组中
   返回值：二进制展开后的结果数组
   
   注：
   x从右往左第i位记为a[i]，从第0位开始
   若为负数，所得为补码！！！
*/
int* zhankai(long long x, int wei) {// wei：位数，int取32, long long取64
	int *s = (int*)malloc(sizeof(int)*wei);
	
	switch(wei) {
		case 32:{
			for(int i = 0; i < 32; i++) {
				s[i] = (x >> i) & 1;
			}
			break;
		}
		case 64:{
			for(int i = 0; i < 64; i++) {
				s[i] = (x >> i) & 1ll;// 之所以不能合并
			}
			break;
		}
	}
	return s;
}

/* 将二进制展开的数组还原为十进制数
   返回值：统一返回long long型数据
   
   注：
   参考7.；仅限于非负数
   需要引入快速幂“super_pow”函数
*/
long long dezhankai(const int *s, int wei) {// wei：位数，int取32, long long取64
	long long sum = 0;
	for(int i = 0; i <= wei - 1; i++) {
		sum += (long long)s[i] * super_pow(2, (long long)i);
	}
	return sum;
}

/* 循环移位
   
   target：-1=左移 1=右移
*/
unsigned int rotate(unsigned int x, int n, int target) {
	int bits = sizeof(x) * 8;// 后序移位操作对其他数据类型也适用
    n %= bits;
    switch (target) {
    	case -1:{
    		return (x << n) | (x >> (bits - n));
			break;
		}
		case 1:{
			return (x >> n) | (x << (bits - n));
			break;
		}
	}
	return 0;
}

/* 泛用版非负整数进制转换（基于long long）
*/
/* 2~36进制（非负整数）转10进制
   返回值：10进制long long整数（溢出返回-1）
   注：
   调用前将原base进制数记录在字符串s中
   不区分大小写，不区分是否为ASCII编码
*/
long long jinzhi_to_10(const char* s, int base) {// 数组存储区，原进制
    long long out = 0;
    char c;
    for (int i = 0; s[i] != '\0'; i++) {
        c = s[i];
        
        //若为字符，转换
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        
        //利用霍纳法则：out = out * base + digit
        out = out * base + c;
        
        //检查溢出
        if (out < 0) return -1;// 溢出处理
    }
    return out;
}
/* 10进制（整数）转2~36进制
   返回值：以ASCII编码的形式（大写字母）存在字符串中
*/
char* jinzhi_to_any(long long in, int base) {// 被转换数字，目标进制
	char *s = (char*)malloc(66);
    // 处理0
    if (in == 0) {
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    char temp[65];  // 临时缓冲区
    int p = 64;   // 从末尾开始
    temp[64] = '\0';  // 字符串结束符
    
    // 处理负数
    int sign = 0;
    if (in < 0) {
        sign = 1;
        in = -in;
    }
    
    // 转换过程
    do {
        int yvshu = in % base;
        
        // 转换为字符
        char c;
        if (yvshu < 10) {
            c = '0' + yvshu;
        } else {
            c = 'A' + (yvshu - 10);
        }
        
        temp[--p] = c;  // 从第temp[63]开始往前填充
        in /= base;
    } while (in > 0);
    
    // 添加负号（如果需要）
    if (sign) {
        temp[--p] = '-';
    }
    
    // 复制结果到输出缓冲区
    strcpy(s, &temp[p]);// 此时p恰位于缓冲区temp的首个有效位置
    return s;
}



// 6.其他
/* 给定日期计算星期几（y年m月d日）（兼容1582.10.4及以前）
   返回值：星期几（1~7）
*/
int day_to_week(int y, int m, int d) {
	int w, c, t = -1;
	if(y<=1582 && m<=10 && d<=4) t = 2;// 对1582.10.4及以前的特殊处理
	
	if(m < 3) {
		y--;
		m+=12;
	}
	c = y / 100;
	y %= 100;
	w = (c/4 - 2*c + y + y/4 + 26*(m+1)/10 + d + t) % 7;
	if(w <= 0) w += 7;
	return w;
}

/* 全排列函数（O(n × n!)）
   返回值：1（已经全为倒序时返回-1）
   
   使用方法：
   do {
	
   } while (next_permutation(a, n));
*/
int next_permutation(int *a, int n) {// 数组，n个数
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;// i为第一个s.t. a[i]<a[i+1] 的角标
    if (i < 0) return 0;// 判断是否为全倒序→结束
    
    int j = n - 1;
    while (a[j] <= a[i]) j--;// i为第一个s.t. a[j]>a[i] 的角标
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;// 交换ai、aj
    for (int l = i + 1, r = n - 1; l < r; l++, r--) {// 将i+1~n部分倒序
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return 1;
}





// 5.特殊算术计算
/* 字符串运算式计算（四则->后缀表达式、后缀表达式计算）（整数）
*/
/* 算术（中缀）表达式->后缀表达式（含括号、^）
   返回值：后缀表达式数组（错误可视化处理并返回NULL）
   函数行为约定：-a视为0-a
*/
char* infix_to_postfix(char *s) {
	// 数据预处理
	int len = strlen(s);
	char *t = (char*)malloc(len+1);
	int i = 0, j = 0;
	while(s[i] == ' ') i++;// 跳过前导空格
	
	for(; i<len; i++) {
		if(j == 0) {
			t[j++] = s[i];
			continue;
		}
		// 此时j>=1
		if(s[i] == ' ') {
			continue;
		}
		t[j++] = s[i];
	}
	t[j] = '\0';
	
	strcpy(s, t);
	free(t);
	if(strlen(s) == 0) {
		t = (char*)malloc(2);
		t[0] = '\0';
		return t;
	}
	int tail = strlen(s);
	while (tail > 0 && s[tail - 1] == ' ') {// 去除末尾空格
		s[--tail] = '\0';
	}
	if(s[strlen(s)-1] == '=') {// 去除可能的‘=’
		s[strlen(s)-1] = '\0';
	}
	
	if(strstr(s, "--")!=NULL) {
		printf("Invalid expression.\n");
		return NULL;
	}
	
	// 处理-a情况
	t = (char*)malloc(strlen(s)*2);
	i = j = 0;
	if(s[i] == '-') {
		t[j++] = '0';
		t[j++] = '-';
		i++;
	}
	for(; i<strlen(s); i++) {
		if(s[i]=='-' && (s[i-1]=='+'||s[i-1]=='-'||s[i-1]=='*'||s[i-1]=='/'||s[i-1]=='^'||s[i-1]=='(')) {
			t[j++] = '0';
		}
		t[j++] = s[i];
	}
	t[j++] = '\0';
	
	// 转化为结构体形式（以便处理多位数字）
	typedef struct {
		int sign;	// 0：算符 1：数字
		char op;	// +-*÷^
		int n;		// 数字
	} inta1;
	inta1 *list = (inta1*)malloc(sizeof(inta1)*strlen(t));// 大小绝对够
	int count = 0;// 当前list中数据数
	i = 0;
	while(i < strlen(t)) {
		if(t[i]>='0' && t[i]<='9') {
			sscanf(t + i, "%d", &list[count].n);
			while(t[i]>='0' && t[i]<='9') {// 定位到下一个符号
				i++;
			}
			list[count++].sign = 1;
		} else {
			sscanf(t + i, "%c", &list[count].op);
			i++;
			list[count++].sign = 0;
		}
	}
	
	// 生成后缀表达式（从list到out）
	char *out = (char*)malloc(strlen(t)*2+1);// 输出队
	for(int i = 0; i <= strlen(t)*2; i++) {// out数组归0
		out[i] = 0;
	}
	
	char *stuck = (char*)malloc(count);// 符号栈
	int top = -1;// 实时栈顶位置
	for(int i=0; i<count; i++) {
		switch(list[i].sign) {
			case 0:{// 符号
				char temp[3];
				temp[1] = ' ';
				temp[2] = '\0';
				if(list[i].op == '(') {
					stuck[++top] = '(';
				} else if(list[i].op == ')') {
					while(stuck[top] != '(') {
						if(top == 0) {// 运算式有bug
							printf("Invalid expression.\n");
							free(stuck);
							free(out);
							free(list);
							free(t);
							return NULL;
						}
						temp[0] = stuck[top--];// 符号出栈
						strcat(out, temp);
					}
					top--;// 舍弃'('
				} else {
					if(list[i].op == '^') {
						stuck[++top] = '^';
					} else if(list[i].op == '*' || list[i].op == '/') {
						while(top>=0 && (stuck[top]=='*' || stuck[top]=='/' || stuck[top]=='^')) {
							out[strlen(out)] = stuck[top--];
							out[strlen(out)] = ' ';
						}
						stuck[++top] = list[i].op;
					} else if(list[i].op == '+' || list[i].op == '-') {
						while(top >= 0 && (stuck[top]!='(')) {
							out[strlen(out)] = stuck[top--];
							out[strlen(out)] = ' ';
						}
						stuck[++top] = list[i].op;
					}
				}
				break;
			}
			case 1:{// 数字
				char temp[100];
				sprintf(temp, "%d ", list[i].n);
				strcat(out, temp);
				break;
			}
		}
	}
	
	while(top >= 0) {// 清空符号栈
		out[strlen(out)] = stuck[top--];
		out[strlen(out)] = ' ';
	}
	free(stuck);
	free(list);
	free(t);
	for(int i = strlen(out) - 1; i >= 0; i--) {// 清除末尾空格
		if(out[i]==' ') {
			out[i] = '\0';
		} else break;
	}
	return out;
}
/* 后缀表达式计算
   返回值：（double型）计算结果（错误可视化处理并返回-1.0）
*/
double count_postfix(char *s) {
	if(s == NULL) {
		printf("Invalid expression.\n");
		return -1.0;
	}
	
	int len = strlen(s);
	double *stuck = (double*)malloc(sizeof(double)*len);// 存放数字
	int top = -1;// 实时栈顶位置
	
	// 转化为结构体形式（以便处理多位数字）
	typedef struct {
		int sign;	// 0：算符 1：数字
		char op;	// +-*÷^
		double n;		// 数字
	} inta1;
	inta1 *list = (inta1*)malloc(sizeof(inta1)*len);
	int count = 0;// 当前list中数据数
	for(int i=0; i<len;) {
		if(s[i] == ' ') {
			i++;
		} else if(s[i]>='0' && s[i]<='9') {
			int t;
			sscanf(s + i, "%d", &t);
			list[count].n = (double)t;
			list[count++].sign = 1;
			while(s[i]>='0' && s[i]<='9') i++;
		} else {// 符号
			sscanf(s + i, "%c", &list[count].op);
			list[count++].sign = 0;
			i++;
		}
	}
	
	// 计算
	for(int i=0; i<count; i++) {
		switch(list[i].sign) {
			case 0:{
				if(top <= 0) {
					printf("Invalid expression.\n");
					free(stuck);
					free(list);
					return -1.0;
				}
				double b = stuck[top--];
				double a = stuck[top--];
				switch(list[i].op) {
					case '+':{
						stuck[++top] = a + b;
						break;
					}
					case '-':{
						stuck[++top] = a - b;
						break;
					}
					case '*':{
						stuck[++top] = a * b;
						break;
					}
					case '/':{
						if(b == 0.0) {
							printf("0 cannot be a divisor.\n");
							free(stuck);
							free(list);
							return -1.0;
						}
						stuck[++top] = a / b;
						break;
					}
					case '^':{
						stuck[++top] = pow(a, b);
						break;
					}
				}
				break;
			}
			case 1:{
				stuck[++top] = list[i].n;
				break;
			}
		}
	}
	
	// 错误检查
	if(top != 0) {
		printf("Invalid expression.\n");
		free(stuck);
		free(list);
		return -1.0;
	}
	
	double out = stuck[0];
	free(list);
	free(stuck);
	return out;
}

/* 超长整数域运算（+-*÷）
   返回值：运算结果数组
   
   option：1 2 3 4对应+ - * ÷
*/
char* long_operation(char *a, char *b, int option) {
	// 处理自带符号问题
	int sign = 0;
	char *t0, *out;
	if(a[0]=='-' && b[0]=='-') {
		switch(option) {
			case 1:{
				sign = 1;
				t0 = long_operation(a+1, b+1, 1);
				break;
			}
			case 2:{
				return long_operation(b+1, a+1, 2);
				break;
			}
			case 3:{
				return long_operation(a+1, b+1, 3);
				break;
			}
			case 4:{
				return long_operation(a+1, b+1, 4);
				break;
			}
		}
	} else if(a[0]=='-') {
		switch(option) {
			case 1:{
				return long_operation(b, a+1, 2);
				break;
			}
			case 2:{
				sign = 1;
				t0 = long_operation(a+1, b, 1);
				break;
			}
			case 3:{
				sign = 1;
				t0 = long_operation(a+1, b, 3);
				break;
			}
			case 4:{
				sign = 1;
				t0 = long_operation(a+1, b, 4);
				break;
			}
		}
	} else if(b[0]=='-') {
		switch(option) {
			case 1:{
				return long_operation(a, b+1, 2);
				break;
			}
			case 2:{
				return long_operation(a, b+1, 1);
				break;
			}
			case 3:{
				sign = 1;
				t0 = long_operation(a, b+1, 3);
				break;
			}
			case 4:{
				sign = 1;
				t0 = long_operation(a, b+1, 4);
				break;
			}
		}
	}
	
	if(sign) {// 只需向t0前添加负号
		out = (char*)malloc(strlen(t0) + 2);
		out[0] = '-';
		strcpy(out + 1, t0);
		free(t0);
		return out;
	}
	// 此时：a b均为非负整数（首位为数字）
	
	while(a[0] == '0' && strlen(a) > 1) a++;// 删除a前导0
	while(b[0] == '0' && strlen(b) > 1) b++;// 删除b前导0
	// 转成两个整数数组
	int len1 = strlen(a), len2 = strlen(b), len3;
	int *x = (int*)malloc(sizeof(int)*len1), *y = (int*)malloc(sizeof(int)*len2);
	for(int i=0; i<len1; i++) {
		x[i] = a[i] - '0';
	}
	for(int i=0; i<len2; i++) {
		y[i] = b[i] - '0';
	}
	sign = 0;
	
//	for(int i=0; i<len1; i++) {
//		printf("%d", x[i]);
//	}printf("\n");
//	for(int i=0; i<len2; i++) {
//		printf("%d", y[i]);
//	}printf("\n");
	
	switch(option) {
		case 1:{// +
			int *z = (int*)malloc(sizeof(int)*(len1+len2+3));// 肯定够长
			len3 = len1 + len2 + 3;
			for(int i=0; i<len3; i++) {
				z[i] = 0;// z数组归0
			}
			
			// 处理相加
			int jin = 0;// 记录进位
			for(int i=len1-1, j=len2-1, k=0; ; i--, j--) {
				if(i>=0 && j>=0) {
					z[k++] = (x[i] + y[j] + jin) % 10;// z中以倒序存储
					jin = (x[i] + y[j] + jin) / 10;
				} else if(i >= 0) {
					z[k++] = (x[i] + jin) % 10;
					jin = (x[i] + jin) / 10;
				} else if(j >= 0) {
					z[k++] = (y[j] + jin) % 10;
					jin = (y[j] + jin) / 10;
				}
				else {
					z[k++] = jin;
					break;
				}
			}
			int t = len3 - 1;
			while(t > 0 && z[t] == 0) t--;// 找到z的最后一个有效位（最高位位置）
			for(int p = 0, q = t; p < q; p++, q--) {// z倒；t对应最低位
				z[p] = z[p]^z[q];
				z[q] = z[p]^z[q];
				z[p] = z[p]^z[q];
			}
			
			// 反转回字符串形式
			out = (char*)malloc(t + 2);
			for(int i=0; i<=t; i++) {
				out[i] = z[i] + '0';
			}
			out[t + 1] = '\0';
			free(z);
			break;
		}
		case 2:{// -
			int *z = (int*)malloc(sizeof(int)*(len1+len2));// 肯定够长
			len3 = len1 + len2;
			for(int i=0; i<len3; i++) {
				z[i] = 0;// z数组归0
			}
			
			// 判断x-y的正负
			if(len1 < len2) {
				sign = 1;// 此处sign=1 代表负数
				int *x1;
				x1 = x;
				x = y;
				y = x1;
				len1 = len1^len2;
				len2 = len1^len2;
				len1 = len1^len2;
			} else if(len1 == len2) {
				for(int i=0; i<len1; i++) {
					if(x[i] > y[i]) {
						break;
					} else if(x[i] < y[i]) {
						sign = 1;
						int *x1;
					x1 = x;
					x = y;
					y = x1;
					len1 = len1^len2;
					len2 = len1^len2;
					len1 = len1^len2;
						break;
					}
				}
				// 此时：若sign仍为0，说明x>=y
			}
			
			// 处理相减
			int jin = 0;
			for(int i=len1-1, j=len2-1, k=0; ; i--, j--) {// jin=1代表借一位
				if(i>=0 && j>=0) {
					if(x[i] - y[j] - jin < 0) {
						z[k++] = x[i] - y[j] - jin + 10;
						jin = 1;
					} else {
						z[k++] = x[i] - y[j] - jin;
						jin = 0;
					}
				} else if(i >= 0) {
					if(x[i] - jin < 0) {
						z[k++] = x[i] - jin + 10;
						jin = 1;
					} else {
						z[k++] = x[i] - jin;
						jin = 0;
					}
				} else {
					break;// jin不可能还有未结算部分
				}
				// （不存在x比y位数多的情况）
			}
			int t = len3 - 1;
			while(t > 0 && z[t] == 0) t--;// 找到z的最后一个有效位（最高位位置）
			for(int p = 0, q = t; p < q; p++, q--) {// z倒；t对应最低位
				z[p] = z[p]^z[q];
				z[q] = z[p]^z[q];
				z[p] = z[p]^z[q];
			}
			
			// 反转回字符串形式
			out = (char*)malloc(t + 2 + sign);
			if(sign) {// 处理sign
				out[0] = '-';
			}
			for(int i = sign; i <= t + sign; i++) {
				out[i] = z[i - sign] + '0';
			}
			out[t + 1 + sign] = '\0';
			free(z);
			break;
		}
		case 3:{// *
			int *z = (int*)malloc(sizeof(int)*(len1 + len2 + 3));// 肯定够长
			len3 = len1 + len2 + 3;
			for(int i = 0; i < len3; i++) {
				z[i] = 0;// z数组归0
			}
			
			// 处理相乘
			for(int i = len1 - 1; i >= 0; i--) {// 将逐位相乘结果累加到z的对应位置
				for(int j = len2 - 1; j >= 0; j--) {
					int p0 = len1 + len2 - i - j - 2;// 移位量，代表合并进z时往高位移动p0位
					z[p0] += x[i]*y[j];
				}
			}
			int jin = 0;
			for(int i = 0; i < len3; i++) {// 处理z中某位>=10的问题
				z[i] += jin;
				jin = z[i] / 10;
				z[i] %= 10;
			}
			
			int t = len3 - 1;
			while(t > 0 && z[t] == 0) t--;// 找到z的最后一个有效位（最高位位置）
			for(int p = 0, q = t; p < q; p++, q--) {// z倒；t对应最低位
				z[p] = z[p]^z[q];
				z[q] = z[p]^z[q];
				z[p] = z[p]^z[q];
			}
			
			// 反转回字符串形式
			out = (char*)malloc(t + 2);
			for(int i = 0; i <= t; i++) {
				out[i] = z[i] + '0';
			}
			out[t + 1] = '\0';
			
			
			
			free(z);
			break;
		}
		case 4:{// ÷
			if(y[0] == 0) {
				printf("0 cannot be a divisor.\n");
				free(x);
				free(y);
				return NULL;
			}
			int *z = (int*)malloc(sizeof(int)*(len1 + 2));
			len3 = len1 + 1;
			for(int i = 0; i < len3; i++) {
				z[i] = 0;// z数组归0
			}
			
			// （待完善）
			
			
			
			break;
		}
	}
	free(x);
	free(y);
	if(strcmp(out, "-0") == 0) {
		out = realloc(out, 2);
		strcpy(out, "0");
	}
	return out;
}





//以下为测试区~！！！！！！！！！！！！！！！！！！！！！！！！//
int main() {
	
	char s[100], s1[100];
	int t;
	while(gets(s)!=NULL) {
		gets(s1);
		scanf("%d", &t);
		getchar();
		char *out = long_operation(s, s1, t);
		puts(out);
	}
	return 0;
}




