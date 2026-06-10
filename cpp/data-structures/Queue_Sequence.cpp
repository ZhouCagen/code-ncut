#include <iostream>
#define MaxSize 50

using namespace std;

typedef int ElemType;

typedef struct{
    ElemType data[MaxSize];
    int front;
    int rear;
}StaticSequenceQueue;

void InitQueue(StaticSequenceQueue &q)
{
    q.front = q.rear = 0;
}

bool QueueEmpty(StaticSequenceQueue q)
{
    return q.front == q.rear;
}

bool EnQueue(StaticSequenceQueue &q, ElemType e)
{
    if(q.rear == MaxSize) return false;

    q.data[q.rear++] = e;
    return true;
}

bool DeQueue(StaticSequenceQueue &q, ElemType &e)
{
    if(q.front == q.rear) return false;

    e = q.data[q.front++];
    return true;
}

bool GetHead(StaticSequenceQueue q, ElemType &e)
{
    if(q.front == q.rear) return false;

    e = q.data[q.front];
    return true;
}

int QueueLen(StaticSequenceQueue q)
{
    return q.rear - q.front;
}

void ClearQueue(StaticSequenceQueue &q)
{
    q.front = 0;
    q.rear = 0;
}




typedef struct{
    ElemType *data;
    int front;
    int rear;
    int capacity;
}DynamicSequenceQueue;

void InitQueue(DynamicSequenceQueue &q)
{
    q.data = new ElemType[MaxSize];
    q.front = 0;
    q.rear = 0;
    q.capacity = MaxSize;
}

void ExpandCapacity(DynamicSequenceQueue &q)
{
    int newCapacity = 2 * q.capacity;
    ElemType *newData = new ElemType[newCapacity];
    int k = 0;
    for(int i = q.front; i < q.rear; i++)
    {
        newData[k++] = q.data[i];
    }
    delete[] q.data;
    q.data = newData;
    q.capacity = newCapacity;

    q.front = 0;
    q.rear = k;
}

bool QueueEmpty(DynamicSequenceQueue q)
{
    return q.front == q.rear;
}

bool EnQueue(DynamicSequenceQueue &q, ElemType e)
{
    if(q.rear == q.capacity) ExpandCapacity(q);

    q.data[q.rear++] = e;
    return true;
}

bool DeQueue(DynamicSequenceQueue &q, ElemType &e)
{
    if(q.front == q.rear) return false;

    e = q.data[q.front++];
    return true;
}

bool GetHead(DynamicSequenceQueue q, ElemType &e)
{
    if(q.front == q.rear) return false;

    e = q.data[q.front];
    return true;
}

int QueueLen(DynamicSequenceQueue q)
{
    return q.rear - q.front;
}

void ClearQueue(DynamicSequenceQueue &q)
{
    q.front = 0;
    q.rear = 0;
}

void FreeQueue(DynamicSequenceQueue &q)
{
    if(q.data != nullptr)
    {
        delete[] q.data;
        q.data = nullptr;
        q.front = 0;
        q.rear = 0;
    }
}