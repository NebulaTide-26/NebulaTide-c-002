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

// all预设函数声明：
void a_to_A(char* s);
void A_to_a(char* s);
void deASCLL(char* s, long long n);
int gcd(int a, int b);
int lcm(int a, int b);
long long super_pow_mod(long long base, long long power, long long mod);
long long super_pow(long long base, long long power);
int sushu(long long x);
void zhankai32(int x, char* wei);
void zhankai64(long long x, char* wei);
int translate32(const char* wei);
long long translate64(const char* wei);
double distance_3d(double x1,double y1,double z1, double x2,double y2,double z2);
double fcos_3jiao(double d12, double d23, double d13);
double shoelace_area(Point points[], int n);
unsigned int rotate_left(unsigned int x, int n);
unsigned int rotate_right(unsigned int x, int n);
int day_to_week(int y, int m, int d);
long long jinzhi_to_10(const char* s, int base);
void jinzhi_to_any(long long in, char* s, int base);
int kmp(char *s, char *p);
void getNext(char *p, int next[], int m);
int next_permutation(int *a, int n);


//1.字符串大小写转换（其余不变）：
//小写to大写：
void a_to_A(char* s)
{
	long long n=strlen(s);
	for(int i=0; i<=n-1; i++)
	{
		if(s[i]>='a'&&s[i]<='z')
			s[i]-=('a'-'A');
	}
	return;
}
//大写to小写：
void A_to_a(char* s)
{
	long long n=strlen(s);
	for(int i=0; i<=n-1; i++)
	{
		if(s[i]>='A'&&s[i]<='Z')
			s[i]+=('a'-'A');
	}
	return;
}


//2.字符串数字去ASCII编码（其余不变）：
void deASCLL(char* s, long long n)// n=strlen(s)
{
	for(int i=0; i<=n-1; i++)
	{
		if(s[i]>='0'&&s[i]<='9')
			s[i]-='0';
	}
}
//注意：可能导致数组末尾识别错误！！！


//3.辗转相除计算最大公约数（以int为例）：
int gcd(int a, int b)
{
	int t;
	while(b!=0)
	{
		t=a%b;
		a=b;
		b=t;
	}
	return a;
}


