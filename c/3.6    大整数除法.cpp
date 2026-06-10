//#include <stdio.h>
//#include<stdlib.h>
//#define MAX_LEN 101
//#include <string.h> 
//char szLine1[MAX_LEN];
//char szLine2[MAX_LEN];
//char xaNouse[MAX_LEN];
//int an1[MAX_LEN];
//int an2[MAX_LEN];
//int tmpAnt2[MAX_LEN];
//int anResult[MAX_LEN];
//
//int Max(int nMaxLen, int* an1, int* an2)
//{
//	int i;
//	for (i = nMaxLen - 1; i >= 0; i--)
//	{
//		if (an1[i] > an2[i])
//			return 1;
//		else if (an1[i] < an2[i])
//			return -1;
//	}
//	return 0;
//}
//void Substract(int nMaxLen, int* an1, int* an2)
//{
//	int nStartPos = 0;
//	int i;
//	for (i = 0; i < nMaxLen; i++)
//	{
//		an1[i] = an1[i] - an2[i];
//		if (an1[i] < 0)
//		{
//			an1[i] = an1[i] + 10;
//			an1[i + 1]--;
//		}
//	}
//}
//void ShiftLeft(int nMaxLen, int* an2, int* tmpAn2, int n)
//{
//	int i;
//	memcpy(tmpAn2, an2, nMaxLen * sizeof(int));
//	if (n <= 0) return;
//	for (i = nMaxLen - 1; i >= 0; i--)
//	{
//		if (i - n >= 0)
//			tmpAn2[i] = an2[i - n];
//		else tmpAn2[i] = 0;
//	}
//}
//
//int main()
//{
//	int n;
//	int i, j;
//	int nLen1, nLen2, nHighestPos, Shiftlen;
//	scanf("%d", &n);
//	getchar();
//	while (n > 0)
//	{
//		gets_s(szLine1);
//		gets_s(szLine2);
//		memset(an1, 0, sizeof(an1));
//		memset(an2, 0, sizeof(an2));
//
//		nLen1 = strlen(szLine1);
//		nLen2 = strlen(szLine2);
//		for (j = 0, i = nLen1 - 1; i >= 0; i--, j++)
//			an1[j] = szLine1[i] - '0';
//		for (j = 0, i = nLen2 - 1; i >= 0; i--, j++)
//			an2[j] = szLine2[i] - '0';
//
//		记录最高位
//		nHighestPos = 0;
//		memset(anResult, 0, sizeof(anResult));
//		Shiftlen = nLen1 - nLen2;
//		if (Shiftlen < 0)
//			printf("0\n");
//		else
//		{
//			while (Max(nLen1, an1, an2) >= 0)
//			{
//
//				ShiftLeft(nLen1, an2, tmpAnt2, Shiftlen);
//
//				while (Max(nLen1, an1, tmpAnt2) >= 0)
//				{
//					Substract(nLen1, an1, tmpAnt2);
//					anResult[Shiftlen]++;
//				}
//				if (nHighestPos == 0 && anResult[Shiftlen])
//					nHighestPos = Shiftlen;
//				Shiftlen--;
//			}
//			for (i = nHighestPos; i >= 0; i--)
//				printf("%d", anResult[i]);
//			printf("\n");
//		}
//		if (n > 1)
//			gets_s(xaNouse);
//		n--;
//	}
//
//	return 0;
//}
//
