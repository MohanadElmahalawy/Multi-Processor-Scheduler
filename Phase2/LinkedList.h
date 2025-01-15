#ifndef _LINKEDLIST
#define _LINKEDLIST
#pragma once
#include "Node.h"
#include <iostream>

template <typename T>
class LinkedList
{
private:
	Node<T> *Head;	//Pointer to the head of the list

	//You can add tail pointer too (depending on your problem)
public:


	LinkedList()
	{
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll(); 
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	void printList()	const
	{		
		Node<T> *p = Head;

		while(p)
		{
			cout << p->getItem() << " ,";
			p = p->getNext();
		}
		cout << '\b';
		cout << " ";
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	* 
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T &data)
	{
		Node<T> *R = new Node<T>(data);
		R->setNext(Head);
		Head = R;

	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T> *P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}



	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions

	T getFirst(T& data) {
		Node<T>* P = Head;
		data =P->getItem();
		return data;
	}
	//[1]InsertEnd 
	//inserts a new node at end if the list
	void InsertEnd(const T &data)
	{
		Node<T>* p = Head;
		if(!p)
		{
			Node<T>* N = new Node<T>(data); 
			Head = N;
			return;
		}
		while(p)
		{
			if(p->getNext() == NULL)
			{
			Node<T>* N = new Node<T>(data);
			p->setNext(N);
			break;
			}
			p = p->getNext();
		}
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(T Key)
	{
		Node<T>* p = Head;
		if(!p)
		{
			return false;
		}
		while(p)
		{
			if(*Key == p->getItem())
				return true;
			p = p->getNext();
		}
		return false;
	}

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int listCount()
	{
		Node<T>* p = Head;
		int count = 0;
		if(!p)
			return count;
		while (p)
		{
				count++;
			p = p->getNext();
		}
		return count;
	}

	//[4] DeleteFirst
	//Deletes the first node in the list
	void DeleteFirst()
	{
		Node<T>* p = Head;
		if(!p)
			return;
		Head = Head->getNext();
		delete p;
	}


	//[5] DeleteLast
	//Deletes the last node in the list
	void DeleteLast()
	{
		Node<T>* p = Head;
		if(p->getNext() ==NULL)
		{
			delete p;
			p=NULL;
			return;
		}
		while (p->getNext())
		{
			p = p->getNext();
		}
		delete p->getNext();
	}

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool Delete5ales(const T &value)
	{
		if (!listCount()) {
			return false;
	     }
		Node<T>* delptr=Head;

		if (Head->getItem() == value) {
			Head = Head->getNext();
			delete delptr;
			return true;
		}
		else
		{
			Node<T>* prev = NULL;
			delptr = Head;
			while (delptr!= NULL && delptr->getItem() != value) {
				prev = delptr;
				delptr = delptr->getNext();
			}
			if (delptr) {
				prev->setNext(delptr->getNext());
				delete delptr;
				return true;
			}
			else return false;
		}
	}

	T DeleteWReturn(T value) //returns null if not deleted, or object deleted if deleted
	{

		T fin=NULL;
		if (!listCount()) {
			return fin;
		}
		Node<T>* delptr = Head;

		if (*value==Head->getItem()) {
			Head = Head->getNext();
			fin = delptr->getItem();
			return fin;
		}
		else
		{
			Node<T>* prev = NULL;
			delptr = Head;
			while (delptr != NULL && !(*value==delptr->getItem())) {
				prev = delptr;
				delptr = delptr->getNext();
			}
			if (delptr) {
				prev->setNext(delptr->getNext());
				fin = delptr->getItem();
				return fin;
			}
			else return fin;
		}
	}
	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNodes(const T &value);	

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	void Merge(const LinkedList& L);

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse();
		
};

#endif	
