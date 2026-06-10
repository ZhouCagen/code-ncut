//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int main()
//{
//    int m, n,get = 0, i, j, max, h;
//    int arr[10][10];
//    scanf("%d%d", &m, &n);
//    for (i = 0; i <= m - 1; i++)
//    {
//        for (j = 0; j <= n - 1; j++)
//            scanf("%d", &arr[i][j]);
//    }
//    for (i = 0; i <= m - 1; i++)
//    {
//        max = 0;
//        int have = 1;
//        for (j = 1; j <= n - 1; j++)
//        {
//            if (arr[i][j] >= arr[i][max])
//            {
//                max = j;
//            }
//        }
//        for (h = 0; h <= m - 1; h++)
//        {
//            if (arr[i][max] > arr[h][max])
//            {
//                have = 0;
//                break;
//            }
//        }
//        if (have == 1)
//        {
//            printf("%d %d %d", i, max, arr[i][max]);
//            get=1;
//        }
//    }
//    if (get == 0)
//        printf("no");
//    return 0;
//
//}