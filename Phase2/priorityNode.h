

template < typename T>
class priorityNode
{
private :
	T item; // A data item
	priorityNode<T>* next; // Pointer to next node
	int prioritypram=1; //paraemeter for priority queue
public :

	priorityNode();
	priorityNode(const T& r_Item);
	priorityNode( const T & r_Item,int t);
	priorityNode( const T & r_Item, priorityNode<T>* nextNodePtr);
	void setItem( const T & r_Item);
	void setNext(priorityNode<T>* nextNodePtr);
	void setpparam(const int pparam);
	T getItem() const ;
	priorityNode<T>* getNext() const ;
	int getpparam() const;
}; // end Node



template < typename T>
priorityNode<T>::priorityNode()
{
	next = nullptr;
} 
template < typename T>
priorityNode<T>::priorityNode(const T& r_Item)
{
	item = r_Item;
	next = nullptr;
}

template < typename T>
priorityNode<T>::priorityNode( const T& r_Item, int t)
{
	item = r_Item;
	next = nullptr;
	prioritypram = t;
} 

template < typename T>
priorityNode<T>::priorityNode( const T& r_Item, priorityNode<T>* nextNodePtr)
{
	item = r_Item;
	next = nextNodePtr;
}
template <typename T>
 void priorityNode<T>::setItem(const T& r_Item)
{
	item = r_Item;
} 
 template <typename T>
 void priorityNode<T>::setNext(priorityNode<T>* nextNodePtr)
{
	next = nextNodePtr;
} 
 template <typename T>
 void priorityNode<T>::setpparam(const int pparam)
{
	prioritypram = pparam;
}

template < typename T>
T priorityNode<T>::getItem() const
{
	return item;
} 

template < typename T>
priorityNode<T>* priorityNode<T>::getNext() const
{
	return next;
} 

template <typename T>
int priorityNode<T>::getpparam() const
{
	return prioritypram;
}




