//#include<iostream>
//#include<cstring>
//using namespace std;
//int main()
//{
//	char str1[200];
//	char str2[200];
//	cin >> str1 >> str2;
//	int len1 = (int)strlen(str1);
//	int len2 = (int)strlen(str2);
//	int a[200] = { 0 }, b[200] = { 0 }, c[300] = { 0 };
//	for (int i = 0; i < len1; i++)
//	{
//		a[i] = str1[len1 - 1 - i] - '0';
//	}
//	for (int i = 0; i < len2; i++)
//	{
//		b[i] = str2[len2 - 1 - i] - '0';
//	}
//	int len = std::max(len1, len2);
//	int temp = 0;
//	for (int i = 0; i < len; i++)
//	{
//		c[i] = a[i] + b[i] + temp;
//		temp = c[i] / 10;
//		c[i] %= 10;
//	}
//	if (temp != 0)
//	{
//		c[len] = temp;
//		for (int i = len; i >= 0; i--)
//		{
//			cout << c[i];
//		}
//	}
//	else
//	{
//		for (int i = len - 1; i >= 0; i--)
//		{
//			cout << c[i];
//		}
//	}
//	return 0;
//}