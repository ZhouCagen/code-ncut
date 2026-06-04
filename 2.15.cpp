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
//int main()
//{
//	List* head_a = new List;
//	List* cur1 = head_a;
//
//	List* head_b = new List;
//	List* cur2 = head_b;
//
//	int size_a, size_b;
//	cin >> size_a;
//	for (int i = 0; i < size_a; i++)
//	{
//		cur1->next = new List;
//		cur1 = cur1->next;
//
//		cin >> cur1->val;
//	}
//
//	cin >> size_b;
//	for (int i = 0; i < size_b; i++)
//	{
//		cur2->next = new List;
//		cur2 = cur2->next;
//
//		cin >> cur2->val;
//	}
//
//	system("cls");
//
//	List* head_c = new List;
//	List* cur3 = head_c;
//
//	cur1 = head_a->next;
//	for (int i = 0; i < size_a; i++)
//	{
//		cur3->next = new List;
//		cur3 = cur3->next;
//
//		cur3->val = cur1->val;
//
//		cur1 = cur1->next;
//	}
//
//	cur2 = head_b->next;
//	for (int i = 0; i < size_b; i++)
//	{
//		cur3->next = new List;
//		cur3 = cur3->next;
//
//		cur3->val = cur2->val;
//
//		cur2 = cur2->next;
//	}
//
//	cur3 = head_c->next;
//	printList(cur3);
//
//	deleteList(cur1);
//	deleteList(cur2);
//	deleteList(cur3);
//
//	return 0;
//}