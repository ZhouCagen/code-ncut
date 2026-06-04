//#include <iostream>
//#include <string>
//#include <stack>
//using namespace std;
//
//int main()
//{
//	stack<char> char_stack;
//	string ArithmeticExpression;
//	getline(cin, ArithmeticExpression);
//
//	bool correct = true;
//
//	bool hasPairingSymbol = false;
//	for (char ch : ArithmeticExpression)
//	{
//		if (ch == '(' || ch == '[' || ch == '{')
//		{
//			char_stack.push(ch);
//		}
//		else if (ch == ')' || ch == ']' || ch == '}')
//		{
//			if (char_stack.empty())
//			{
//				correct = false;
//				break;
//			}
//			hasPairingSymbol = true;
//			char top = char_stack.top();
//			if ((ch == ')' && top == '(') || (ch == ']' && top == '[') || (ch == '}' && top == '{'))
//			{
//				char_stack.pop();
//			}
//			else
//			{
//				correct = true;
//				break;
//			}
//		}
//	}
//
//	if (correct == 1)
//	{
//		if (hasPairingSymbol == 1)
//		{
//			cout << "The parentheses in the expression are paired correctly" << endl;
//		}
//		else
//		{
//			cout << "The parentheses in the expression are not paired correctly" << endl;
//		}
//	}
//	else
//	{
//		cout << "The parentheses in the expression are not paired correctly" << endl;
//	}
//
//	return 0;
//}