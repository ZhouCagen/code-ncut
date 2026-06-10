//#include <iostream>
//#include <algorithm>
//#include <string>
//#include <vector>
//using namespace std;
//
///* 0-1 背包：动态规划 */
//
//int knapsackDP2(vector<int>& weights, vector<int>& values, int cap)
//{
//	int n = weights.size();
//	vector<vector<int>> dp(n + 1, vector<int>(cap + 1, 0));
//	for (int i = 1; i <= n; i++)
//	{
//		for (int c = 0; c <= cap; c++)
//		{
//			if (weights[i - 1] > c)
//			{
//				dp[i][c] = dp[i - 1][c];
//			}
//			else
//			{
//				dp[i][c] = max(dp[i - 1][c], dp[i - 1][c - weights[i - 1]] + values[i - 1]);
//			}
//		}
//
//	}
//
//	return dp[n][cap];
//}
//
///* 0-1 背包：空间优化后的动态规划 */
//int knapsackDPComp2(vector<int>& weights, vector<int>& values, int cap)
//{
//	int n = weights.size();
//	vector<int>dp(cap + 1, 0);
//
//	for (int i = 1; i <= n; i++)
//	{
//		for (int c = cap; c >= weights[i - 1]; c--)
//		{
//
//			dp[c] = max(dp[c], dp[c - weights[i - 1]] + values[i - 1]);
//		}
//	}
//	return dp[cap];
//}
//
//
////
////int main() 
////{
////	int n; 
////	cin >> n;
////
////	vector<int> weights(n), values(n);
////
////	for (int i = 0; i < n; ++i)
////	{
////		cin >> weights[i] >> values[i]; 
////	}
////
////	int cap;
////	cin >> cap; 
////
////	cout << "DP二维数组结果: " << knapsackDP2(weights, values, cap) << endl;
////	cout << "DP空间优化结果: " << knapsackDPComp2(weights, values, cap) << endl;
////
////	return 0;
////}