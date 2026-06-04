//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//enum Status { OK, INFEASIBLE };
//
//class SqList
//{
//public:
//	vector<int>elem;
//	int length;
//
//	SqList() :length(0) {}
//
//	Status sort();
//	Status insertInOrderElement(int x);
//};
//
//Status SqList::insertInOrderElement(int x)
//{
//	
//	int i = 0;
//	while (i < length && elem[i] < x)
//	{
//		i++;
//	}
//	elem.insert(elem.begin() + i, x);
//
//	length++;
//
//	return OK;
//}
//
//
//Status SqList::sort()
//{
//	for (int i = 1; i <length; i++)
//	{
//		int no_insert = elem[i];
//		int j = i - 1;
//		while (j >= 0 && elem[j] > no_insert)
//		{
//			elem[j + 1] = elem[j];
//			j--;
//		}
//		elem[j + 1] = no_insert;
//	}
//
//	cout << "Insertsort SqList:" << endl;
//	for (int i = 0; i < length; i++)
//	{
//		cout << elem[i] << " ";
//	}
//
//	cout << endl;
//
//	return OK;
//}
//
//int main()
//{
//
//	SqList a;
//
//	cin >> a.length;
//
//	int number;
//
//	for (int i = 0; i < a.length; i++)
//	{
//		cin >> number;
//		a.elem.emplace_back(number);
//	}
//
//	system("cls");
//
//	a.sort();
//	cout << "-------------------------------" << endl;
//	cout << "input insert number:" << endl;
//
//	int x;
//	cin >> x;
//
//	system("cls");
//
//	a.insertInOrderElement(x);
//
//	cout << "New SqList:" << endl;
//
//	for (int i = 0; i < a.length; i++)
//	{
//		cout << a.elem[i] << " ";
//	}
//
//	cout << endl;
//
//	return 0;
//}