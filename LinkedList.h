#ifndef LinkedList_H
#define LinkedList_H

template <typename T>

class List
{
public:
	T val;

	List<T>* next;
};

template <typename T>

void deleteList(List<T>* head)
{
	List<T>* cur = head;

	List<T>* next = nullptr;

	while (cur != nullptr)
	{
		next = cur->next;
		delete cur;
		cur = next;
	}
}

#endif