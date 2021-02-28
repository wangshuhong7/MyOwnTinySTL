#ifndef __LIST_H
#define __LIST_H
#include <stddef.h>
#include "allocator.h"//require "TinySTL_alloc.cc"
#include "construct.h"

namespace myTinySTL
{

	template<typename T>
	struct node
	{
		node<T>* prev;
		node<T>* next;
		T data;
		node(const T& val = T()) :prev(0), next(0), data(val) {}
		node(const T& val, node<T>* prev_, node<T>* next_) :prev(prev_), next(next_), data(val) {}
		bool operator==(const node<T>& nod)
		{
			return prev == nod.prev && next == nod.next && data == nod.data;
		}

		bool operator!=(const node<T>& nod)
		{
			return prev != nod.prev || next != nod.next || data != nod.data;
		}

	};


	template<typename T>
	struct listIterator
	{
		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;

		typedef node<T>* nodePtr;
		typedef listIterator<T> self;

		nodePtr p;

		listIterator(const nodePtr& _p = 0) :p(_p) {}
		bool operator==(const listIterator<T>& iter)
		{
			return p == iter.p;
		}
		bool operator!=(const listIterator<T>& iter)
		{
			return p != iter.p;
		}
		reference operator*()const
		{
			return (*p).data;
		}
		pointer operator->()const
		{
			return &((*p).data);
		}

		self& operator++()
		{
			p = (nodePtr)(p->next);
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			p = (nodePtr)(p->next);
			return tmp;
		}

		self& operator--()
		{
			p = (nodePtr)(p->prev);
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			p = (nodePtr)(p->prev);
			return tmp;
		}

	};


	template<typename T, typename Alloc = alloc>
	class list
	{
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef T& reference;


		typedef node<T>* nodePtr;
		typedef listIterator<T> iterator;
		typedef listIterator<const T> const_iterator;

		typedef simple_alloc<node<T>, Alloc> list_node_allocator;
	protected:
		iterator head;
		iterator tail;
	protected:
		//请求allocator分配内存并构造
		nodePtr newNode(const T& value = T());
		//delete node
		void deleteNode(nodePtr p);


	public: //Iterator
		iterator begin() { return head; }
		iterator end() { return tail; }


	public: //construct and destroy
		list();
		//list(size_type n,const T& value);
		~list();
	public:  // about capacity
		bool empty() { return begin() == end(); }
		//size_type size();

	public:  //element access
		reference front()
		{

			return *begin();
		}
		reference back()
		{
			return (tail.p->prev->data);
		}
	public:
		void push_front(const T& value);
		void pop_front();
		void push_back(const T& value);
		void pop_back();

	protected: //insert
		template<typename InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last);

	public:
		iterator insert(iterator position, const T& value);
		template<typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
	public: //erase
		iterator erase(iterator position);

	protected:
		//并不处理[first,last)先后节点的关系，只适合list.begin(),list.end()调用，请小心食用
		void transfer(iterator position, iterator first, iterator last);

	public:
		void splice(iterator position, list<T, Alloc>& other);


	};


	template<typename T, typename Alloc>
	typename list<T, Alloc>::nodePtr list<T, Alloc>::newNode(const T& value)
	{
		nodePtr ptr = list_node_allocator::allocate();
		construct(ptr, node<T>(value));
		return ptr;
	}
	template<typename T, typename Alloc>
	void list<T, Alloc>::deleteNode(nodePtr p)
	{
		p->prev = p->next = 0;
		destroy(p);
		list_node_allocator::deallocate(p);
	}

	template<typename T, typename Alloc>
	list<T, Alloc>::list()
	{
		head.p = newNode();
		tail.p = head.p;
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::push_front(const T& value)
	{
		nodePtr newnode = newNode(value);
		newnode->next = head.p;
		head.p->prev = newnode;
		head.p = newnode;
	}
	template<typename T, typename Alloc>
	void list<T, Alloc>::pop_front()
	{
		nodePtr nextnode = head.p->next;
		nextnode->prev = 0;
		nodePtr tmp = head.p;
		head.p = nextnode;
		deleteNode(tmp);
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::push_back(const T& value)
	{
		nodePtr newnode = newNode();
		newnode->prev = tail.p;
		tail.p->next = newnode;
		tail.p->data = value;
		tail.p = newnode;
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::pop_back()
	{
		nodePtr prevnode = tail.p->prev;
		prevnode->next = 0;
		nodePtr tmp = tail.p;
		tail.p = prevnode;
		deleteNode(tmp);
	}
	template<typename T, typename Alloc>
	list<T, Alloc>::~list()
	{

		for (; head != tail;)
		{
			nodePtr tmpnode = head.p;
			++head;
			deleteNode(tmpnode);

		}
		deleteNode(tail.p);

	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	void list<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last)
	{
		for (--last; first != last; --last)
		{
			position = insert(position, *first);
		}
		insert(position, *last);
	}


	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator position, const T& value)
	{
		if (position == begin())
		{
			push_front(value);
			return head;
		}
		else if (position == end())
		{
			//fixme：有点bug 不是返回tail
			iterator ret = position;
			push_back(value);
			return ret;
		}
		else
		{
			nodePtr newnode = newNode(value);
			nodePtr prevnode = position.p->prev;
			prevnode->next = newnode;
			newnode->prev = prevnode;
			newnode->next = position.p;
			position.p->prev = newnode;
			position.p = newnode;
			return position;
		}
	}
	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator position)
	{
		if (position == begin())
		{
			pop_front();
			return begin();
		}
		else if (position == end())
		{
			//实际应该弹出异常,这里直接返回end()，不做改动
			return end();
		}
		else
		{
			nodePtr prevNode = position.p->prev;
			nodePtr nextNode = position.p->next;
			prevNode->next = nextNode;
			nextNode->prev = prevNode;
			deleteNode(position.p);
			return iterator(nextNode);
		}
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	void list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last)
	{
		insert_aux(position, first, last);
	}


	template<typename T, typename Alloc>
	void list<T, Alloc>::transfer(iterator position, iterator first, iterator last)
	{
		if (first == last)
			return;

		if (position == begin())
		{
			nodePtr old_head = position.p;
			nodePtr lastPrev = last.p->prev;
			first.p->prev = 0;
			head.p = first.p;
			old_head->prev = lastPrev;
			lastPrev->next = old_head;
		}
		else
		{
			nodePtr positionPrev = position.p->prev;
			nodePtr lastPrev = last.p->prev;
			positionPrev->next = first.p;
			first.p->prev = positionPrev;
			position.p->prev = lastPrev;
			lastPrev->next = position.p;
		}
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::splice(iterator position, list<T, Alloc>& other)
	{
		transfer(position, other.begin(), other.end());
		other.head = other.tail;
		nodePtr dummyNode = tail.p;
		dummyNode->prev = dummyNode->next = 0;

	}

}
#endif