//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//void zf(char *a)
//{
//	int j,i=0,c,d=0,len=0;
//	int b[26] = {0};
//	while (a[i++] != '\0')
//	{
//		len++;
//	}
//	for (j = 0; j < len; j++)					
//	{
//		c = 0;
//		c = a[j] - 'a';
//		b[c]++;
//	}
//	int max = b[0];
//	for (i = 1; i < 26; i++)
//	{
//		if (max < b[i])
//		{
//			max = b[i];
//			d = i;
//		}
//	}
//	printf("%c %d\n", d + 'a', max);
//}
//int main()
//{
//	int n, i;
//	char a[100] = { '\0' };
//	scanf("%d", &n);
//	for (i = 1; i <= n; i++)
//	{
//		scanf("%s", a);
//		zf(a);
//	}
//	return 0;
//}