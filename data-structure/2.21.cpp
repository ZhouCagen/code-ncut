//#include <algorithm>
//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//enum Status { OK, INFESIBLE };
//
//class SqList
//{
//public:
//	vector<int>elem;
//	int length;
//
//	SqList() :length(0) {}
//
//	Status reverseElement();
//	Status printList();
//};
//
//Status SqList::reverseElement()
//{
//	reverse(elem.begin(), elem.end());
//	return OK;
//}
//
//Status SqList::printList()
//{
//	for (int num : elem)
//	{
//		cout << num << " ";
//	}
//
//	return OK;
//}
//
//int main()
//{
//	SqList a;
//
//	cin >> a.length;
//
//	int num;
//
//	for (int i = 0; i < a.length; i++)
//	{
//		cin >> num;
//		a.elem.emplace_back(num);
//	}
//
//	a.reverseElement();
//	a.printList();
//
//	return 0;
//}