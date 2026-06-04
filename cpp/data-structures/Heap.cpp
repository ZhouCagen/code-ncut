#include <iostream>
using namespace std;

typedef int ElemType;

void MaxHeapAdjust(ElemType a[], int k, int len)
{
    a[0] = a[k];
    for (int i = 2 * k; i <= len; i *= 2)
    {
        if (i < len && a[i] < a[i + 1])
            i++;

        if (a[0] >= a[i])
            break;
        else
        {
            a[k] = a[i];
            k = i;
        }
    }
    a[k] = a[0];
}

void BuildMaxHeap(ElemType a[], int len)
{
    for (int i = len / 2; i > 0; i--)
        MaxHeapAdjust(a, i, len);
}

void MinHeapAdjust(ElemType a[], int k, int len)
{
    a[0] = a[k];
    for (int i = 2 * k; i <= len; i *= 2)
    {
        if (i < len && a[i] > a[i + 1])
            i++;

        if (a[0] <= a[i])
            break;
        else
        {
            a[k] = a[i];
            k = i;
        }
    }
    a[k] = a[0];
}

void BuildMinHeap(ElemType a[], int len)
{
    for (int i = len / 2; i > 0; i--)
        MinHeapAdjust(a, i, len);
}

void HeapSort(ElemType a[], int len)
{
    BuildMaxHeap(a, len);
    for (int i = len; i > 1; i--)
    {
        int temp = a[1];
        a[1] = a[i];
        a[i] = temp;

        MaxHeapAdjust(a, 1, i - 1);
    }
}

void MaxHeapInsert(ElemType a[], int& len, ElemType x)
{
    int i = ++len;
    while (i > 1 && a[i / 2] < x)
    {
        a[i] = a[i / 2];
        i /= 2;
    }

    a[i] = x;
}

void MinHeapInsert(ElemType a[], int& len, ElemType x)
{
    int i = ++len;
    while (i > 1 && a[i / 2] > x)
    {
        a[i] = a[i / 2];
        i /= 2;
    }

    a[i] = x;
}

bool MaxHeapDelete(ElemType a[], int& len, int pos, ElemType& deletedValue)
{
    if (pos < 1 || pos > len)
        return false;

    deletedValue = a[pos];

    if (pos == len)
    {
        len--;
        return true;
    }

    a[pos] = a[len];
    len--;

    if (pos > 1 && a[pos] > a[pos / 2])
    {
        a[0] = a[pos];
        while (pos > 1 && a[pos / 2] < a[0])
        {
            a[pos] = a[pos / 2];
            pos /= 2;
        }

        a[pos] = a[0];
    }
    else
    {
        a[0] = a[pos];
        for (int i = 2 * pos; i <= len; i *= 2)
        {
            if (i < len && a[i] < a[i + 1])
                i++;

            if (a[0] >= a[i])
                break;
            else
            {
                a[pos] = a[i];
                pos = i;
            }
        }

        a[pos] = a[0];
    }

    return true;
}

bool MinHeapDelete(ElemType a[], int& len, int pos, ElemType& deletedValue)
{
    if (pos < 1 || pos > len)
        return false;

    deletedValue = a[pos];

    if (pos == len)
    {
        len--;
        return true;
    }

    a[pos] = a[len];
    len--;

    if (pos > 1 && a[pos] < a[pos / 2])
    {
        a[0] = a[pos];
        while (pos > 1 && a[pos / 2] > a[0])
        {
            a[pos] = a[pos / 2];
            pos /= 2;
        }

        a[pos] = a[0];
    }
    else
    {
        a[0] = a[pos];
        for (int i = 2 * pos; i <= len; i *= 2)
        {
            if (i < len && a[i] > a[i + 1])
                i++;

            if (a[0] <= a[i])
                break;
            else
            {
                a[pos] = a[i];
                pos = i;
            }
        }

        a[pos] = a[0];
    }

    return true;
}
