#include <iostream>
#define MaxSize 50

using namespace std;

typedef int ElemType;

typedef struct LNode{
    ElemType data;
    struct LNode *next;
}LNode, *LinkStack;

void InitStack(LinkStack &s)
{
    s = nullptr;
}

bool StackEmpty(LinkStack s)
{
    return s == nullptr;
}

bool Push(LinkStack &s, ElemType e)
{
    LNode *temp = new LNode();
    temp->data = e;
    temp->next = s;
    s = temp;

    return true;
}

bool Pop(LinkStack &s, ElemType &e)
{
    if(s == nullptr) return false;
    LNode *temp = s;
    e = temp->data;
    s = s->next;
    delete temp;
    
    return true;
}

bool GetTop(LinkStack s, ElemType &e)
{
    if(s == nullptr) return false;
    
    e = s->data;
    return true;
}

void FreeStack(LinkStack &s)
{
    LNode *temp;
    while(s != nullptr)
    {
        temp = s;
        s = s->next;
        delete temp;
    }
}