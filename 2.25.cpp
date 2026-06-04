//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//vector<int> Intersection(vector<int> A, vector<int>B)
//{
//	vector<int>C;
//	int i = 0, j = 0;
//
//	while (i < A.size() && j < B.size())
//	{
//		if (A[i] == B[j])
//		{
//			C.emplace_back(A[i]);
//			i++;
//			j++;
//		}
//		else if (A[i] < B[j])
//		{
//			i++;
//		}
//		else
//		{
//			j++;
//		}
//	}
//
//	return C;
//}
//
//int main()
//{
//	vector<int>A, B;
//	int length_A, length_B;
//	int num;
//
//	cin >> length_A;
//	for (int i = 0; i < length_A; i++)
//	{
//		cin >> num;
//		A.emplace_back(num);
//	}
//
//	cin >> length_B;
//	for (int i = 0; i < length_B; i++)
//	{
//		cin >> num;
//		B.emplace_back(num);
//	}
//
//	vector<int>C = Intersection(A, B);
//	for (int num : C)
//	{
//		cout << num << " ";
//	}
//
//	return 0;
//}