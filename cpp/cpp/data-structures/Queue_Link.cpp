#include <iostream>
#define MaxSize 50

using namespace std;

typedef int ElemType;

typedef struct LNode{
    ElemType data;
    struct LNode *next;
}LNode;

typedef struct{
    LNode *front, *rear;
}NoHeadLinkQueue;

void InitQueue(NoHeadLinkQueue &q)
{
    q.front = nullptr;
    q.rear = nullptr;
}

bool QueueEmpty(NoHeadLinkQueue q)
{
    return q.front == nullptr;
}

bool EnQueue(NoHeadLinkQueue &q, ElemType e)
{
    LNode *temp = new LNode();
    temp->data = e;
    temp->next = nullptr;
    if(q.front == nullptr)
    {
        q.front = temp;
        q.rear = temp;
    }
    else
    {
        q.rear->next = temp;
        q.rear = temp;
    }
    return true;
}

bool DeQueue(NoHeadLinkQueue &q, ElemType &e)
{
    if(q.front == nullptr) return false;
    LNode *temp = q.front;
    e = temp->data;
    q.front = temp->next;
    if(q.rear == temp)
    {
        q.front = nullptr;
        q.rear = nullptr;
    }
    delete temp;

    return true;
}

bool GetHead(NoHeadLinkQueue q, ElemType &e)
{
    if(q.front == nullptr) return false;
    e = q.front->data;
    return true;
}

int QueueLen(NoHeadLinkQueue q)
{
    int count = 0;
    LNode *p = q.front;
    while(p != nullptr)
    {
        p = p->next;
        count++;
    }

    return count;
}

void ClearQueue(NoHeadLinkQueue &q)
{
    LNode *p = q.front;
    while(p != nullptr)
    {
        LNode *temp = p;
        p = p->next;
        delete temp;
    }

    q.front = nullptr;
    q.rear = nullptr;
}

typedef struct{
    LNode *front, *rear;
}LinkQueue;

void InitQueue(LinkQueue &q)
{
    q.front = q.rear = new LNode();
    q.front->next = nullptr;
}

bool QueueEmpty(LinkQueue &q)
{
    return q.front == q.rear;
}

bool EnQueue(LinkQueue &q, ElemType e)
{
    LNode *temp = new LNode();
    temp->data = e;
    temp->next = nullptr;
    q.rear->next = temp;
    q.rear = temp;

    return true;
}

bool DeQueue(LinkQueue &q, ElemType &e)
{
    if(q.front == q.rear) return false;
    LNode *temp = q.front->next;
    e = temp->data;
    q.front->next = temp->next;
    if(q.rear == temp)
    {
        q.rear = q.front;
    }
    delete temp;

    return true;
}

bool GetHead(LinkQueue q, ElemType &e)
{
    if(q.front == q.rear) return false;
    e = q.front->next->data;

    return true;
}

int QueueLen(LinkQueue q)
{
    int count = 0;
    LNode *p = q.front->next;
    while(p != nullptr)
    {
        p = p->next;
        count++;
    }

    return count;
}

void ClearQueue(LinkQueue &q)
{
    LNode *p = q.front->next;
    while(p != nullptr)
    {
        LNode *temp = p;
        p = p->next;
        delete temp;
    }
    q.rear = q.front;
    q.front->next = nullptr;
}

void FreeQueue(LinkQueue &q)
{
    LNode *p = q.front;
    while(p != nullptr)
    {
        LNode *temp = p;
        p = p->next;
        delete temp;
    }

    q.front = nullptr;
    q.rear = nullptr;
}