//#include <iostream>
//using namespace std;
//
//class List
//{
//public:
//	int val;
//	List* next;
//	List(int data) :val(data), next(nullptr) {}
//};
//
//class Dynamic_CyclicQueue
//{
//private:
//	List* head;
//	List* rear;
//public:
//	Dynamic_CyclicQueue()
//	{
//		head = new List(0);
//		head->next = head;
//		rear = head;
//	}
//	~Dynamic_CyclicQueue() {}
//	bool isEmpty();
//	void enQuene(int data);
//	int deQuene();
//};
//
//bool Dynamic_CyclicQueue::isEmpty()
//{
//	if (rear == head)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//void Dynamic_CyclicQueue::enQuene(int data)
//{
//	List* newNode = new List(data);
//	rear->next = newNode;
//	newNode->next = head;
//	rear = newNode;
//}
//
//int Dynamic_CyclicQueue::deQuene()
//{
//	int deQuene_value = 0;
//	if (isEmpty())
//	{
//		cout << "The queue is empty" << endl;
//	}
//	else
//	{
//		List* temp = head->next;
//		deQuene_value = temp->val;
//		head->next = temp->next;
//		if (rear == temp)
//		{
//			rear = head;
//		}
//		delete temp;
//	}
//	return deQuene_value;
//}
//
//
//
//int main()
//{
//	Dynamic_CyclicQueue queue;
//	queue.enQuene(10);
//	queue.enQuene(20);
//	queue.enQuene(30);
//
//	int data = 0;
//	while (!queue.isEmpty())
//	{
//		cout << "The value that is dequeued from the queue£º" << queue.deQuene() << endl;
//	}
//
//	return 0;
//}
