#include <iostream>
#define MaxSize 50

using namespace std;

typedef int ElemType;

typedef struct{
    ElemType data [MaxSize];
    int top1;
    int top2;
}SharedStack;

void InitStack(SharedStack &s)
{
    s.top1 = -1;
    s.top2 = MaxSize;
}

bool StackEmpty(SharedStack s, int stackNum)
{
    if (stackNum == 1) return s.top1 == -1;
    else if (stackNum == 2) return s.top2 == MaxSize;
    else return false;
}

bool GetTop(SharedStack s, int stackNum, ElemType &e)
{
    if(stackNum == 1)
    {
        if(s.top1 == -1) return false;

        e = s.data[s.top1];
        return true;
    }
    else if(stackNum == 2)
    {
        if(s.top2 == MaxSize) return false;

        e = s.data[s.top2];
        return true;
    }
    else return false;
}

bool Push(SharedStack &s, int stackNum, ElemType e)
{
    if(s.top1 + 1 == s.top2) return false;
    if(stackNum == 1)
    {
        s.data[++s.top1] = e;
        return true;
    }
    else if(stackNum == 2)
    {
        s.data[--s.top2] = e;
        return true;
    }
    else return false;
}

bool Pop(SharedStack &s, int stackNum, ElemType &e)
{
    if(stackNum == 1)
    {
        if(s.top1 == -1) return false;

        e = s.data[s.top1--];
        return true;
    }
    else if(stackNum == 2)
    {
        if(s.top2 == MaxSize) return false;

        e = s.data[s.top2++];
        return true;
    }
    else return false;
}

int StackLen(SharedStack s)
{
    return MaxSize + 1 + s.top1 - s.top2;
}

void ClearStack(SharedStack &s)
{
    s.top1 = -1;
    s.top2 = MaxSize;
}