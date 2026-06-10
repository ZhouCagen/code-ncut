//#include <iostream>
//#include <algorithm>
//#include <string>
//#include <vector>
//using namespace std;
//
//void findLongestIncreasingSubsequence(const vector<int>& arr)
//{
//	int n = static_cast<int>(arr.size() - 1);
//
//	int maxLen = 1, maxStart = 0;
//	int currLen = 1, currStart = 0;
//
//	for (int i = 1; i < n; ++i)
//	{
//		if (arr[i] >= arr[i - 1])
//		{
//			currLen++;
//		}
//		else
//		{
//			if (currLen > maxLen)
//			{
//				maxLen = currLen;
//				maxStart = currStart;
//			}
//			currStart = i;
//			currLen = 1;
//		}
//	}
//
//	if (currLen > maxLen)
//	{
//		maxLen = currLen;
//		maxStart = currStart;
//	}
//
//	vector<int> result;
//
//	for (int i = maxStart; i < maxStart + maxLen; i++)
//	{
//		result.push_back(arr[i]);
//	}
//
//	cout << "最长非递减子数组是：";
//	for (int num : result)
//	{
//		cout << num << " ";
//	}
//}
//
////int main()
////{
////	int n;
////	cin >> n;
////	vector<int> arr(n, 0);
////	for (int i = 0; i < n; i++)
////	{
////		cin >> arr[i];
////	}
////
////	findLongestIncreasingSubsequence(arr);
////
////	return 0;
////}
//
///*
//9
//5 6 3 5 7 8 9 1 2
//*/