/*问题描述】按字符串比较过程对给定的两个字符串进行比较，若相等则结果为0，若不等则结果为对应不等字符的差。按要求实现对给定字符串的比较。要求程序中用函数实现两个字符串的比较。int stringcmp(char* str1, char* str2)
【输入形式】第一行为一个整数N，表示测试数据的组数，接下来的N行中每行包含两个字符串，字符串之间由空格分隔。
【输出形式】输出共N行，每行为对应输入一行的两个字符串的比较结果。
【样例输入】3
Happy Help
Help Help
Help Happy
【样例输出】 - 4
0
4
【样例说明】测试3组字符串。第1组为"Happy"和"Help", 第2组为"Help"和"Help"，第3组为"Help"和"Happy"; ，比较结果分别为 - 4、0和4。
【评分标准】本题共2个测试点，每个测试点1.0分，共2.0分。*/


//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//int stringcmp (char* str1, char* str2)
//{
//	int i = 0;
//	while (1)
//	{
//		//if (str1[i] != str2[i])
//		//{
//		//	return str1[i] - str2[i];
//		//}
//		//else
//		//{
//		//	if (str1[i] == '\0' && str2[i] == '\0')
//		//	{
//		//		return 0;
//		//	}
//		//	else
//		//	{
//		//		i++;
//		//	}
//		if (str1[i] == str2[i])
//		{
//			if (str1[i] == '\0' && str2[i] == '\0')
//			{
//				return 0;
//			}
//			else
//			{
//				i++;
//			}
//		}
//		else
//		{
//			return str1[i] - str2[i];
//		}
//	}
//}
//int main()
//{
//	int n, i;
//	scanf("%d", &n);
//	char a[100] = { '\0' }, b[100] = { '\0' };
//	int* c = (int*)malloc(n * sizeof(int));
//	for (i = 0; i < n; i++)
//	{
//		scanf("%s %s", a, b);
//		c[i] = stringcmp(a, b);
//	}
//	for (i = 0; i < n; i++)
//	{
//		printf("%d\n", c[i]);
//	}
//	return 0;
//}