#include <iostream>
using namespace std;

typedef int ElemType;

typedef struct DLNode{
    ElemType data;
    struct DLNode *prior, *next;
}DLNode, *DLinkList;

void InitList(DLinkList &L)
{
    L = nullptr;
}

int Length(DLinkList L)
{
    DLNode *p = L;
    int length = 0;
    while(p != nullptr)
    {
        length++;
        p = p->next;
    }

    return length;
}

DLNode* LocateElem(DLinkList L, ElemType e)
{
    DLNode *p = L;
    while(p != nullptr && p->data != e)
    {
        p = p->next;
    }

    return p;
}

ElemType GetElem(DLinkList L, int i)
{
    DLNode *p = L;
    int pos = 1;
    while(p != nullptr && pos < i)
    {
        pos++;
        p = p->next;
    }

    if(pos > i || p == nullptr) return -1;

    return p->data;
}

bool ListInsert(DLinkList &L, int i, ElemType e)
{
    if(i < 1) return false;
    if(i == 1)
    {
        DLNode *temp = new DLNode();
        temp->data = e;
        temp->next = L;
        temp->prior = nullptr;
        if(L != nullptr)
        {
            L->prior = temp;
        }
        L = temp;

        return true;
    }

    DLNode *p = L;
    int pos = 1;
    while(p != nullptr && pos < i - 1)
    {
        p = p->next;
        pos++;
    }

    if(p == nullptr || pos > i - 1) return false;

    DLNode *temp = new DLNode();
    temp->data = e;
    temp->next = p->next;
    temp->prior = p;
    if(p->next != nullptr)
    {
        p->next->prior = temp;
    }

    p->next = temp;

    return true;
}


bool ListDelete(DLinkList &L, int i, ElemType &e)
{
    if(i < 1 || L == nullptr) return false;

    if(i == 1)
    {
        DLNode *temp = L;
        e = temp->data;
        L = L->next;
        if(L != nullptr) L->prior = nullptr;
        delete temp;
        return true;
    }

    DLNode *p = L;
    int pos = 1;
    while(p != nullptr && pos < i - 1)
    {
        pos++;
        p = p->next;
    }

    if(pos > i - 1 || p == nullptr || p->next == nullptr)  return false;

    DLNode *temp = p->next;
    e = temp->data;
    
    p->next = temp->next;
    if(temp->next != nullptr)
    {
        temp->next->prior = p;
    }
    

    delete temp;

    return true;
}

void PrintList(DLinkList L)
{
    DLNode *p = L;
    while(p != nullptr)
    {
        cout << p->data <<" ";
        p = p->next;
    }

    return;
}

bool Empty(DLinkList L)
{
    return L == nullptr;
}

void ClearList(DLinkList &L)
{
    while(L != nullptr)
    {
        DLNode *p = L;
        L = L->next;
        delete p;
    }

    return;
}


DLinkList LinkList_HeadInsert(DLinkList &L)
{
    ElemType x;
    cin >> x;
    while(x != 9999)
    {
        DLNode *temp = new DLNode();
        temp->data = x;
        temp->next = L;
        temp->prior = nullptr;
        if(L != nullptr)
        {
            L->prior = temp;
        }
        L = temp;

        cin >> x;
    }

    return L;
}

DLinkList LinkList_TailInsert(DLinkList &L)
{
    ElemType x;
    cin >> x;
    DLNode *tail = L;
    while(x != 9999)
    {
        DLNode *temp = new DLNode();
        temp->data = x;
        temp->next = nullptr;
        if(L == nullptr)
        {
            L = temp;
            L->prior = nullptr;
        }
        else
        {
            tail->next = temp;
            temp->prior = tail;
        }

        tail = temp;
        cin >> x;
    }

    return L;
}

DLinkList ReverseList_HeadInsert(DLinkList &L)
{
    DLNode *p = L;
    DLNode *r = nullptr;
    DLNode *dummyHead = new DLNode();
    dummyHead->next = nullptr;

    while(p != nullptr)
    {
        r = p->next;
        p->next = dummyHead->next;
        p->prior = dummyHead;
        if(dummyHead->next != nullptr)
        {
            dummyHead->next->prior = p;

        }

        dummyHead->next = p;

        p = r;
    }

    L = dummyHead->next;
    if(L != nullptr)
    {
        L->prior = nullptr;
    }

    dummyHead->next = nullptr;
    delete dummyHead;

    return L;
}
