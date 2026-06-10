//#include<iostream>
//using namespace std;
//int main()
//{
//	int N;
//	cin >> N;
//	int a[26] = { 0 };
//	char str1[100] = { '\0' }, str2[100] = { '\0' }, str3[100] = { '\0' };
//	for (int i = 0; i < N; i++)
//	{
//		cin >> str1[i] >> str3[i] >> str2[i];
//	}
//	a[0] = 1;
//	for (int i = 0; i < N; i++)
//	{
//		a[str1[i] - 'a'] = a[str2[i] - 'a'];
//	}
//	int c = 0, m = 0;
//	char str[26] = { '\0' };
//	for (int i = 1; i < 26; i++)
//	{
//		if (a[i] == 1)
//		{
//			str[m++] = i + 'a';
//			c = 1;
//		}
//	}
//	if (c == 0)
//	{
//		if (a[0] == 0)
//		{
//			cout << "none";
//		}
//		else
//		{
//			cout << "a only";
//		}
//	}
//	if (c == 1)
//	{
//		if (a[0] == 1)
//		{
//			cout << 'a' << " ";
//		}
//		for (int i = 0; i < m; i++)
//		{
//			cout << str[i] << " ";
//		}
//	}
//	return 0;
//}