#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>//qsort
#include <errno.h>//检查strtol转换溢出问题→"errno"
#define eps 1e-8



int main()
{
	double x;
	while(scanf("%lf", &x)!=EOF)
	{
		printf("gpa=%.2lf\n", 4-3*(100-x)*(100-x)/1600);
		
	}
return 0;
}
