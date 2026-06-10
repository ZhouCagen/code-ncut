#include <iostream>
using namespace std;

typedef int ElemType;

typedef struct DLNode{
    ElemType data;
    struct DLNode *prior, *next;
}DLNode, *DLinkList;

void InitList(DLinkList &L)
{
    L = new DLNode();
    L->data = 0;
    L->next = L;
    L->prior = L;
    return;
}

int Length(DLinkList L)
{
    DLNode *p = L->next;
    int count = 0;
    while(p != L)
    {
        count++;
        p = p->next;
    }

    return count;
}

DLNode* LocateElem(DLinkList L, ElemType e)
{
    DLNode *p = L->next;
    while(p != L && p->data != e)
    {
        p = p->next;
    }

    if(p == L) return nullptr;

    return p;
}

ElemType GetElem(DLinkList L, int i)
{
    DLNode *p = L->next;
    int pos = 1;
    while(p != L && pos < i)
    {
        pos++;
        p = p->next;
    }

    if(p == L || pos > i) return -1;

    return p->data;
}

bool ListInsert(DLinkList &L, int i, ElemType e)
{
    DLNode *p = L;
    int pos = 0;
    while(p->next != L && pos < i - 1)
    {
        pos++;
        p = p->next;
    }

    if(pos != i - 1) return false;

    DLNode *temp = new DLNode();
    temp->data = e;
    temp->next = p->next;
    temp->prior = p;

    p->next->prior = temp;

    p->next = temp;

    return true;
}


bool ListDelete(DLinkList &L, int i, ElemType &e)
{
    DLNode *p = L;
    int pos = 0;
    while(p->next != L && pos < i - 1)
    {
        pos++;
        p = p->next;
    }

    if(pos != i - 1 || p->next == L) return false;

    DLNode *temp = p->next;
    e = temp->data;
    temp->next->prior = p;
    p->next = temp->next;

    delete temp;

    return true;
}

void PrintList(DLinkList L)
{
    DLNode *p = L->next;
    while(p != L)
    {
        cout << p->data <<" ";
        p = p->next;
    }

    return;
}

bool Empty(DLinkList L)
{
    return L->next == L;
}

void ClearList(DLinkList &L)
{
    DLNode *p = L->next;
    while(p != L)
    {
        DLNode *temp = p;
        p = p->next;
        delete temp;
    }

    L->next = L;
    L->prior = L;
    return;
}

void DestroyList(DLinkList &L)
{
    DLNode *p = L->next;
    while(p != L)
    {
        DLNode *temp = p;
        p = p->next;
        delete temp;
    }

    delete L;
    L = nullptr;

    return;
}


DLinkList LinkList_HeadInsert(DLinkList &L)
{
    ElemType e;
    cin >> e;
    while(e != 9999)
    {
        DLNode *temp = new DLNode();
        temp->data = e;
        temp->next = L->next;
        temp->prior = L;
        L->next->prior = temp;
        L->next = temp;

        cin >> e;
    }

    return L;
}

DLinkList LinkList_TailInsert(DLinkList &L)
{
    ElemType e;
    cin >> e;
    DLNode *tail = L;
    while(e != 9999)
    {
        DLNode *temp = new DLNode();
        temp->data = e;

        temp->next = L;
        temp->prior = tail;
        tail->next = temp;
        L->prior = temp;
        
        tail = temp;
        cin >> e;
    }

    return L;
}

DLinkList ReverseList_HeadInsert(DLinkList &L)
{

    DLNode *p = L->next;
    DLNode *r = nullptr;

    L->next = L;
    L->prior = L;
    
    while(p != L)
    {
        r = p->next;
        p->next = L->next;
        p->prior = L;
        L->next = p;
        p->next->prior = p;
        p = r;
    }

    return L;
}