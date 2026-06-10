//#include<iostream>
//using namespace std;
//int Fibonacci(int n)
//{
//	int a[2001] = { 0 };
//	if (n < 1)
//	{
//		return -1;
//	}
//	a[1] = 1;
//	a[2] = 1;
//	for (int i = 3; i <= n; i++)
//	{
//		a[i] = (a[i - 1] + a[i - 2]) % 1000;
//	}
//	return a[n];
//}
//int main()
//{
//	int n = 0;
//	cin >> n;
//	for (int i = 1; i <= n; i++)
//	{
//		int a;
//		cin >> a;
//		cout << Fibonacci(a) << endl;
//	}
//	return 0;
//}