//#include<iostream>
//#include<string>
//using namespace std;
//int main()
//{
//    string n;
//    int s, i, x, l, m;
//    while (cin >> n >> s)
//    {
//        i = -1, m = 0, x = 0;
//        l = n.length();
//        while (x < s && m == 0)
//        {
//            i++;
//            if (n[i] > n[i + 1])//出现递减,删除递减的首数字 
//            {
//                n = n.erase(i, 1);
//                x++;// x统计删除数字的个数 
//                i = -1;//从头开始查递减区间 
//            }
//            if (i == l - x - 2 && x < s)
//                m = 1;//已经无递减区间,m=1脱离循环 
//        }
//        cout << n.substr(0, l - s + x) << endl;//只打印剩下的左边l-(s-x)个数字 
//    }
//    return 0;
//}