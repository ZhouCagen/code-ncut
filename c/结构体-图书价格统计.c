//【问题描述】用结构数组保存书的信息。现有N（N < 50）本图书，每本图书包括书名（长度小于30的字符串)、出版社（长度小于15的字符串）
//     数量（整数）、单价（浮点数）信息，读入书 的信息并按单价由少到多的顺序输出各书的所有信息，然后输出购买这些书的总费用。
// 【输入形式】第一行为一个整数N，表示图书数量，接下来的N行中每行包含一本书的信息数据，各数
//    据之间由空格分隔。
// 【输出形式】输出前N行，为按单价排序的所有书的信息，第N + 1行为购买这些书的总费用（保留到小数点后两位）。
//    【样例输入】3
//    C - Programming       qinghu                50    25.50
//    DataStruct              jixiegongye         30    31.55
//    OpratingSystem      gaodengjiaoyu     60    18.30
//
//    【样例输出】OpratingSystem      gaodengjiaoyu    60     18.30
//    C - Programming        qinghu               50     25.50
//    DataStruct               jixiegongye        30      31.55
//    3319.50
//    【样例说明】(1)输入的所有书的信息用英文以及数字(不能是汉字)表示。
//    (2)输出前N行的格式设定为："%-30s%-20s%-10d%-.2f\n"。



//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//struct book
//{
//	char name[30];
//	char cbs[15];
//	int num;
//	float price;
//};
//int main()
//{
//	struct book * c;
//	int n, i, j;
//	scanf("%d", &n);
//	while (n >= 50 && n <= 0)
//	{
//		scanf("%d", &n);
//	}
//	c = (struct book*)malloc(n * sizeof(struct book));
//	for (i = 0; i < n; i++)
//	{
//		scanf("%s %s %d %f", c[i].name, c[i].cbs, &c[i].num, &c[i].price);
//	}
//	struct book temp;
//	for (i = 0; i < n - 1; i++)
//	{
//		for (j = 0; j < n - i - 1; j++)
//		{
//			if (c[j].price > c[j + 1].price)
//			{
//				temp = c[j];
//				c[j] = c[j + 1];
//				c[j + 1] = temp;
//			}
//		}
//	}
//	float sum = 0;
//	for (i = 0; i < n; i++)
//	{
//		sum += c[i].num * c[i].price;
//	}
//	for (i = 0; i < n; i++)
//	{
//		printf("%-30s%-20s%-10d%-.2f\n", c[i].name, c[i].cbs, c[i].num, c[i].price); 
//	}
//	printf("%.2f", sum);
//	return 0;
//}