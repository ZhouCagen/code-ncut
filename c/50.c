////猴子第一天摘下若干个桃子，当即吃了一半，还不瘾，又多吃了一个，第二天早上又将剩下的桃子吃掉一半，又多吃了一个。
//// 以后每天早上都吃了前一天剩下的一半零一个。到第n天早上想再吃时，见只剩下一个桃子了。求第一天共摘了多少。
//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int peach(int n)
//{
//	if (n == 1)
//	{
//		return 1;
//	}
//	else
//	{
//		return 2 * (peach(n - 1) + 1);
//	}
//}
//int main()
//{
//	int n;
//	scanf("%d", &n);
//	printf("%d", peach(n));
//	return 0;
//}