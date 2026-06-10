//#include<iostream>
//#include<cstring>
//using namespace std;
//int main()
//{
//	char str1[1000], str2[1000];
//	cin >> str1 >> str2;
//	int a[1000] = { 0 }, b[1000] = { 0 }, c[1000] = { 0 };
//	int len1 = (int)strlen(str1);
//	int len2 = (int)strlen(str2);
//	int f = 0; 
//	if (len1 < len2 || (strcmp(str1, str2) < 0 && len1 == len2))
//	{
//		cout<<"-";
//		char temp[1000];
//		strcpy_s(temp, str1);
//		strcpy_s(str1, str2);
//		strcpy_s(str2, temp);
//		int len = len1;
//		len1 = len2;
//		len2 = len;
//	}
//	for (int i = 0; i < len1; i++)
//	{
//		a[i] = str1[len1 - 1 - i] - '0';
//	}
//	for (int i = 0; i < len2; i++)
//	{
//		b[i] = str2[len2 - 1 - i] - '0';
//	}
//	for (int i = 0; i < len1; i++)
//	{
//		if (a[i] < b[i])
//		{
//			a[i + 1]--;
//			a[i] += 10;
//			c[i] = a[i] - b[i];
//		}
//		else
//		{
//			c[i] = a[i] - b[i];
//		}
//	}
//	for (int i = len1 - 1; i >= 0; i--)
//	{
//		if (c[i] == 0 && f == 0)
//		{
//			continue;
//		}
//		else
//		{
//			f = 1;
//			cout << c[i];
//		}
//	}
//	if (f == 0)
//	{
//		cout << "0";
//	}
//	return 0;
//}