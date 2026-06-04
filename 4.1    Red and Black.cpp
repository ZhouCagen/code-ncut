//问题描述
//
//There is a rectangular room，covered with square tiles.Each tile is colored either red or black.A man is standing on a black tile.From a tile， he can move to one of four adjacent tiles.But he can't move on red tiles， he can move only on black tiles.
//
//Write a program to count the number of black tiles which he can reach by repeating the moves described above.
//
//输入形式
//
//The input consists of multiple data sets.A data set starts with a line containing two positive integers W and H; W and H are the numbers of tiles in the x - and y - directions， respectively.W and H are not more than 20.
//
//There are H more lines in the data set, each of which includes W characters.Each character represents the color of a tile as follows.
//
//'.' - a black tile
//
//'#' - a red tile
//
//'@' - a man on a black tile(appears exactly once in a data set)
//
//The end of the input is indicated by a line consisting of two zeros.
//
//输出形式
//
//For each data set， your program should output a line which contains the number of tiles he can reach from the initial tile(including itself).
//
//样例输入
//
//6 9
//
//....#.
//
//.....#
//
//......
//
//......
//
//......
//
//......
//
//......
//
//#@...#
//
//.#..#.
//
//0 0
//
//样例输出
//
//45
//
//中文翻译：
//
//问题描述
//
//有一个长方形的房间，用方瓷砖覆盖地面。瓷砖的颜色是红色或黑色。一个人站在一个黑色的瓷砖上。从一个瓷砖可以移动到相邻的四个瓷砖上。但他不能移动到红色瓷砖上，他仅可以在黑色的瓷砖上移动。
//
//编写一个程序来计算他可以到达的黑瓷砖数量。
//
//输入形式
//
//输入由多组数据组成。一组数据由一个包含两个正整数W和H开始；W和H分别是在x和y方向的瓷砖数。W和H不超过20。
//
//有H行的数据，其中每行包括W个字符。每个字符代表一个瓷砖的颜色。
//
//'.' - 黑色瓷砖
//
//'#' - 红色瓷砖
//
//'@' - 黑色瓷砖的一个人（在数据中只出现一次）
//
//输入结束 : 由两个零组成的数表示。
//
//输出形式
//
//对于每组数据，你的程序应该输出一行，包含了从最初的瓷砖开始可以达到的瓷砖数量（包括最初的瓷砖）。

//#include <iostream>
//#include <vector>
//using namespace std;
//
//int dx[] = { 1, 0, -1, 0 };
//int dy[] = { 0, 1, 0, -1 };
//int redandblack(int x, int y, int W, int H, vector<vector<char>>& v1)
//{
//	if (x < 0 || y < 0 || x >= W || y >= H) return 0;
//	if (v1[x][y] == '#') return 0;
//
//	v1[x][y] = '#';
//
//	int rst = 1;
//	for (int i = 0; i < 4; i++)
//	{
//		rst += redandblack(x + dx[i], y + dy[i], W, H, v1);
//	}
//	return rst;
//}
//
//int main()
//{
//	int H, W, x = 0, y = 0;
//
//	vector<vector<char>> v1;
//
//	while (cin >> H >> W)
//	{
//		if (H == 0 && W == 0) break;
//
//		v1.resize(W, vector<char>(H));
//
//		for (int i = 0; i < W; i++)
//		{
//			for (int j = 0; j < H; j++)
//			{
//				cin >> v1[i][j];
//				if (v1[i][j] == '@')
//				{
//					x = i;
//					y = j;
//				}
//			}
//		}
//		cout << redandblack(x, y, W, H, v1) << endl;
//	}
//
//	return 0;
//}