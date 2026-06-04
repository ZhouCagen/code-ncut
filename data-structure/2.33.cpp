//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//class List
//{
//public:
//	char val;
//	List* next = nullptr;
//};
//
//void deleteList(List* head)
//{
//	if (!head)
//	{
//		return;
//	}
//	List* cur = head;
//	//List* next = nullptr;
//	do
//	{
//		next = cur->next;
//		delete cur;
//		cur = next;
//	} while (cur != head);
//
//	return;
//}
//
//void printList(List* head)
//{
//	if (!head)
//	{
//		cout << "No data" << endl << endl;
//		return;
//	}
//
//	List* cur = head;
//	do
//	{
//		if (cur != nullptr)
//		{
//			cout << cur->val << " ";
//			cur = cur->next;
//		}
//	} while (cur != head);
//
//	cout << endl << endl;
//
//	return;
//}
//
//void addNode(List*& head, List*& tail, char val)
//{
//	List* newNode = new List;
//	newNode->val = val;
//	if (!head)
//	{
//		head = tail = newNode;
//		head->next = head; 
//	}
//	else 
//	{
//		tail->next = newNode;
//		tail = newNode;
//		tail->next = head;
//	}
//
//	return;
//}
//
//void classifyList(List* cur, List*& letter_head, List*& number_head, List*& others_head, List*& letter_tail, List*& number_tail, List*& others_tail)
//{
//	while (cur != nullptr)
//	{
//		if (cur->val >= '0' && cur->val <= '9')
//		{
//			addNode(number_head, number_tail, cur->val);
//		}
//		else if ((cur->val >= 'a' && cur->val <= 'z') || (cur->val >= 'A' && cur->val <= 'Z'))
//		{
//			addNode(letter_head, letter_tail, cur->val);
//		}
//		else
//		{
//			addNode(others_head, others_tail, cur->val);
//		}
//
//		cur = cur->next;
//
//	}
//
//	return;
//}
//
//int main() {
//	List* head = nullptr;
//	List* cur = nullptr;
//
//	int length;
//	cin >> length;
//
//	for (int i = 0; i < length; i++)
//	{
//		List* newNode = new List;
//		cin >> newNode->val;
//		if (!head) {
//			head = cur = newNode;
//		}
//		else {
//			cur->next = newNode;
//			cur = newNode;
//		}
//	}
//
//
//	List* letter_head = nullptr, * letter_tail = nullptr;
//	List* number_head = nullptr, * number_tail = nullptr;
//	List* others_head = nullptr, * others_tail = nullptr;
//
//	classifyList(head, letter_head, number_head, others_head, letter_tail, number_tail, others_tail);
//
//	system("cls");
//
//	cout << "Letters£º" << endl;
//	printList(letter_head);
//
//	cout << "Numbers£º" << endl;
//	printList(number_head);
//
//	cout <<  "Others£º" << endl;
//	printList(others_head);
//
//
//	return 0;
//}
