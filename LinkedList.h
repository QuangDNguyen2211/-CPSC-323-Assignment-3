/*
	Members: Vinh Tran
			 Quang Nguyen
	Course: CPSC 323 - 04
	Professor: Anthony Le
	Assignment 3: Syntax Analyzer
*/

// Implement the class named "Node"
template<class T>
class Node
{
public:
	Node(void) {}
	Node(const T& _data1, const T& _data2, Node* _next)
		: data1(_data1), data2(_data2), next(_next)
	{};
	T data1;
	T data2;
	Node* next;
};

// Implement the class named "LinkedList"
template<class T>
class LinkedList
{
public:
	// Default Constructor
	LinkedList(void);

	// Non-default Constructor
	LinkedList(const LinkedList<T>& source);
	LinkedList(const LinkedList<T>& leftSource, const LinkedList<T>& rightSource);

	// Destructor
	~LinkedList();

	// Operator Overloading
	LinkedList<T>& operator=(const LinkedList<T>& source);
	friend LinkedList<T> operator+(const LinkedList& leftSource, const LinkedList& rightSource)
	{
		return LinkedList<T>(leftSource, rightSource);
	}

	// Getters
	T showToken(void);
	T showLexeme(void);

	// Methods
	void push_back(const T& item1, const T& item2);
	bool isEmpty(void);
	void pop_front(void);
	void clear();

private:
	Node<T>* list;
	void copy(const LinkedList<T>& source);
};

// Implement the default constructor
template<class T>
LinkedList<T>::LinkedList(void)
	: list(nullptr)
{}

// Implement the non-default constructor
template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& source)
	: list(nullptr)
{
	copy(source);
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& leftSource, const LinkedList<T>& rightSource)
	: list(nullptr)
{
	Node<T>* tempList = nullptr;

	for (Node<T>* ptr = leftSource.list; ptr != nullptr; ptr = ptr->next)
		push_back(ptr->data1, ptr->data2);
	for (Node<T>* ptr = rightSource.list; ptr != nullptr; ptr = ptr->next)
		push_back(ptr->data1, ptr->data2);
}

// Implement the destructor
template<class T>
LinkedList<T>::~LinkedList()
{
	clear();
}

// Implement the operator overloading
template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& source)
{
	copy(source);
	return *this;
}

// Implement the getters
template<class T>
T LinkedList<T>::showToken(void)
{
	if (list == nullptr)
		return nullptr;
	else
		return (list->data1);
}

template<class T>
T LinkedList<T>::showLexeme(void)
{
	if (list == nullptr)
		return nullptr;
	else
		return (list->data2);
}

// Implement the member functions or methods
template<class T>
void LinkedList<T>::push_back(const T& item1, const T& item2)
{
	if (list == nullptr)
		list = new Node<T>(item1, item2, nullptr);
	else
	{
		for (Node<T>* previousPtr = list; true; previousPtr = previousPtr->next)
		{
			if (previousPtr->next == nullptr)
			{
				previousPtr->next = new Node<T>(item1, item2, nullptr);
				break;
			}
		}
	}
}

template<class T>
bool LinkedList<T>::isEmpty(void)
{
	return list == nullptr;
}

template<class T>
void LinkedList<T>::pop_front(void)
{
	Node<T>* junk = list;
	list = list->next;
	delete junk;
}

template<class T>
void LinkedList<T>::clear()
{
	while (list != nullptr)
		pop_front();
}

template<class T>
void LinkedList<T>::copy(const LinkedList<T>& source)
{
	clear();
	for (Node<T>* np = source.list; np != nullptr; np = np->next)
		push_back(np->data1, np->data2);
}

