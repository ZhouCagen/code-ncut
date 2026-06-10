//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//int main()
//{
//	int k, i;
//	scanf("%d", &k);
//	char gender[10];
//	float WBC = 0, RBC = 0;
//	int HGB = 0, HCT = 0, PLT = 0;
//	for (i = 1; i <= k; i++)
//	{
//		scanf("%s",gender);
//		scanf("%f %f %d %d %d", &WBC, &RBC, &HGB, &HCT, &PLT);
//		int a = 0;
//		if (WBC < 4.0 || WBC>10.0)
//		{
//			a++;
//		}
//		if (RBC < 3.5 ||RBC>5.5)
//		{
//			a++;
//		}
//		if (strcmp(gender, "female") == 0)
//		{
//			if (HGB < 110 ||HGB>150)
//			{
//				a++;
//			}
//			if (HCT < 36 || HCT>40)
//			{
//				a++;
//			}
//		}
//		if (strcmp(gender, "male") == 0)
//		{
//			if (HGB < 120 ||HGB>160)
//			{
//				a++;
//			}
//			if (HCT < 42 || HCT>48)
//			{
//				a++;
//			}
//		}
//		if (PLT < 100 || PLT>300)
//		{
//			a++;
//		}
//		if (a == 0)
//		{
//			printf("normal\n");
//		}
//		else
//		{
//			printf("%d\n", a);
//		}
//	}
//	return 0;
//}
