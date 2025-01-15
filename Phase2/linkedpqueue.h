#pragma once
//#ifndef PLINKED_QUEUE_
//#define PLINKED_QUEUE_
#include "priorityNode.h"
#include <vector>
using namespace std;
	
template <typename T>

class linkedpqueue 
{
private:

	priorityNode<T>* backPtr;
	priorityNode<T>* frontPtr;
public:
	linkedpqueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry, int pparam);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	void printList();
	int listCount();
	~linkedpqueue();

	//copy constructor
	linkedpqueue(const linkedpqueue<T> & LPQ);
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
linkedpqueue<T>::linkedpqueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool linkedpqueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.
Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool linkedpqueue<T>::enqueue(const T& newEntry,int pparam )
{
	priorityNode<T>* newNodePtr = new priorityNode<T>(newEntry,pparam);
	if (isEmpty()) {
		frontPtr = newNodePtr;
		return true;
	}
	else if (newNodePtr->getpparam() < frontPtr->getpparam()) {
		newNodePtr->setNext(frontPtr);
		frontPtr = newNodePtr;
	}
		priorityNode<T>* previous = frontPtr;
		priorityNode<T>* tmp = frontPtr;
		while (tmp != NULL && newNodePtr->getpparam() >= tmp->getpparam()) //traverses on the linked list
		{
			previous = tmp;
			tmp = tmp->getNext();
		}
		previous->setNext(newNodePtr);
		newNodePtr->setNext(tmp);
	}
 // end enqueue


  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  /*Function: dequeue
  Removes the front of this queue. That is, removes the item that was added
  earliest.

  Input: None.
  Output: True if the operation is successful; otherwise false.
  */

template <typename T>
bool linkedpqueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	priorityNode<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;

	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: peek
copies the front of this queue to the passed param. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
*/
template <typename T>
bool linkedpqueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
int linkedpqueue<T>::listCount() {
	int counter = 0;
	priorityNode<T>* temp = frontPtr;
	while (temp) {
		counter++;
		temp = temp->getNext();
	}
	return counter;
}
template <typename T>
void linkedpqueue<T>::printList() {
	priorityNode<T>* temp = frontPtr;
	while (temp) {
		cout << temp->getItem() << " ,";
		temp = temp->getNext();
	}
	cout << '\b';
	cout << " " << endl;
}

/*
Function: destructor
removes all nodes from the queue by dequeuing them
*/
template <typename T>
linkedpqueue<T>::~linkedpqueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: Copy constructor
To avoid shallow copy,
copy constructor is provided

Input: LinkedQueue<T>: The Queue to be copied
Output: none
*/

template <typename T>
linkedpqueue<T>::linkedpqueue(const linkedpqueue<T>& LQ)
{
	priorityNode<T>* NodePtr = LQ.frontPtr;
	if (!NodePtr) //LQ is empty
	{
		frontPtr = backPtr = nullptr;
		return;
	}

	//insert the first node
	priorityNode<T>* ptr = new priorityNode<T>(NodePtr->getItem());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		priorityNode<T>* ptr = new priorityNode<T>(NodePtr->getItem());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
	}

};

