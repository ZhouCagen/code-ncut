#include <iostream>
#define MaxSize 50

using namespace std;

typedef int ElemType;

typedef struct{
    ElemType data [MaxSize];
    int top;
}StaticSequenceStack;

void InitStack(StaticSequenceStack &s)
{
    s.top = -1;
}

bool StackEmpty(StaticSequenceStack s)
{
    return s.top == -1;
}

bool GetTop(StaticSequenceStack s, ElemType &e)
{
    if(s.top == -1) return false;
    e = s.data[s.top];
    return true;
}

bool Push(StaticSequenceStack &s, ElemType e)
{
    if(s.top + 1 == MaxSize) return false;

    s.data[++s.top] = e;
    return true;
}

bool Pop(StaticSequenceStack &s, ElemType &e)
{
    if(s.top == -1) return false;

    e = s.data[s.top--];
    return true;
}

int StackLen(StaticSequenceStack s)
{
    return s.top + 1;
}

void ClearStack(StaticSequenceStack &s)
{
    s.top = -1;
}



typedef struct{
    ElemType *data;
    int top;
    int capacity;
}DynamicSequenceStack;

void InitStack(DynamicSequenceStack &s)
{
    s.data = new ElemType[MaxSize];
    s.top = -1;
    s.capacity = MaxSize;
}

void ExpandCapacity(DynamicSequenceStack &s)
{
    int newCapacity = 2 * s.capacity;
    ElemType *newData = new ElemType[newCapacity];
    for(int i = 0; i <= s.top; i++)
    {
        newData[i] = s.data[i];
    }
    delete[] s.data;
    s.data = newData;
    s.capacity = newCapacity;
}

bool StackEmpty(DynamicSequenceStack s)
{
    return s.top == -1;
}

bool GetTop(DynamicSequenceStack s, ElemType &e)
{
    if(s.top == -1) return false;
    e = s.data[s.top];
    return true;
}

bool Push(DynamicSequenceStack &s, ElemType e)
{
    if(s.top + 1 == s.capacity) ExpandCapacity(s);
    s.data[++s.top] = e;
    return true;
}

bool Pop(DynamicSequenceStack &s, ElemType &e)
{
    if(s.top == -1) return false;
    e = s.data[s.top--];
    return true;
}

int StackLen(DynamicSequenceStack s)
{
    return s.top + 1;
}

void ClearStack(DynamicSequenceStack &s)
{
    s.top = -1;
}

void FreeStack(DynamicSequenceStack &s)
{
    if(s.data != nullptr)
    {
        delete[] s.data;
        s.data = nullptr;
        s.top = -1;
        s.capacity = 0;
    }
}





