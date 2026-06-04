//#include <iostream>
//using namespace std;
//
//class List
//{
//public:
//	int val;
//	List* prior, * next;
//	List(int data) :val(data), prior(nullptr), next(nullptr) {}
//};
//
//class DoublyStack
//{
//private:
//	List* frontTop;
//	List* backTop;
//public:
//	DoublyStack() :frontTop(nullptr), backTop(nullptr) {}
//	~DoublyStack()
//	{
//		while (frontTop != nullptr)
//		{
//			pop(1);
//		}
//		while (backTop != nullptr)
//		{
//			pop(0);
//		}
//	}
//	void push(int data, int i);
//	int pop(int i);
//	void printStack();
//};
//
//void DoublyStack::push(int data, int i)
//{
//	List* newNode = new List(data);
//	if (i == 1)
//	{
//		if (frontTop== nullptr)
//		{
//			frontTop = newNode;
//		}
//		else
//		{
//			newNode->prior = frontTop;
//			frontTop->next = newNode;
//
//			frontTop = newNode;
//		}
//	}
//	else if (i == 0)
//	{
//		if (backTop == nullptr)
//		{
//			backTop = newNode;
//		}
//		else
//		{
//			newNode->next = backTop;
//			backTop->prior = newNode;
//
//			backTop = newNode;
//		}
//	}
//	else
//	{
//		cout << "The stack in this direction is not defined" << endl;
//	}
//}
//
//int DoublyStack::pop(int i)
//{
//	List* temp = nullptr;
//	int popValue = 0;
//	if (i == 1)
//	{
//		if (frontTop == nullptr)
//		{
//			cout << "The front stack is empty" << endl;
//
//		}
//		else
//		{
//			temp = frontTop;
//			popValue = frontTop->val;
//			frontTop = frontTop->prior;
//			if (frontTop != nullptr)
//			{
//				frontTop->next = nullptr;
//			}
//			
//			delete temp;
//		}
//	}
//	else if (i == 0)
//	{
//		if (backTop == nullptr)
//		{
//			cout << "The back stack is empty" << endl;
//		}
//		else
//		{
//			temp = backTop;
//			popValue = backTop->val;
//			backTop = backTop->next;
//			if (backTop != nullptr)
//			{
//				backTop->prior = nullptr;
//			}
//
//			delete temp;
//		}
//	}
//	else
//	{
//		cout << "The stack in this direction is not defined" << endl;
//		popValue = -1;
//	}
//	return popValue;
//}
//
//void DoublyStack::printStack()
//{
//	cout << "Front stack: ";
//	List* current = frontTop;
//	while (current != nullptr)
//	{
//		cout << current->val << " ";
//		current = current->prior;   
//	}
//	cout << endl;
//
//	cout << "Back stack: ";
//	current = backTop;
//	while (current != nullptr)
//	{
//		cout << current->val << " ";
//		current = current->next;
//	}
//	cout << endl;
//}
//
//int main() 
//{
//	DoublyStack doublystack;
//	doublystack.push(10, 1);
//	doublystack.push(20, 1);
//	doublystack.push(30, 0);
//	doublystack.push(40, 0);
//
//	doublystack.printStack();
//
//	cout << "The value that is popped from the front stack£º" << doublystack.pop(1) << endl;
//	cout << "The value that is popped from the back stack£º " << doublystack.pop(0) << endl;
//
//	doublystack.printStack();
//
//	return 0;
//}
