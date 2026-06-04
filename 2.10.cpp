//#include <iostream>
//using namespace std;
//
//enum Status { OK, INFEASIBLE };
//
//struct SqList
//{
//	int* elem;
//	int length;
//};
//Status DeleteSqList_K(SqList& a, int i, int k)
//{
//	if (i > 0 && i <= a.length && k >= 0 && k <= a.length - 1)
//	{
//		for (int j = i + k - 1; j < a.length; j++)
//		{
//			a.elem[j - k] = a.elem[j];
//		}
//		a.length -= k;
//		return OK;
//	}
//	else
//	{
//		return INFEASIBLE;
//	}
//}
//
//int main()
//{
//	SqList a;
//	int initialData[] = { 1,2,3,4,5,6,7,8,9,10 };
//	a.elem = new int[10];
//	a.length = 10;
//	for (int i = 0; i < a.length; i++)
//	{
//		a.elem[i] = initialData[i];
//	}
//
//	Status result = DeleteK(a, 3, 4);
//	if (result == OK)
//	{
//		cout << "Deleted successfully." << endl;
//		for (int i = 0; i < a.length; i++)
//		{
//			cout << a.elem[i] << " ";
//		}
//	}
//	else
//	{
//		cout << "Operation infeasible." << endl;
//	}
//
//	delete[] a.elem;
//
//	return 0;
//}