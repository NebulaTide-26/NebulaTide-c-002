#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>//qsort
#include <errno.h>//检查strtol转换溢出问题→"errno"
#define eps 1e-8

double a[1000];
double b[1000];

int main()
{
	printf("请以“学分 分数 学分 分数 ...”的形式输入数据\n");
	//a[i]=各科学分, b[i]=各科分数
	int i=1;
	
	while(scanf(" %lf %lf", &a[i], &b[i])!=EOF)
	{
		i++;
//		printf("%d\n", i);
	}
	
	double zi1=0, zi2=0, mu=0, sgpa=0;
	
	for(int j=1; j<=i-1; j++)
	{
		zi1+=a[j]*b[j];//加权分数和
		zi2+=b[j];//分数和
		mu+=a[j];//总学分
		sgpa+=a[j]*(4-3*(100-b[j])*(100-b[j])/1600);
	}
	
	printf("number of subjects: %d\n", i-1);//学科数
	printf("Total credits: %.1lf\n", mu);//总学分
	printf("Weighted average score: %.2lf\n", zi1/mu);//加权平均分
	printf("Unweighted average score: %.2lf\n", zi2/(i-1));//算术平均分
	printf("Weighted average GPA: %.2lf\n", sgpa/mu);//加权平均gpa
	
return 0;
}
//5 69 4 77 2 84 0.5 85 0.5 86 1 70 3 86 0.2 100 3 89 2 100 0.5 86 0.2 83 0.3 100 0.2 100 1 86 1.5 90
