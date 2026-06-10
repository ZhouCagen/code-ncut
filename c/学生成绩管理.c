//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int main()
//{
//    int arr1[3][3] = { {0} };
//    char xm1[3][100] = { {'\0'} };
//    int xuehao[3], i, j = 0;
//    for (i = 0; i < 3; i++)
//    {
//        scanf("%d", &xuehao[i]);
//        scanf("%s", xm1[i]);
//        for (j = 0; j < 3; j++)
//        {
//            scanf("%d", &arr1[i][j]);
//        }
//    }
//    int sum[3] = { 0 };
//    for (i = 0; i < 3; i++)
//    {
//        for (j = 0; j < 3; j++)
//        {
//            sum[i] = sum[i] + arr1[i][j];
//        }
//    }
//    int idx[3] = { 0,1,2 };
//    for (i = 0; i < 3; i++)
//    {
//        for (j = 0; j < 2- i; j++)
//        {
//            if (sum[j] < sum[j + 1])
//            {
//                int z = idx[j];
//                idx[j] = idx[j + 1];
//                idx[j + 1] = z;
//
//                z = sum[j];
//                sum[j] = sum[j + 1];
//                sum[j + 1] = z;
//            }
//        }
//    }
//    float Maths = (arr1[0][0] + arr1[1][0] + arr1[2][0]) / 3.0;
//    float English = (arr1[0][1] + arr1[1][1] + arr1[2][1]) / 3.0;
//    float Computer = (arr1[0][2] + arr1[1][2] + arr1[2][2]) / 3.0;
//    printf("NO   Name   Maths   Engilsh   Computer   Total\n");
//    for (i = 0; i < 3; i++)
//    {
//        printf("%d    ", xuehao[idx[i]]);
//        printf("%s    ", xm1[idx[i]]);
//        for (j = 0; j < 3; j++)
//        {
//            printf("%d    ", arr1[idx[i]][j]);
//        }
//        printf("%d\n", sum[i]);
//    }
//    printf("average of Maths:%.2f\n", Maths);
//    printf("average of English:%.2f\n", English);
//    printf("average of Computer:%.2f\n", Computer);
//    return 0;
//
//
//}