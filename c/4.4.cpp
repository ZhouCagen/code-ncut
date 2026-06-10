//问题描述
//会下国际象棋的人都很清楚：皇后可以在横、竖、斜线上不限步数地吃掉其他棋子。如何将8个皇后放在棋盘上（有8 * 8个方格），使它们谁也不能被吃掉！这就是著名的八皇后问题。
//对于某个满足要求的8皇后的摆放方法，定义一个皇后串a与之对应，即a = b1b2...b8，其中bi为相应摆法中第i行皇后所处的列数。已经知道8皇后问题一共有92组解（即92个不同的皇后串）。
//给出一个数b，要求输出第b个串。串的比较是这样的：皇后串x置于皇后串y之前，当且仅当将x视为整数时比y小。
//输入形式
//第1行是测试数据的组数n，后面跟着n行输入。每组测试数据占1行，包括一个正整数b(1 <= b <= 92)
//输出形式
//输出有n行，每行输出对应一个输入。输出应是一个正整数，是对应于b的皇后串。
//样例输入
//2
//1
//92
//样例输出
//15863724
//84136275

//#include <iostream>
//#include <vector>
//using namespace std;
//vector<int> ans;
//void queen(vector<vector<bool>> vec, int n = 0, int cur = 0)
//{
//    // cout << cur << endl;
//    if (n == 8)
//    {
//        ans.push_back(cur);
//        return;
//    }
//
//    for (int i = 0; i < 8; i++)
//    {
//        if (vec[n][i])
//        {
//            vector<vector<bool>> temp = vec;
//            for (int j = 0; j < 8; j++)
//            {
//                temp[j][i] = 0;
//                if (i - j + n >= 0 && i - j + n < 8)
//                    temp[j][i + n - j] = 0;
//                if (i + j - n >= 0 && i + j - n < 8)
//                    temp[j][i + j - n] = 0;
//            }
//            queen(temp, n + 1, 10 * cur + i + 1);
//        }
//    }
//
//}
//
//int main()
//{
//    vector<vector<bool>> vec(8, vector<bool>(8, true));
//    queen(vec);
//    int t, n;
//    cin >> t;
//    while (t--)
//    {
//        cin >> n;
//        cout << ans[n - 1] << endl;
//    }
//    return 0;
//}