//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int main()
//{
//    int m, n, have = 1, get = 0, i, j, max, h,s=0, k=0;
//    int arr[10][10];
//    scanf("%d%d", &m, &n);
//    for (i = 0; i <m ; i++)
//    {
//        for (j = 0; j < n ; j++)
//            scanf("%d", &arr[i][j]);
//    }
//    for (i = 0; i < m ; i++)
//    {
//        max = arr[i][0];
//        have = 1;
//        for (j = 1; j < n; j++)
//        {
//            if (arr[i][j] > max)
//            {
//                max = arr[i][j];
//                s = i;
//                k = j;
//            }
//        }
//        for (h = 0; h < m; h++)
//        {
//            if (max > arr[h][k])
//            {
//                have = 0;
//                break;
//            }
//        }
//        if (have == 1)
//        {
//            printf("%d %d %d", s, k, max);
//            get = 1;
//        }
//    }
//    if (get == 0)
//        printf("no");
//    return 0;
//
//}