#include <iostream>
using namespace std;

typedef int ElemType;

typedef struct LNode{
    ElemType data;
    struct LNode *next;
}LNode, *LinkList;

void InitList(LinkList &L)
{
    L = nullptr;
    return;
}

int Length(LinkList L)
{
    LNode *p = L;
    int length = 0;
    while(p != nullptr)
    {
        length++;
        p = p->next;
    }

    return length;
}

LNode* LocateElem(LinkList L, ElemType e)
{
    LNode *p = L;
    while(p != nullptr && p->data != e)
    {
        p = p->next;
    }

    return p;
}

ElemType GetElem(LinkList L, int i)
{
    LNode *p = L;
    int pos = 1;
    while(p != nullptr && pos < i)
    {
        pos++;
        p = p->next;
    }

    if(pos > i || p == nullptr) return -1;

    return p->data;
}

bool ListInsert(LinkList &L, int i, ElemType e)
{
    if(i < 1) return false;

    if(i == 1)
    {
        LNode *temp = new LNode();
        temp->data = e;
        temp->next = L;
        L = temp;

        return true;
    }

    LNode *p = L;
    int pos = 1;
    while(p != nullptr && pos < i - 1)
    {
        pos++;
        p = p->next;
    }
    
    if(p == nullptr || pos > i) return false;

    LNode * temp = new LNode();
    temp->data = e;
    temp->next = p->next;
    p->next = temp;

    return true;
}


bool ListDelete(LinkList &L, int i, ElemType &e)
{
    if(i < 1 || L == nullptr) return false;

    if(i == 1)
    {
        LNode *temp = L;
        e = temp->data;
        L = L->next;
        delete temp;
        return true;
    }

    int pos = 1;
    LNode *p = L;
    while(p!= nullptr && pos < i - 1)
    {
        pos++;
        p = p->next;
    }
    if(p == nullptr || p -> next == nullptr || pos > i) return false;

    LNode *temp = p->next;
    e = temp->data;
    p->next = temp->next;
    delete temp;

    return true;
}

void PrintList(LinkList L)
{
    LNode *p = L;
    while(p != nullptr)
    {
        cout << p->data <<" ";
        p = p->next;
    }

    return;
}

bool Empty(LinkList L)
{
    return L == nullptr;
}

void ClearList(LinkList &L)
{
    while(L != nullptr)
    {
        LNode *p = L;
        L = L->next;
        delete p;
    }

    return;
}

LinkList LinkList_HeadInsert(LinkList &L)
{
    ElemType x;
    cin >> x;
    while(x != 9999)
    {
        LNode *temp = new LNode();
        temp->data = x;
        temp->next = L;
        L = temp;
        cin >> x;
    }

    return L;
}

LinkList LinkList_TailInsert(LinkList &L)
{
    ElemType x;
    cin >> x;
    LNode *tail = nullptr;
    while(x != 9999)
    {
        LNode * temp = new LNode();
        temp->data = x;
        temp->next = nullptr;
        if(L == nullptr)
        {
            L = temp;
            tail = L;
        }
        else
        {
            tail->next = temp;
            tail = temp;
        }

        cin >> x;
    }

    return L;
}

LinkList ReverseList_HeadInsert(LinkList &L)
{
    LNode *p = L;
    LNode *r = nullptr;
    LNode *dummyHead = new LNode();
    dummyHead->next = nullptr;

    while(p != nullptr)
    {
        r = p->next;
        p->next = dummyHead->next;
        dummyHead->next = p;
        p = r;
    }

    L = dummyHead->next;
    dummyHead->next = nullptr;
    delete dummyHead;

    return L;
}

LinkList ReverseList_ThreePointers(LinkList &L)
{
    if(L == nullptr || L->next == nullptr) return L;

    LNode *pre, *p = L, *r = p->next;
    L->next = nullptr;
    while(r != nullptr)
    {
        pre = p;
        p = r;
        r = r->next;
        p->next = pre;
    }

    L = p;

    return L;
}