//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<math.h>
//int jinzhi(char *a)
//{
//	int i = 0, digit=0,sum = 0;
//	while (a[i] != '\0')
//	{
//		i++;
//	}
//	for (i--; i >= 0; i--)
//	{
//		if (a[i] >= '0' && a[i] <= '9')
//		{
//			sum += (int)pow(16, digit++) * (a[i] - '0');
//		}
//		else if (a[i] >= 'a' && a[i] <= 'f')
//		{
//			sum += (int)pow(16, digit++) * (a[i] - 'a' + 10);
//		}
//		else if (a[i] >= 'A' && a[i] <= 'F')
//		{
//			sum += (int)pow(16, digit++) * (a[i] - 'A' + 10);
//		}
//	}
//	return sum;
//}
//int main()
//{
//	char m[100] = {'\0'};
//	char n[100] = {'\0'};
//	scanf("%s", m);
//	scanf("%s", n);
//	int sum0;
//	sum0 = jinzhi(m) + jinzhi(n);
//	printf("%d", sum0);
//	return 0;
//}
