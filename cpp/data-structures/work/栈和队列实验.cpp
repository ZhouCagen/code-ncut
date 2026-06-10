/*

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdexcept>	// 包含标准异常类
#include <string>

using namespace std;

#define MAXSIZE 0xffff

template <class type>
class my_stack
{
private:
	int top;
	int maxsize;
	type* my_s;
public:
	my_stack() :top(-1), maxsize(MAXSIZE)
	{
		my_s = new type[maxsize];
		if (my_s == NULL)
		{
			cout << "动态存储分配失败！" << endl;
			exit(1);
		}
	}

	my_stack(int size) :top(-1), maxsize(size)
	{
		my_s = new type[maxsize];
		if (my_s == NULL)
		{
			cout << "动态存储分配失败！" << endl;
			exit(1);
		}
	}

	bool empty();
	void push(type tp);
	type Top();
	void pop();
	int size();
};

template <class type>
// 判断栈是否为空
bool my_stack<type>::empty()
{
	if (top == -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<class type>

type my_stack<type>::Top() // 返回栈顶元素
{
	if (top != -1) // 如果栈不为空
	{
		return my_s[top]; // 返回栈顶元素
	}
	else
	{
		cout << "栈空" << endl;
		exit(1);
	}
}

template<class type>
void my_stack<type>::push(type tp) // 压栈操作
{
	if (top + 1 < maxsize)  // 如果栈未满
	{
		my_s[++top] = tp; // 先将栈顶指针加1，然后将新元素放入栈顶
	}
	else
	{
		cout << "栈满" << endl;
		exit(1);
	}
}

template<class type>
void my_stack<type>::pop()// 出栈操作
{
	if (top >= 0) // 如果栈不为空
	{
		top--; // 将栈顶指针减1，实现出栈
	}
	else
	{
		cout << "栈空" << endl;
		exit(1);
	}
}

template<class type>
int my_stack<type>::size() // 返回栈中元素的数量
{
	return top + 1; // 栈顶指针加1即为栈中元素的数量
}


template<class T>
class my_queue
{
private:
	int front;
	int rear;
	int maxsize;
	T* value;
public:
	my_queue() :front(0), rear(-1), maxsize(MAXSIZE)
	{
		value = new T[maxsize];
		if (value == nullptr)
		{
			cout << "动态存储分配失败！" << endl;
			exit(1);
		}
	}
	my_queue(int size) :front(0), rear(-1), maxsize(size)
	{
		value = new T[size];
		if (value == nullptr)
		{
			cout << "动态存储分配失败！" << endl;
			exit(1);
		}
	}

	bool empty();
	int size();
	T head();
	T back();
	void push(T x);
	void pop();
};

template<class T>
bool my_queue<T>::empty()
{
	return rear + 1 == front ? true : false;
}

template<class T>
int my_queue<T>::size()
{
	return rear - front + 1;
}

template<class T>
T my_queue<T>::head()
{
	if (!empty())
	{
		return value[front];
	}
	else
	{
		throw runtime_error("Queue is empty"); //运行时
	}
}

template<class T>
T my_queue<T>::back()
{
	if (!empty())
	{
		return value[rear];
	}
	else
	{
		throw runtime_error("Queue is empty"); //运行时
	}
}

template<class T>
void my_queue<T>::push(T x)
{
	value[++rear] = x;
}

template<class T>
void my_queue<T>::pop()
{
	if (!empty())
	{
		front++;
	}
	else
	{
		return;
	}
}

struct Car
{
	int id;
	int time;
};

class Parking
{
public:
	Parking(int parkingcarcapacity, double parkingcarfee) :parkingCarCapacity(parkingcarcapacity), parkingCarFee(parkingcarfee) {}
	void accessAndOutlet(int choice, int id, int time)
	{
		if (choice == 0)
		{
			Access(id, time);
		}
		else if (choice == 1)
		{
			Outlet(id, time);
		}
	}

	void printData()
	{
		printCarFee();
		printParkingLot();
		printSidewalk();
	}


private:
	int parkingCarCapacity;
	double parkingCarFee;
	my_stack<Car> parkingLot;
	my_queue<Car> sidewalk;
	vector<pair<int, double>> carFee;

	int carTotal = 0;

	void Access(int id, int time)
	{
		if (parkingLot.size() < parkingCarCapacity)
		{
			parkingLot.push({ id,time });
		}
		else
		{
			sidewalk.push({ id,time });
		}
		carTotal++;
	}
	void Outlet(int id, int time)
	{
		my_stack<Car> tempParkingLot;
		bool isFoundParkingLot = false;
		Car leavingCar;

		// 查找要离开的车辆，并保存到leavingCar中
		while (!parkingLot.empty())
		{
			Car tempCar = parkingLot.Top();
			parkingLot.pop();
			if (tempCar.id == id)
			{
				isFoundParkingLot = true;
				leavingCar = tempCar;
				int duration = time - tempCar.time;
				double fee = duration * parkingCarFee;
				carFee.push_back({ id, fee });
				break;
			}
			else
			{
				tempParkingLot.push(tempCar);
			}
		}

		// 将临时保存的停车场车辆放回原处
		while (!tempParkingLot.empty())
		{
			parkingLot.push(tempParkingLot.Top());
			tempParkingLot.pop();
		}

		if (isFoundParkingLot)
		{
			// 检查便道，如果有车辆且停车场有空位，则让最早到达的车辆进入停车场
			if (!sidewalk.empty() && parkingLot.size() < parkingCarCapacity)
			{
				parkingLot.push(sidewalk.head());
				cout << endl << "Car " << sidewalk.head().id << " moved from sidewalk to parking lot." << endl << endl;
				sidewalk.pop();

				// 如果便道上还有其他车辆，且停车场仍然有空位，则继续处理
				while (!sidewalk.empty() && parkingLot.size() < parkingCarCapacity)
				{
					parkingLot.push(sidewalk.head());
					cout << endl << "Car " << sidewalk.head().id << " moved from sidewalk to parking lot." << endl << endl;
					sidewalk.pop();
				}
			}
		}

		// 如果没有在停车场找到车辆，则检查便道
		else if (!isFoundParkingLot)
		{
			bool isFoundSidewalk = false;
			my_queue<Car> tempSidewalk;
			while (!sidewalk.empty())
			{
				Car tempCar = sidewalk.head();
				sidewalk.pop();
				if (tempCar.id == id)
				{
					isFoundSidewalk = true;
					cout << endl << "Car " << id << " was in sidewalk, no fee." << endl << endl;
					break;
				}
				else
				{
					tempSidewalk.push(tempCar);
				}
			}

			while (!tempSidewalk.empty())
			{
				sidewalk.push(tempSidewalk.head());
				tempSidewalk.pop();
			}

			if (!isFoundSidewalk)
			{
				cout << endl << "Car " << id << " was not in sidewalk and parking lot." << endl << endl;
			}
		}
	}

	void printCarFee()
	{
		cout << "Carfee：" << endl;
		for (auto fee : carFee)
		{
			cout << setw(4) << "" << "id：" << fee.first << setw(6) << "" << "fee：" << setiosflags(ios::fixed) << setprecision(2) << fee.second << endl;
		}
		cout << endl;
	}

	void printParkingLot()
	{
		vector<int> parkingLotCars;
		my_stack<Car>tempParkingLot = parkingLot;
		while (!tempParkingLot.empty())
		{
			parkingLotCars.push_back(tempParkingLot.Top().id);
			tempParkingLot.pop();
		}

		cout << "ParkingLot：" << endl;
		for (int i = static_cast<int>(parkingLotCars.size()) - 1; i >= 0; i--)
		{
			cout << parkingLotCars[i] << setw(2) << "";
		}
		cout << endl << endl;
	}

	void printSidewalk()
	{
		vector<int> sidewalkCars;
		my_queue<Car>tempSidewalk = sidewalk;
		while (!tempSidewalk.empty())
		{
			sidewalkCars.push_back(tempSidewalk.head().id);
			tempSidewalk.pop();
		}
		cout << "Sidewalk：" << endl;
		for (int i = 0; i < sidewalkCars.size(); i++)
		{
			cout << sidewalkCars[i] << setw(2) << "";
		}

		cout << endl;
	}
};


int main()
{
	int parkingCarNumber;
	double parkingCarFee;

	cin >> parkingCarNumber >> parkingCarFee;

	Parking parking(parkingCarNumber, parkingCarFee);

	//while (1)
	//{
	//	int choice, id, time;
	//	cin >> choice >> id >> time;

	//	if (choice == 0 && id == 0 && time == 0)
	//	{
	//		break;
	//	}

	//	parking.accessAndOutlet(choice, id, time);

	//}

	string line;
	while (getline(cin, line))
	{
		if (line == "0 0 0")break;
		stringstream ss(line);
		int choice, id, time;
		ss >> choice >> id >> time;
		parking.accessAndOutlet(choice, id, time);
	}

	cout << endl;

	parking.printData();

	return 0;
}

*/