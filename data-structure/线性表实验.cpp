////#include <iostream>
////using namespace std;
////
////class List
////{
////public:
////	int id;
////	int val;
////	List* next = nullptr;
////};
////
////void JosephProblem(int upperLimit, int personNumber, List* head)
////{
////	List* current = head;
////	List* beforeNode = new List;
////
////	for (int i = 1; i <= personNumber; i++)
////	{
////		for (int count = 1; count < upperLimit; count++)
////		{
////			beforeNode = current;
////			current = current->next;
////		}
////
////		cout << current->id << " ";
////
////		upperLimit = current->val;
////
////		beforeNode->next = current->next;
////		List* temp = current;
////
////		current = current->next;
////		delete temp;
////
////	}
////}
////
////int main()
////{
////	List* head = new List;
////	List* current = head;
////
////	int upperLimit, personNumber;
////	cin >> upperLimit >> personNumber;
////
////	for (int i = 1; i <= personNumber; i++)
////	{
////		current->id = i;
////		cin >> current->val;
////		current->next = new List;
////		if (i < personNumber)
////		{
////			current = current->next;
////		}
////	}
////	current->next = head;
////
////
////	JosephProblem(upperLimit, personNumber, head);
////
////	return 0;
////
////}
//
//#include <iostream>
//#include <vector>
//#include <queue>
//using namespace std;
//
//void JosephProblem(vector<int>password, int upperLimit)
//{
//	queue<pair<int, int>>Queue;
//	for (int i = 1; i <= password.size(); i++)
//	{
//		Queue.push({ i, password[i - 1] });
//	}
//
//	vector<int> Listing_order;
//
//	int temp_upperLimit = upperLimit;
//
//	while (!Queue.empty())
//	{
//		int count = 1;
//
//		while (count < temp_upperLimit)
//		{
//			Queue.push(Queue.front());
//			Queue.pop();
//			count++;
//		}
//
//		pair<int, int> temp = Queue.front();
//		Queue.pop();
//		Listing_order.push_back(temp.first);
//
//		temp_upperLimit = temp.second;
//	}
//
//	for (int order : Listing_order)
//	{
//		cout << order << " ";
//	}
//
//	cout << endl;
//}
//int main()
//{
//	int upperLimit, personNumber;
//	cin >> upperLimit >> personNumber;
//
//	vector<int>password(personNumber);
//	for (int i = 0; i < personNumber; i++)
//	{
//		cin >> password[i];
//	}
//
//	JosephProblem(password, upperLimit);
//}