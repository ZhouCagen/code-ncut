#include <iostream>
using namespace std;

typedef int ElemType;

void move(ElemType a[], int n)
{
    int low = 0, high = n - 1;
    while (low < high)
    {
        while (low < high && a[high] % 2 == 0)
            high--;

        while (low < high && a[low] % 2 == 1)
            low++;
        if (low < high)
        {
            int temp = a[low];
            a[low] = a[high];
            a[high] = a[low];
            low++;
            high--;
        }
    }
}

int getKthElem(ElemType a[], int low, int high, int k)
{
    int pivot = a[low];
    int low_temp = low;
    int high_temp = high;
    while (low < high)
    {
        while (low < high && a[high] >= pivot)
            high--;
        a[low] = a[high];

        while (low < high && a[low] <= pivot)
            low++;
        a[high] = a[low];
    }

    a[low] = pivot;

    if (low == k)
        return a[low];
    else if (low < k)
        return getKthElem(a, low + 1, high_temp, k);
    else
        return getKthElem(a, low_temp, low - 1, k);
}
