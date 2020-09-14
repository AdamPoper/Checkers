#pragma once
#include <iostream>
#include <assert.h>

namespace ap
{
	template<typename type> class QueueLinkedList;
	// Node
	template<typename type>
	class Node
	{
	private:
		// default
		Node();
		// other
		Node(type&, Node*);
		// get the data
		type& getData() const { return data; }
	private:
		Node* next;
		type data;
		int num;
		friend class QueueLinkedList<type>;
	};
	// - Node Implementations
	template<typename type>
	Node<type>::Node()
	{
		next = nullptr;
		data = type();
	}
	template<typename type>
	Node<type>::Node(type& data, Node* ptr) : data(data)
	{
		//this->data = data;
		next = ptr;
		num = 0;
	}
	// Class Declaration for Linked List
	template<typename type>
	class QueueLinkedList
	{
	public:
		// default
		QueueLinkedList();
		// copy
		//QueueLinkedList(const QueueLinkedList<type>&);
		// add an element to the queue
		void enQueue(type&);
		// get number of elements
		unsigned int Count() const;
		// dequeue the queue
		void deQueue();
		// remove a random element
		bool remove(int);
		// [] overload
		type& operator[](int);
		// const [] overload
		type& operator[](int) const;
		// display data
		void display();
		// update the internal number system
		void updateNums();

	private:
		Node<type>* first;
		unsigned int count;
	};
	// - QueueLinkedList implementations
	template<typename type>
	QueueLinkedList<type>::QueueLinkedList()
	{
		count = 0;
		first = nullptr;
	}
	template<typename type>
	void QueueLinkedList<type>::enQueue(type& data)
	{
		if (first == nullptr)
		{
			first = new Node<type>(data, first);
			first->num = count;
		}
		else
		{
			Node<type>* iter = first;
			while (iter->next != nullptr)
			{
				iter = iter->next;
			}
			iter->next = new Node<type>(data, iter->next);
			iter->next->num = count;
		}
		count++;
	}
	template<typename type>
	unsigned int QueueLinkedList<type>::Count() const
	{
		return count;
	}
	template<typename type>
	void QueueLinkedList<type>::deQueue()
	{
		Node<type>* doomed = first;
		first = first->next;
		delete doomed;
		count--;
		Node<type>* iter = first;
		int c = 0;
		while (iter/*->next*/ != nullptr)
		{
			iter->num = c;
			iter = iter->next;
			c++;
		}
	}
	template<typename type>
	type& QueueLinkedList<type>::operator[](int index)
	{
		Node<type>* iter = first;
		while (iter != nullptr)
		{
			if (iter->num == index)
			{
				return iter->data;
			}
			iter = iter->next;
		}
	}
	template<typename type>
	type& QueueLinkedList<type>::operator[](int index) const
	{
		Node<type>* iter = first;
		while (iter != nullptr)
		{
			if (iter->num == index)
			{
				return iter->data;
			}
			iter = iter->next;
		}
	}
	template<typename type>
	bool QueueLinkedList<type>::remove(int index)
	{
		//std::cout << "in remove" << std::endl;
		if (first->num == index)
		{
			//std::cout << "its was the first" << std::endl;
			deQueue();
		}
		else
		{
			//std::cout << "else" << std::endl;
			if (first->next->num == index)
			{
				//std::cout << "was the next one after the first one" << std::endl;
				Node<type>* doomed = first->next;
				first->next = first->next->next;
				delete doomed;
				count--;
				updateNums();
				return true;
			}
			else
			{
				//std::cout << "Other else" << std::endl;
				Node<type>* iter = first;
				while (iter->next != nullptr)
				{
					if (iter->next->num == index)
					{
						//std::cout << "found correct index" << std::endl;
						//std::cout << "iter->next->num: " << iter->next->num << std::endl;
						Node<type>* doomed = iter->next;
						iter->next = iter->next->next;
						delete doomed;
						//std::cout << "doomed deleted" << std::endl;
						count--;
						updateNums();
						return true;
					}
					//std::cout << "updating iter" << std::endl;
					iter = iter->next;
				}
				//std::cout << "out of while" << std::endl;
			}
		}
		return false;
	}
	template<typename type>
	void QueueLinkedList<type>::display()
	{
		Node<type>* iter = first;
		while (iter != nullptr)
		{
			std::cout << iter->data << std::endl;
			iter = iter->next;
		}
	}
	template<typename type>
	void QueueLinkedList<type>::updateNums()
	{
		//std::cout << "updating the nums" << std::endl;
		Node<type>* iter = first;
		int c = 0;
		while (iter != nullptr)
		{
			iter->num = c;
			//std::cout << "iter->num: " << iter->num << std::endl;
			c++;
			iter = iter->next;
		}
	}
}
