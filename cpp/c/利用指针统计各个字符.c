//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int main()
//{
//	char ans[80] = { '\0' };
//	char* p = ans;
//	int i;
//	int dx = 0, xx = 0, space = 0, num = 0, zf = 0;
//	while ((*p++ = getchar()) != '\n');
//	*--p = '\0';
//	for (i = 0; ans[i] != '\0'; i++)
//	{
//		if (ans[i] >= 'A' && ans[i] <= 'Z')
//			dx++;
//		else if (ans[i] >= 'a' && ans[i] <= 'z')
//			xx++;
//		else if (ans[i] == ' ')
//			space++;
//		else if (ans[i] >= '0' && ans[i] <= '9')
//			num++;
//		else
//			zf++;
//	}
//	printf("%d %d %d %d %d", dx, xx, space, num, zf);
//	return 0;
//}