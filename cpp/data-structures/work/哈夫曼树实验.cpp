//#define _CRT_SECURE_NO_WARNINGS
//
//#include <iostream>
//#include <iomanip>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <chrono>
//#include <queue>
//#include <map>
//
//using namespace std;
//
//struct HuffmanTreeNode
//{
//	char ch;
//	int weight;
//	HuffmanTreeNode* leftChild, * rightChild;
//
//	HuffmanTreeNode(char c = ' ', int w = 0) :ch(c), weight(w), leftChild(nullptr), rightChild(nullptr) {}
//
//};
//
//void printHuffmanTreeAchieve(HuffmanTreeNode* node, int depth, std::ofstream& file)
//{
//	if (node == nullptr) return;
//
//	std::cout << std::setw(depth * 2) << " " << "(" << node->ch << ", " << node->weight << ")" << std::endl;
//	file << std::setw(depth * 2) << " " << "(" << node->ch << ", " << node->weight << ")" << std::endl;
//
//	printHuffmanTreeAchieve(node->leftChild, depth + 1, file);
//	printHuffmanTreeAchieve(node->rightChild, depth + 1, file);
//}
//
//void printHuffmanTree(HuffmanTreeNode* root)
//{
//	ofstream HuffmanTreePrintFile;
//	HuffmanTreePrintFile.open("HuffmanTreePrint.txt", ios_base::out);
//	if (HuffmanTreePrintFile.is_open())
//	{
//		printHuffmanTreeAchieve(root, 1, HuffmanTreePrintFile);
//		HuffmanTreePrintFile.close();
//	}
//	else
//	{
//		cerr << "Error：Unable to open HuffmanTreePrint.txt " << endl;
//		exit(0);
//	}
//	return;
//}
//
//struct Compare 
//{
//	bool operator()(HuffmanTreeNode* leftChild, HuffmanTreeNode* rightChild) 
//	{
//		return leftChild->weight > rightChild->weight;
//	}
//};
//
//HuffmanTreeNode* createHuffmanTree(const vector<pair<char, int>>& charWeights)
//{
//	priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, Compare>minHeap;
//
//	for (const auto &pair:charWeights)
//	{
//		minHeap.push(new HuffmanTreeNode(pair.first, pair.second));
//	}
//
//	while (minHeap.size() > 1)
//	{
//		HuffmanTreeNode* left = minHeap.top();
//		minHeap.pop();
//		HuffmanTreeNode* right = minHeap.top();
//		minHeap.pop();
//
//		HuffmanTreeNode* newNode = new HuffmanTreeNode(' ', left->weight + right->weight);
//		newNode->leftChild = left;
//		newNode->rightChild = right;
//
//		minHeap.push(newNode);
//	}
//
//	return minHeap.top();
//}
//
//// 未使用堆
//
///*
//HuffmanTreeNode* findSecondMinNode(vector <HuffmanTreeNode*>forest, HuffmanTreeNode* exclude)
//{
//	HuffmanTreeNode* secondMin = nullptr;
//	for (HuffmanTreeNode* node : forest)
//	{
//		if (node != exclude && (secondMin == nullptr || node->weight < secondMin->weight))
//		{
//			secondMin = node;
//		}
//	}
//
//	if (secondMin == nullptr)
//	{
//		throw("Error：secondMinNode is nullptr");
//	}
//
//	return secondMin;
//}
//
//HuffmanTreeNode* createHuffmanTree(int n, const vector<pair<char, int>>& charWeights)
//{
//	vector<HuffmanTreeNode*> forest;
//	forest.reserve(charWeights.size());
//
//
//	for (const auto& weightPair : charWeights)
//	{
//		HuffmanTreeNode* temp = new HuffmanTreeNode(weightPair.first, weightPair.second);
//		forest.push_back(temp);
//	}
//
//	while (forest.size() > 1)
//	{
//		auto min1 = min_element(forest.begin(), forest.end(),
//			[](HuffmanTreeNode* a, HuffmanTreeNode* b) {return a->weight < b->weight; });
//		HuffmanTreeNode* minNode = *min1;
//		forest.erase(min1);
//
//		HuffmanTreeNode* secondMinNode = findSecondMinNode(forest, minNode);
//		if (secondMinNode != nullptr)
//		{
//			auto it = find(forest.begin(), forest.end(), secondMinNode);
//			if (it != forest.end())
//			{
//				forest.erase(it);
//			}
//			HuffmanTreeNode* newNode = new HuffmanTreeNode(' ', minNode->weight + secondMinNode->weight);
//			newNode->leftChild = minNode;
//			newNode->rightChild = secondMinNode;
//
//			forest.push_back(newNode);
//		}
//		else
//		{
//			cerr << "Error: secondMinNode is nullptr" << endl;
//			exit(EXIT_FAILURE);
//		}
//	}
//
//	return forest.front();
//}
//*/
//
//void preorderSave(HuffmanTreeNode* node, ofstream& HuffmanTreeFile)
//{
//	if (node == nullptr)
//	{
//		return;
//	}
//
//	HuffmanTreeFile << node->ch << " " << node->weight << " " << endl;
//
//	preorderSave(node->leftChild, HuffmanTreeFile);
//	preorderSave(node->rightChild, HuffmanTreeFile);
//
//	return;
//}
//
//void saveTreeToFile(HuffmanTreeNode* root)
//{
//	ofstream HuffmanTreeFile;
//	HuffmanTreeFile.open("HuffmanTree.txt");
//
//	if (HuffmanTreeFile.is_open())
//	{
//		preorderSave(root, HuffmanTreeFile);
//		HuffmanTreeFile.close();
//
//		return;
//	}
//	else
//	{
//		cerr << "Error：Unable to open HuffmanTree.txt " << endl;
//		exit(0);
//	}
//
//}
//
//HuffmanTreeNode* loadTreeFromFile(map<char, int> &charWeightsMap)
//{
//	ifstream HuffmanTreeFile;
//	HuffmanTreeFile.open("HuffmanTree.txt");
//	if (HuffmanTreeFile.is_open())
//	{
//		string line;
//		vector<pair<char, int>> charWeightsVector;
//		while (getline(HuffmanTreeFile, line))
//		{
//			string source = line;
//			stringstream ss(source);
//			string str;
//
//			getline(ss, str, ' ');
//
//			if (!str.empty())
//			{
//				string ch = str;
//				getline(ss, str, ' ');
//				string weight = str;
//
//				charWeightsVector.push_back(make_pair(ch[0], stoi(weight)));
//
//				charWeightsMap[ch[0]] = stoi(weight);
//			}
//		}
//		HuffmanTreeFile.close();
//
//		HuffmanTreeNode* root = createHuffmanTree(charWeightsVector);
//
//		return root;
//	}
//	else
//	{
//		cerr << "Error：Unable to open HuffmanTreeFile.txt " << endl;
//		exit(0);
//	}
//}
//
//void getHuffmanCoding(HuffmanTreeNode* root, string path, map<char, string>& codingVector)
//{
//	if (root == nullptr)
//	{
//		return;
//	}
//	if (root->leftChild == nullptr && root->rightChild == nullptr)
//	{
//		codingVector[root->ch] = path;
//	}
//	else
//	{
//		getHuffmanCoding(root->leftChild, path + "0", codingVector);
//		getHuffmanCoding(root->rightChild, path + "1", codingVector);
//	}
//
//	return;
//}
//
//void saveTextToFile()
//{
//	ofstream TobetransFile;
//	TobetransFile.open("Tobetrans.txt");
//
//	if (TobetransFile.is_open())
//	{
//		cout << "请输入内容到文本中" << endl;
//		string text;
//		cin >> text;
//		TobetransFile << text;
//
//		TobetransFile.close();
//		return;
//	}
//	else
//	{
//		cerr << "Error：Unable to open Tobetrans.txt " << endl;
//		exit(0);
//	}
//}
//
//string loadTextFromFile()
//{
//	ifstream TobetransFile;
//	TobetransFile.open("Tobetrans.txt");
//
//	if (TobetransFile.is_open())
//	{
//		stringstream buffer;
//		buffer << TobetransFile.rdbuf();
//
//		/*
//		string line;
//		while (getline(TobetransFile, line))
//		{
//			buffer << line;
//		}
//		*/
//
//		string text = buffer.str();
//
//		TobetransFile.close();
//
//		return text;
//	}
//	else
//	{
//		cerr << "Error：Unable to open Tobetrans.txt " << endl;
//		exit(0);
//	}
//}
//
//void encode(const string& text, const map<char, string>& codes)
//{
//	string encodeText;
//	for (char ch : text)
//	{
//		auto it = codes.find(ch);
//		if (it != codes.end())
//		{
//			encodeText += it->second;
//		}
//		else
//		{
//			cerr << "Character '" << ch << "' not found in encoding map." << std::endl;
//			encodeText += "X"; //X为占位符编码
//		}
//	}
//
//	ofstream encodeFile;
//	encodeFile.open("Code.txt");
//	if (encodeFile.is_open())
//	{
//		encodeFile << encodeText;
//		encodeFile.close();
//	}
//	else
//	{
//		cerr << "Error：Unable to open Code.txt " << endl;
//		exit(0);
//	}
//
//	return;
//}
//
//void decode(map<char, string>codingVector)
//{
//	ifstream decodeFile;
//	decodeFile.open("Code.txt");
//
//	if (decodeFile.is_open())
//	{
//		stringstream buffer;
//		buffer << decodeFile.rdbuf();
//		string encodeText = buffer.str();
//
//		map<string, char> reverseCodingVector;
//		for (const auto& pair : codingVector)
//		{
//			reverseCodingVector[pair.second] = pair.first;
//		}
//
//		string decodeText;
//		string tempCode;
//		for (char ch : encodeText)
//		{
//			tempCode += ch;
//			if (reverseCodingVector.find(tempCode) != reverseCodingVector.end())
//			{
//				decodeText += reverseCodingVector[tempCode];
//				tempCode.clear();
//			}
//		}
//
//		ofstream textFile;
//		textFile.open("Text.txt");
//
//		if (textFile.is_open())
//		{
//			textFile << decodeText;
//			textFile.close();
//		}
//		else
//		{
//			cerr << "Error：Unable to open Text.txt " << endl;
//			exit(0);
//		}
//
//		decodeFile.close();
//
//		return;
//	}
//	else
//	{
//		cerr << "Error：Unable to open Code.txt " << endl;
//		exit(0);
//	}
//}
//
//string printCodeFile()
//{
//	ifstream decodeFile;
//	decodeFile.open("Code.txt");
//	if (decodeFile.is_open())
//	{
//		stringstream buffer;
//		buffer << decodeFile.rdbuf();
//		string encodeText = buffer.str();
//
//		ofstream codeprintFile;
//		codeprintFile.open("CodePrint.txt");
//		if (codeprintFile.is_open())
//		{
//			codeprintFile << encodeText;
//			codeprintFile.close();
//		}
//		else
//		{
//			cerr << "Error：Unable to open CodePrint.txt " << endl;
//			exit(0);
//		}
//
//		decodeFile.close();
//
//		return encodeText;
//	}
//	else
//	{
//		cerr << "Error：Unable to open Code.txt " << endl;
//		exit(0);
//	}
//}
//
//void printtime()
//{
//
//	/* 获取当前时间点
//	   std::chrono::system_clock::now()获取当前时间点
//	   std::chrono::system_clock是<chrono>头文件中定义的一个时钟类型
//	   now()是一个静态函数，返回表示当前时间点的std::chrono::system_clock::time_point类型的值。time_point是一个时间点
//	*/
//	auto now = std::chrono::system_clock::now();
//	/* 转换为time_t类型
//	   to_time_t 是一个静态成员函数，它将 std::chrono::system_clock::time_point 转换为 std::time_t 类型的值
//	   std::time_t 是一个在 <ctime> 头文件中定义的算术类型
//	*/
//	auto in_time_t = std::chrono::system_clock::to_time_t(now);
//
//	/* 转换为本地时间
//	   std::localtime 是一个在 <ctime> 头文件中定义的函数，它接受一个指向 std::time_t对象的指针，并返回一个指向 std::tm 结构的指针
//	   std::tm 是一个结构体，包含了分解的时间信息（如年、月、日、小时、分钟、秒等）
//
//	   localtime函数被认为是不安全的，因为它可能引发缓冲区溢出等安全问题
//	*/
//	std::tm* now_c = std::localtime(&in_time_t);
//
//	/* 使用put_time来格式化并显示时间
//	   std::put_time 是一个在 <iomanip> 头文件中定义的I/O操纵符，它接受一个指向 std::tm 对象的指针和一个格式字符串，并将格式化的时间输出到与之关联的流中。
//	   格式字符串 "%Y-%m-%d %H:%M:%S" 指定了输出的格式，其中 %Y、%m、%d 分别代表四位数的年份、月份、日期，%H、%M、%S 分别代表小时、分钟、秒。
//	*/
//
//	std::cout << "+------------------------------------------+" << std::endl;
//	std::cout << "|" << "      当前时间: " << std::put_time(now_c, "%Y-%m-%d %H:%M:%S") << "       " << "|" << std::endl;
//	std::cout << "+------------------------------------------+" << std::endl << std::endl;
//}
//
//
///*
//	#ifdef _WIN32   是一个预处理指令，用于检查 _WIN32 宏是否已定义。_WIN32 宏在 Windows 系统上由编译器自动定义。
//	如果 _WIN32 已定义，则编译器会编译std::system("cls") 在 Windows 上，这行代码调用 std::system 函数执行 cls 命令，该命令用于清除命令行窗口的屏幕。
//
//	#elif __linux__ 如果 _WIN32 未定义，则预处理器会检查 __linux__ 宏是否已定义。__linux__ 宏在 Linux 系统上由编译器自动定义。
//	如果 _WIN32 未定义且 __linux__ 已定义，则编译器会std::system("clear") 在 Linux 上，这行代码调用 std::system 函数执行 clear 命令，该命令用于清除终端的屏幕。
//*/
//void cls()
//{
//
//#ifdef _WIN32
//	std::system("cls");
//#elif  __linux__ 
//	std::system("clear");
//#endif
//
//}
//
//void RefreshScreenBuffer()
//{
//	cls();
//	printtime();
//}
//
//void printMenu()
//{
//	cout << "哈夫曼编译码系统：" << endl;
//	cout << "I：初始化哈夫曼树" << endl;
//	cout << "B：输入待编译正文" << endl;
//	cout << "C: 进行正文编码" << endl;
//	cout << "D：进行代码译码" << endl;
//	cout << "P：打印代码文件" << endl;
//	cout << "T：打印哈夫曼树" << endl;
//	cout << "E：退出系统" << endl;
//	cout << "请输入您的选择：";
//}
//
//void Menu()
//{
//	HuffmanTreeNode* root = nullptr;
//
//	while (1)
//	{
//		printMenu();
//		string choice;
//		cin >> choice;
//		RefreshScreenBuffer();
//
//		if (choice == "I")
//		{
//			cout << "请输入字符集大小：";
//
//			int n;
//			cin >> n;
//
//			RefreshScreenBuffer();
//
//			vector<pair<char, int>> charWeights(n);
//
//			cout << "请输入字符和所对应的权值：" << endl;
//
//			for (int i = 0; i < n; i++)
//			{
//				cin >> charWeights[i].first >> charWeights[i].second;
//			}
//
//			RefreshScreenBuffer();
//
//			root = createHuffmanTree(charWeights);
//
//			saveTreeToFile(root);
//
//			cout << "初始化哈夫曼树成功，是否需要打印哈夫曼树？（Yes/No）" << endl;
//			while (1)
//			{
//				string printChoice;
//				cin >> printChoice;
//				RefreshScreenBuffer();
//
//				if (printChoice == "YES" || printChoice == "yes" || printChoice == "Yes")
//				{
//					cout << "哈夫曼树如下：" << endl;
//					printHuffmanTree(root);
//					break;
//				}
//				else if (printChoice == "NO" || printChoice == "no" || printChoice == "No")
//				{
//					break;
//				}
//				else
//				{
//					cout << "输入错误，请重新输入：";
//				}
//			}
//
//			cout << endl;
//		}
//		else if (choice == "B")
//		{
//			saveTextToFile();
//			RefreshScreenBuffer();
//			cout << "输入成功" << endl << endl;
//		}
//		else if (choice == "C")
//		{
//			map<char, string>codingVector;
//			getHuffmanCoding(root, "", codingVector);
//
//			string uncodedText = loadTextFromFile();
//			encode(uncodedText, codingVector);
//
//			cout << "正文编码成功" << endl << endl;
//		}
//		else if (choice == "D")
//		{
//			map<char, string>codingVector;
//			getHuffmanCoding(root, "", codingVector);
//
//			decode(codingVector);
//
//			cout << "代码译码成功" << endl << endl;
//		}
//		else if (choice == "P")
//		{
//			cout << "文件codeFile的内容：" << printCodeFile() << endl << endl;
//		}
//		else if (choice == "T")
//		{
//			cout << "哈夫曼树如下：" << endl;
//			printHuffmanTree(root);
//
//			cout << endl;
//		}
//		else if (choice == "E")
//		{
//			cout << "退出系统成功" << endl;
//			break;
//		}
//		else
//		{
//			cout << "输入错误，请重新输入：" << endl << endl;
//		}
//	}
//}
//
//int main()
//{
//	RefreshScreenBuffer();
//	Menu();
//	return 0;
//}
