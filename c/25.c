//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<string.h>


//void str_bin(char* s1, char* s2, char* s3)
//{
//	int  m = 0, n = 0, i = 0;
//	while (m < 100 && n < 100 && s1[m] != '\0' && s2[n] != '\0')
//		s3[i++] = s1[m] < s2[n] ? s1[m++] : s2[n++];
//	while (m < 100 && s1[m] != '\0')
//		s3[i++] = s1[m++];
//
//	while (n < 100 && s2[n] != '\0')
//		s3[i++] = s2[n++];
//
//	s3[i] = '\0';
//}
//int main()
//{
//	char a[100], b[100], c[200];
//	scanf("%s", a);
//	scanf("%s", b);
//	str_bin(a, b, c);
//	printf("%s", c);
//	return 0;
//}
 


//void str_bin(char s1[], char s2[])
//{
//	int i = 0, count = 0, j;
//	strcat(s1, s2);
//	while (s1[i++] != '\0')
//	{
//		count++;
//	}
//	for (i = 0; i < count - 1; i++)
//	{
//		for (j = 0; j < count - i - 1; j++)
//		{
//			if (s1[j] > s1[j + 1])
//			{
//				int temp = s1[j];
//				s1[j] = s1[j + 1];
//				s1[j + 1] = temp;
//			}
//		}
//	}
//}
//int main()
//{
//	char a[200] = { '\0' }, b[100] = { '\0' };
//	scanf("%s", a);
//	scanf("%s", b);
//	str_bin(a, b);
//	printf("%s", a);
//	return 0;
//}