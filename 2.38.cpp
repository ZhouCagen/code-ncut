//#include <iostream>  
//using namespace std;
//
//class ListNode 
//{
//public:
//    int data, freq;
//    ListNode* prior, * next;
//    ListNode(int d, int f = 0) : data(d), freq(f), prior(nullptr), next(nullptr) {}
//};
//
//class ListOperator 
//{
//public:
//    ListNode* head;
//
//    ListOperator()
//    {
//        head = new ListNode(0);  
//        head->next = head;
//        head->prior = head;
//    }
//
//    void AppendNode(int data)
//    {
//        ListNode* newNode = new ListNode(data);
//        InsertNode(newNode);
//    }
//
//    void InsertNode(ListNode* newNode) 
//    {
//        ListNode* current = FindInsertPosition(newNode);
//        InsertBeforeNode(newNode, current);
//    }
//
//    void InsertBeforeNode(ListNode* newNode, ListNode* beforeNode) 
//    {
//        ListNode* afterNode = beforeNode;
//        newNode->next = afterNode;
//        newNode->prior = afterNode->prior;
//        afterNode->prior->next = newNode;
//        afterNode->prior = newNode;
//
//        // 如果新节点被插入为头节点  
//        if (newNode->next == head) 
//        {
//            head->prior = newNode;
//        }
//    }
//
//    ListNode* FindInsertPosition(ListNode* newNode) 
//    {
//        ListNode* current = head->prior;
//        while (current != head && (current->freq < newNode->freq || (current->freq == newNode->freq && current->data < newNode->data)))
//        {
//            current = current->prior;
//        }
//        return current->next; 
//    }
//
//    void RemoveNode(ListNode* node)
//    {
//        node->prior->next = node->next;
//        node->next->prior = node->prior;
//        if (node->next == head)
//        {
//            head->prior = node->prior;
//        }
//    }
//
//    void LocateAndUpdate(int val)
//    {
//        ListNode* cur = head->next;
//        bool found = false;
//        while (cur != head)
//        {
//            if (cur->data == val) 
//            {
//                cur->freq++;
//                RemoveNode(cur);
//                InsertNode(cur);
//                found = true;
//                break;
//            }
//            cur = cur->next;
//        }
//        if (!found) 
//        {
//            cout << "Node not found." << endl;
//        }
//        else
//        {
//            cout << "Node found successfully." << endl;
//        }
//    }
//
//    void PrintList() 
//    {
//        ListNode* cur = head->next;
//        cout << "Data: ";
//        while (cur != head)
//        {
//            cout << cur->data << " ";
//            cur = cur->next;
//        }
//        cout << endl;
//
//        cur = head->next;
//        cout << "Freq: ";
//        while (cur != head)
//        {
//            cout << cur->freq << " ";
//            cur = cur->next;
//        }
//        cout << endl;
//    }
//
//    ~ListOperator() 
//    {
//        while (head->next != head)
//        {
//            ListNode* temp = head->next;
//            RemoveNode(temp);
//            delete temp;
//        }
//        delete head;
//    }
//};
//
//int main() {
//    ListOperator listOperator;
//
//    int length;
//    cin >> length;
//
//    for (int i = 0; i < length; i++) 
//    {
//        int data;
//        cin >> data;
//        listOperator.AppendNode(data);
//    }
//
//    int value;
//    do 
//    {
//        cin >> value;
//        listOperator.LocateAndUpdate(value);
//        listOperator.PrintList();
//    } while (value != 0);
//
//    return 0;
//}