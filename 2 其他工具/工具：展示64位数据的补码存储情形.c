#include <stdio.h>
#include <math.h>

//展示64位数据的补码存储情形
//输入的数据为a

int main()
{
	long long at, it;
	
	
	while(scanf("%lld", &at)!=EOF)
	{
		it = 64;
	while(it--)
	{
		printf("%lld", (at>> (it) ) &1 );
		
		if (it%4 == 0 && it != 0){
		printf(" ");
		}
	}
	printf("\n");
	}

return 0;
}
