//#include<iostream>
//#include<vector>
//#include<algorithm>
//using namespace std;
//int main()
//{
//	int n;
//	cin >> n;
//	vector<vector<int>>a(n, vector<int>(n));
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < n; j++)
//		{
//			a[i][j] = 0;
//		}
//	}
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < i + 1; j++)
//		{
//			cin >> a[i][j];
//		}
//	}
//	int max = 0;
//	for (int j = 0; j < n; j++)
//	{
//		int sum = a[n - 1][j];
//		int c = j;
//		for (int m = n - 2; m > 0; m--)
//		{
//			if (c == 0)
//			{
//				sum += a[m][c];
//			}
//			else
//			{
//				sum += std::max(a[m][c], a[m][c - 1]);
//				if (a[m][c] < a[m][c - 1])
//				{
//					c = c - 1;
//				}
//			}
//		}
//		sum += a[0][0];
//		max = std::max(sum, max);
//	}
//	cout << max << endl;
//	return 0;
//}