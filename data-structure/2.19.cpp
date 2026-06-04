//#include <iostream>
//using namespace std;
//
//class List
//{
//public:
//	int val;
//	List* next = nullptr;
//};
//
//void deleteList(List* head)
//{
//	List* cur = head;
//	List* next = nullptr;
//
//	while (cur != nullptr)
//	{
//		next = cur->next;
//		delete cur;
//		cur = next;
//	}
//
//}
//
//void printList(List* cur)
//{
//	while (cur != nullptr)
//	{
//		cout << cur->val << " ";
//		cur = cur->next;
//	}
//
//	cout << endl;
//}
//
//void removeElements(List* head, int mink, int maxk)
//{
//	List* Node = new List;
//	List* cur = head;
//	while (cur != nullptr)
//	{
//		if (cur->val > mink && cur->val < maxk)
//		{
//			if (Node != nullptr)
//			{
//				Node->next = cur->next;
//			}
//			else 
//			{
//				head = cur->next;
//			}
//			delete cur;
//			cur = Node ? Node->next : head;
//		}
//		else
//		{
//			Node = cur;
//			cur = cur->next;
//		}
//	}
//
//}
//
//int main()
//{
//	List* head = new List;
//	List* cur = head;
//
//	int n;
//	cin >> n;
//
//	for (int i = 0; i < n; i++)
//	{
//		cur->next = new List;
//		cur = cur->next;
//
//		cin >> cur->val;
//	}
//
//	int mink, maxk;
//	cin >> mink >> maxk;
//	
//	cur = head;
//	removeElements(cur, mink, maxk);
//
//	cur = head->next;
//	printList(cur);
//
//	deleteList(head);
//
//	return 0;
//}