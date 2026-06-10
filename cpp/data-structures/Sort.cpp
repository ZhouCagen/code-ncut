#include <iostream>
using namespace std;

typedef int ElemType;

void InsertSort(ElemType a[], int n)
{
    for (int i = 2; i <= n; i++)
    {
        if (a[i] < a[i - 1])
        {
            int j = 0;
            a[0] = a[i];
            for (j = i - 1; a[0] < a[j]; j--)
                a[j + 1] = a[j];

            a[j + 1] = a[0];
        }
    }
}

void BinaryInsertSort(ElemType a[], int n)
{
    for (int i = 2; i <= n; i++)
    {
        a[0] = a[i];
        int low = 1, high = i - 1;
        while (low <= high)
        {
            int mid = high + (high - low) / 2;
            if (a[0] < a[mid])
                high = mid - 1;
            else
                low = mid + 1;
        }

        for (int j = i - 1; j >= low; j--)
            a[j + 1] = a[j];

        a[high + 1] = a[0];
    }
}

void ShellSort(ElemType a[], int n)
{
    for (int dk = n / 2; dk >= 1; dk /= 2)
    {
        for (int i = dk + 1; i <= n; i++)
        {
            if (a[i] < a[i - dk])
            {
                a[0] = a[i];
                int j = 0;
                for (j = i - dk; j > 0 && a[0] < a[j]; j -= dk)
                    a[j + dk] = a[j];
                a[j + dk] = a[0];
            }
        }
    }
}

void BubbleSort(ElemType a[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        bool flag = false;
        for (int j = n - 1; j > i; j--)
        {
            if (a[j] < a[j - 1])
            {
                int temp = a[j - 1];
                a[j - 1] = a[j];
                a[j] = temp;
                flag = true;
            }
        }
        if (!flag)
            return;
    }
}

int Partition(ElemType a[], int low, int high)
{
    ElemType pivot = a[low];
    while (low < high)
    {
        while (pivot <= a[high] && low < high)
            high--;
        a[low] = a[high];
        while (pivot >= a[low] && low < high)
            low++;
        a[high] = a[low];
    }

    a[low] = pivot;

    return low;
}

void QuickSort(ElemType a[], int low, int high)
{
    if (low < high)
    {
        int pivotPos = Partition(a, low, high);
        QuickSort(a, low, pivotPos - 1);
        QuickSort(a, pivotPos + 1, high);
    }
}

void SelectSort(ElemType a[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int min = i;
        for (int j = i + 1; j < n; j++)
        {
            if (a[j] < a[min])
                min = j;
        }

        if (min != i)
        {
            int temp = a[i];
            a[i] = a[min];
            a[min] = a[i];
        }
    }
}

void HeapAdjust(ElemType a[], int k, int len)
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
        HeapAdjust(a, i, len);
}

void HeapSort(ElemType a[], int len)
{
    BuildMaxHeap(a, len);
    for (int i = len; i > 1; i--)
    {
        int temp = a[1];
        a[1] = a[i];
        a[i] = temp;

        HeapAdjust(a, 1, i - 1);
    }
}

void Merge(ElemType a[], ElemType help[], int low, int mid, int high)
{
    for (int i = low; i <= high; i++)
        help[i] = a[i];

    int i = low;
    int j = mid + 1;
    int k = low;

    while (i <= mid && j <= high)
    {
        if (help[i] <= help[j])
            a[k++] = help[i++];

        else
            a[k++] = help[j++];
    }

    while (i <= mid)
        a[k++] = help[i++];

    while (j <= high)
        a[k++] = help[j++];
}

void MergeSort(ElemType a[], ElemType help[], int low, int high)
{
    if (low < high)
    {
        int mid = low + (high - low) / 2;
        MergeSort(a, help, low, mid);
        MergeSort(a, help, mid + 1, high);
        Merge(a, help, low, mid, high);
    }
}

const int Radix = 10;

typedef struct Node
{
    ElemType data;
    struct Node* next;
} LNode;

int getDigit(int number, int digit)
{
    for (int i = 1; i <= digit - 1; i++)
        number /= 10;

    return number % 10;

    // return number % (int)pow(10, digit) / (int)pow(10, digit - 1)
}

int getMaxDigit(ElemType a[], int n)
{
    int maxValue = a[0];
    for (int i = 1; i < n; i++)
        maxValue = (a[i] > maxValue) ? a[i] : maxValue;

    if (maxValue == 0)
        return 1;

    int digitCount = 0;
    while (maxValue > 0)
    {
        maxValue /= 10;
        digitCount++;
    }

    return digitCount;

    // return (int)log10(maxValue) + 1
}

void radixSort(ElemType a[], int n)
{
    LNode* head[Radix];
    LNode* tail[Radix];

    int maxDigit = getMaxDigit(a, n);

    for (int i = 1; i <= maxDigit; i++)
    {
        for (int j = 0; j < Radix; j++)
        {
            head[j] = nullptr;
            tail[j] = nullptr;
        }

        for (int j = 0; j < n; j++)
        {
            int digitKey = getDigit(a[j], i);
            LNode* newNode = new LNode();
            newNode->data = a[j];
            newNode->next = nullptr;

            if (head[digitKey] == nullptr)
            {
                head[digitKey] = newNode;
                tail[digitKey] = newNode;
            }
            else
            {
                tail[digitKey]->next = newNode;
                tail[digitKey] = newNode;
            }
        }

        int index = 0;
        for (int j = 0; j < Radix; j++)
        {
            LNode* p = head[j];
            while (p != nullptr)
            {
                a[index++] = p->data;
                LNode* temp = p;
                p = p->next;
                delete temp;
            }
        }
    }
}

int getMaxValue(ElemType a[], int n)
{
    int maxValue = a[0];
    for (int i = 1; i < n; i++)
        maxValue = (a[i] > maxValue) ? a[i] : maxValue;

    return maxValue;
}

void CountingSort(ElemType a[], int n)
{
    int maxValue = getMaxValue(a, n);
    int* count = new int[maxValue + 1];
    int* result = new int[n];

    for (int i = 0; i <= maxValue; i++)
        count[i] = 0;

    for (int i = 0; i < n; i++)
        count[a[i]]++;

    for (int i = 1; i <= maxValue; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--)
        result[--count[a[i]]] = a[i];

    for (int i = 0; i < n; i++)
        a[i] = result[i];

    delete[] count;
    delete[] result;
}
