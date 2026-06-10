#include <iostream>
#define MaxSize 100

using namespace std;

typedef int ElemType;

typedef struct{
    ElemType data[MaxSize];
    int top;
}Stack;

void InitStack(Stack &S) {
    S.top = -1;
}

bool StackEmpty(Stack S) {
    return S.top == -1;
}

bool StackOverflow(Stack S) {
    return S.top == MaxSize - 1;
}

bool Push(Stack &S, ElemType x) {
    if (StackOverflow(S)) return false;
    S.data[++S.top] = x;
    return true;
}

bool Pop(Stack &S, ElemType &x) {
    if (StackEmpty(S)) return false;
    x = S.data[S.top--];
    return true;
}

bool EnQueue(Stack &s1, Stack &s2, ElemType x)
{
    if(!StackOverflow(s1))
    {
        Push(s1, x);
        return true;
    }
    else if(StackOverflow(s1) && StackEmpty(s2))
    {
        ElemType temp;
        while(!StackEmpty(s1))
        {
            Pop(s1, temp);
            Push(s2, temp);
        }

        Push(s1, x);
        return true;
    }
    else
    {
        return false;
    }
}

bool DeQueue(Stack &s1, Stack &s2, ElemType &x)
{
    if(!StackEmpty(s2))
    {
        Pop(s2, x);
        return true;
    }
    else if(StackEmpty(s1) && StackEmpty(s2))
    {
        return false;
    }
    else
    {
        ElemType temp;
        while(!StackEmpty(s1))
        {
            Pop(s1, temp);
            Push(s2, temp);
        }

        Pop(s2, x);
        return true;
    }
}

bool QueueEmpty(Stack s1, Stack s2)
{
    return StackEmpty(s1) && StackEmpty(s2);
}