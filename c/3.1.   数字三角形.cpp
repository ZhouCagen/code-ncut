//#include<iostream>
//using namespace std;
//int main()
//{
//	ios::sync_with_stdio(false);
//	cin.tie(nullptr);
//	cout.tie(nullptr);
//	int a;
//	cin >> a;
//	if (a <= 0 || a >= 7)
//	{
//		cin >> a;
//	}
//	for (int b = 1; b <= 6; b++)
//	{
//		if (a == b) 
//		{
//			continue;
//		}
//		for (int c = 1; c <= 6; c++)
//		{
//			if (a == c || b == c)
//			{
//				continue;
//			}
//			for (int d = 1; d <= 6; d++)
//			{
//				if (a == d || b == d || c == d)
//				{
//					continue;
//				}
//				for (int e = 1; e <= 6; e++)
//				{
//					if (a == e || b == e || c == e || d == e)
//					{
//						continue;
//					}
//					for (int f = 1; f <= 6; f++)
//					{
//						if (a == f || b == f || c == f || d == f || e == f)
//						{
//							continue;
//						}
//						if (a + b + c == a + e + f && a + b + c == c + d + e)
//						{
//							cout << a << b << c << d << e << f << endl;
//						}
//					}
//				}
//			}
//		}
//	}
//	return 0;
//}
