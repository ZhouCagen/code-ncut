//#include<iostream>
//#include<cstring>
//#include<algorithm>
//using namespace std;
//#define Max_len 100
//int main()
//{
//	char str1[Max_len], str2[Max_len];
//	int a[2 * Max_len] = { 0 };
//	cin >> str1 >> str2;
//	int len1 = (int)strlen(str1);
//	int len2 = (int)strlen(str2);
//	for (int i = 0; i < len1; i++)
//	{
//		for (int j = 0; j < len2; j++)
//		{
//			a[i + j] += (str1[len1 - 1 - i] - '0') * (str2[len2 - 1 - j] - '0');
//		}
//	}
//	for (int i = 0; i < len1 + len2; i++)
//	{
//		a[i + 1] += a[i] / 10;
//		a[i] %= 10;
//	}
//	int max = len1 + len2;
//	
//	while (max > 0 && a[max - 1] == 0)
//	{
//		max--;
//	}
//	for (int i = max - 1; i >= 0; i--)
//	{
//		cout << a[i];
//	}
//	return 0;
//}