//4.辗转相除计算最小公倍数（以int为例）：
int lcm(int a, int b)
{
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


//5.快速幂乘法:
//带mod版：
long long super_pow_mod(long long base, long long power, long long mod)
{
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
			// 根据公式在最后的的结果上再来一次取余数
	return result % mod;       
}

//不带mod版：
long long super_pow(long long base, long long power)
{
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


//6.判断某数是否为素数（是=1，否=0）:
int sushu(long long x)//yes=1，no=0
{
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


//7.获取32or64位二进制数的每一位，并记入给定空数组wei：
//注意：若为负数，所得为补码！！！
//（从右往左，第i位记为a[i]，从第0位开始
//前置语句：
//char wei[70];
//32位:
void zhankai32(int x, char* wei)
{
	for(int i=0; i<=31; i++)
	{
		wei[i] = (x >> (i)) & 1;
	}
	return;
}
//64位:
void zhankai64(long long x, char* wei)
{
	for(int i=0; i<=63; i++)
	{
		wei[i] = (x >> (i)) & 1;
	}
	return;
}


//8.将二进制展开的数组还原为十进制数：
//（参考7.；仅限于非负数）
//需引入“快速幂super_pow”
//32位：
int translate32(const char* wei)
{
	int sum=0;
	for(int i=0; i<=31; i++)
	{
		sum += (int)wei[i]*(int)super_pow(2,(long long)i);
	}
	return sum;
}

//64位：
long long translate64(const char* wei)
{
	long long sum=0;
	for(int i=0; i<=63; i++)
	{
		sum += (long long)wei[i]*super_pow(2,(long long)i);
	}
	return sum;
}


//9.几何计算：
//三维空间两点距离计算：
double distance_3d(double x1,double y1,double z1, double x2,double y2,double z2)
{
	double d;
	d = sqrt( pow((x1-x2),2) + pow((y1-y2),2) + pow((z1-z2),2) );
	return d;
}

//给出三角形三边长，算第三个参量边所对的内角余弦值：
double fcos_3jiao(double d12, double d23, double d13)
{
	double cos2;
	
	cos2 = (d12*d12 + d23*d23 - d13*d13) / (2*d12*d23);
	
	return cos2;
}

//给出三角形角坐标，计算面积：
// 需预设结构体：（用来输入所有n个点的坐标）
typedef struct {
	 double x, y;
} Point;

double shoelace_area(Point points[], int n) {
    if (n < 3) return 0.0;  // 不是多边形

    double area = 0.0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;  // 下一个顶点（最后一个点对应的j是points[0]）
        area += points[i].x * points[j].y - points[j].x * points[i].y;// 二维向量叉乘模长公式
    }
    area = fabs(area) / 2.0;
    return area;
}


//10.循环移位：
//将x循环左移n位
unsigned int rotate_left(unsigned int x, int n)
{
    int bits = sizeof(x) * 8;
    n %= bits;
    return (x << n) | (x >> (bits - n));
}
//将x循环右移n位
unsigned int rotate_right(unsigned int x, int n)
{
    int bits = sizeof(x) * 8;
    n %= bits;
    return (x >> n) | (x << (bits - n));
}
//注：若n可能等于0或bits的整数倍，需排除此情况！！！


//11.给定日期计算星期几（y年m月d日）（兼容1582.10.4及以前）：
int day_to_week(int y, int m, int d)
{
	int w, c, t=-1;
	if(y<=1582 && m<=10 && d<=4)
		t=2;
	if(m<3)
	{
		y--;
		m+=12;
	}
	c=y/100;
	y%=100;
	w=(c/4-2*c+y+y/4+26*(m+1)/10+d+t)%7;
	if(w<=0) w+=7;
	return w;
}


//12.泛用版非负整数进制转换（基于long long）:
//2~36进制转10进制：
//前置：将原base进制数记录在字符串s中
//（不区分大小写，不区分是否为ASCII编码）
long long jinzhi_to_10(const char* s, int base)// 存储区，原进制
{
    long long out = 0;
    char c;
    for (int i = 0; s[i] != '\0'; i++)
    {
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
        if (out < 0) 
            return -1;//溢出处理
    }
    return out;
}

//10进制转2~36进制：
//结果以ASCII编码的形式（大写字母）存在预留的字符串s中
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


//13.低时间复杂度字符串匹配函数（KMP）及其辅助函数
int kmp(char *s, char *p) {// 主串，模式串
    int n = strlen(s);        // 主串长度
    int m = strlen(p);        // 模式串长度
    if (m == 0) return 0;     // 空模式串默认匹配开头
	
    // 动态分配 next 数组
    int *next = (int *)malloc(m * sizeof(int));
    if (!next) return -1;     // 内存分配失败，指针next值为NULL
	
    getNext(p, next, m);      // 调用函数，构建next数组
	
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

    free(next);               // 释放 next 数组

    if (j == m) {
        // 匹配成功，返回主串中起始位置
        return i - j;
    } else {
        return -1;// 匹配失败
    }
}
void getNext(char *p, int next[], int m) {// 模式串，next数组，当前计算的next数组下标
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


//14.全排列函数（O(n × n!)）
/*
// 使用方法：
do {
	
	} while ( next_permutation(a, n) );
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


//以下为测试区~！！！！！！！！！！！！！！！！！！！！！！！！//

char s[70];
int main()
{
	int a, b, c;
	while(scanf("%d%d%d", &a, &b, &c)!=EOF)
		printf("%d\n", day_to_week(a, b, c));
	
	return 0;
}
