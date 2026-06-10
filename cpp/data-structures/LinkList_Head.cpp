#include <iostream>
using namespace std;

typedef int ElemType;

typedef struct LNode{
	ElemType data;
	struct LNode *next;
} LNode, *LinkList;

void InitList(LinkList &L)
{
	L = new LNode();
	L->data = 0;
	L->next = nullptr;
	return;
}

int Length(LinkList L)
{
	int length = 0;
	LNode* P = L->next;
	while(P!= nullptr)
	{
		length++;
		P = P->next;
	}

	return length;
}

LNode* LocateElem(LinkList L, ElemType e)
{
	LNode* P = L->next;
	while(P != nullptr && P->data != e)
	{
		P = P->next;
	}
	return P;
}

ElemType GetElem(LinkList L, int i)
{
	LNode* P = L->next;
	int pos = 1;
	while(P != nullptr && pos < i)
	{
		pos++;
		P = P->next;
	}

	if(P == nullptr || pos > i) return -1;

	return P->data;
}

bool ListInsert(LinkList &L, int i, ElemType e)
{
	LNode* P = L;
	int count = 1;
	while(count < i && P != nullptr)
	{
		count++;
		P = P->next;
	}

	if(count > i || P == nullptr) return false;

	LNode* temp = new LNode();
	temp->data = e;
	temp->next = P->next;
	P->next = temp;

	return true;
}

bool ListDelete(LinkList &L, int i, ElemType &e)
{
	if(i < 1) return false;

	LNode *P = L;
	int count = 1;
	while(P != nullptr && count < i)
	{
		count++;
		P = P->next;
	}

	if(count > i || P == nullptr || P->next == nullptr) return false;

	LNode *q = P->next;
	e = q->data;
	P->next = q->next;
	delete q;
	return true;
}

void PrintList(LinkList L)
{
	LNode *p = L->next;
	while(p != nullptr)
	{
		cout << p->data <<" ";
		p = p->next;
	}

	return;
}

bool Empty(LinkList L)
{
	return L->next == nullptr;
}

void DestroyList(LinkList &L)
{
	LNode *p = L;
	LNode *q;
	while(p != nullptr)
	{
		q = p;
		p = p->next;
		delete q;
	}

	L = nullptr;

	return;
}

void ClearList(LinkList &L)
{
	LNode *p = L->next;
	LNode *q;

	while(p != nullptr)
	{
		q = p;
		p = p->next;
		delete q;
	}

	L->next= nullptr;
	return;
}

LinkList LinkList_HeadInsert(LinkList &L)
{
	L = new LNode();
	L->data = 0;
	L->next = nullptr;
	ElemType x;
	cin >> x;

	while(x != 9999)
	{
		LNode *temp = new LNode();
		temp->data = x;
		temp->next = L->next;

		L->next = temp;

		cin >> x;
	}

	return L;
}

LinkList LinkList_TailInsert(LinkList &L)
{
	L = new LNode();
	L->data = 0;
	L->next = nullptr;

	LNode* tail = L;
	ElemType x;
	cin >> x;

	while(x != 9999)
	{
		LNode* temp = new LNode();
		temp->data = x;
		temp->next = nullptr;
		tail->next = temp;
		tail = temp;
		cin >> x;
	}

	return L;
}

LinkList ReverseList_HeadInsert(LinkList &L)
{
	LNode *p = L->next;
	LNode *r = nullptr;

	L->next = nullptr;
	while(p != nullptr)
	{
		r = p->next;
		p->next = L->next;
		L->next = p;
		p = r;
	}

	return L;
}

LinkList ReverseList_ThreePointers(LinkList &L)
{
    if(L->next == nullptr || L->next->next == nullptr) return L;

    LNode *pre, *p = L->next, *r = p->next;
    p->next = nullptr;
    while(r != nullptr)
    {
        pre = p;
        p = r;
        r = r->next;
        p->next = pre;
    }

    L->next = p;
    return L;
}