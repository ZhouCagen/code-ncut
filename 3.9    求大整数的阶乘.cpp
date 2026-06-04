//#include<iostream>
//using namespace std;
//int main()
//{
//	int a[2001] = { 0 };
//	int temp, digit, n, i, j = 0;
//	cin >> n;
//	a[0] = 1;
//	digit = 1;
//	for (i = 2; i <= n; i++)
//	{
//		int num = 0;
//		for (j = 0; j < digit; j++)
//		{
//			temp = a[j] * i + num;
//			a[j] = temp % 10;
//			num = temp / 10;
//		}
//		while (num)
//		{
//			a[digit] = num % 10;
//			num = num / 10;
//			digit++;
//		}
//	}
//	for (i = digit - 1; i >= 0; i--)
//	{
//		cout << a[i];
//	}
//	return 0;
//}