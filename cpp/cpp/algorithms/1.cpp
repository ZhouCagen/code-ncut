//#include <iostream>
//#include <algorithm>
//#include <string>
//#include <vector>
//using namespace std;
//
//string constructString(int i, int j, const vector<vector<int>> split)
//{
//	if (i == j)
//	{
//		return "A" + to_string(i);
//	}
//	else
//	{
//		int k = split[i][j];
//		return "(" + constructString(i, k, split) + "*" + constructString(k + 1, j, split) + ")";
//	}
//}
//
//pair<int, string> matrixOrder(const vector<int> arrays)
//{
//	int n = static_cast<int>(arrays.size() - 1);
//
//	vector<vector<int>> dp(n + 1, vector<int>(n + 1, INT_MAX));
//	vector<vector<int>> split(n + 1, vector<int>(n + 1, 0));
//
//	for (int i = 1; i <= n; i++)
//	{
//		dp[i][i] = 0;
//	}
//
//	for (int len = 2; len <= n; len++)
//	{
//		for (int i = 1; i + len - 1 <= n; i++)
//		{
//			int end = i + len - 1;
//			for (int j = i; j < end; j++)
//			{
//				int cost = dp[i][j] + dp[j + 1][end] + arrays[i - 1] * arrays[j] * arrays[end];
//				if (cost < dp[i][end])
//				{
//					dp[i][end] = cost;
//					split[i][end] = j;
//				}
//			}
//		}
//	}
//
//	return { dp[1][n],constructString(1,n,split) };
//}
//
//int main()
//{
//	int n;
//	cin >> n;
//
//	vector<int> arrays(n + 1, 0);
//	for (int i = 0; i <= n; i++) 
//	{
//		cin >> arrays[i];
//	}
//
//	auto answer = matrixOrder(arrays);
//
//	cout << "最少计算量：" << answer.first << endl;
//	cout << "计算量最小的矩阵连乘加括号方式" << answer.second << endl;
//
//}
//
///*
//	6
//	30 35 15 5 10 20 25
//	最少计算量：15125
//	计算量最小的矩阵连乘加括号方式((A1*(A2*A3))*((A4*A5)*A6))
//*/
//
//
//
//
