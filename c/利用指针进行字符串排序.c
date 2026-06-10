////问题描述
////
////在主函数中输入n个的字符串。用另一函数用指针方法对它们按照ASCII码从小到大的顺序排序。然后在主函数输出这n个已排好序的字符串。（n不大于10，每个字符串的长度不大于80）
////
////输入形式
////
////输入n + 1行，第一行是正整数n，第二行到第n + 1行是n个字符串。
////
////输出形式
////
////输出n行，给出排序好的n个字符串。
//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//int greater(char* a, char* b)
//{
//	while (*a != '\0' && *b != '\0')
//	{
//		if (*a > *b) return 1;
//		if (*a < *b) return -1;
//		a++; b++;
//	}
//
//	if (*a == '\0') return 0;
//	return 0;
//}
//
//void sort(char **arr, int n)
//{
//	int i, j;
//	char * t;
//	for (i = 0; i < n - 1; i++)
//	{
//		for (j = 0; j < n - i - 1; j++)
//		{
//			printf("%d %d", i, j);
//			if (greater(*arr[j], *arr[j + 1]) > 0)
//			{
//				
//				t = arr[j];
//				arr[j] = arr[j+1];
//				arr[j + 1] = t;
//			}
//		}
//	}
//
//}
//
//int main()
//{
//	int n, i, j = 0;
//	scanf("%d", &n);
//	getchar();
//	char arr[10][80] = { {'\0'} };
//	for (i = 0; i < n; i++)
//	{
//		/*while (scanf("%c", &ch) && ch != '\n')
//		{
//			arr[i][j++] = ch;
//		}*/
//		gets(arr[i]);
//	}
//	sort(arr, n);
//	for (i = 0; i < n; i++)
//	{
//		printf("%s", arr[i]);
//	}
//	return 0;
//}