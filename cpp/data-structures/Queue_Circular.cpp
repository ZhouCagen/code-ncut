#include <iostream>
#define MaxSize 50

using namespace std;

typedef int ElemType;

typedef struct{
    ElemType data[MaxSize];
    int front;
    int rear;
}CircularQueue;

void InitQueue(CircularQueue &q)
{
    q.front = q.rear = 0;
}

bool QueueEmpty(CircularQueue q)
{
    return q.front == q.rear;
}

bool EnQueue(CircularQueue &q, ElemType e)
{
    if((q.rear + 1) % MaxSize == q.front) return false;

    q.data[q.rear] = e;
    q.rear = (q.rear + 1) % MaxSize;
    return true;
}

bool DeQueue(CircularQueue &q, ElemType &e)
{
    if(q.front == q.rear) return false;

    e = q.data[q.front];
    q.front = (q.front + 1) % MaxSize;

    return true;
}

bool GetHead(CircularQueue q, ElemType &e)
{
    if(q.front == q.rear) return false;

    e = q.data[q.front];
    return true;
}

int QueueLen(CircularQueue q)
{
    return (q.rear - q.front + MaxSize) % MaxSize;
}

void ClearQueue(CircularQueue &q)
{
    q.front = 0;
    q.rear = 0;
}