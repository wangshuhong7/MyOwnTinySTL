#pragma once
#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_
#include<cstring>
#include<utility>
#include"allocator.h"
#include"functional.h"
#include"iterator.h"
#include"TypeTraits.h"
#include"utility.h"

namespace myTinySTL {
	template<class ForwardIterator,class T>
	void fill(ForwardIterator first, ForwardIterator last, const T&value) {
		for (; first != last; ++first) {
			*first = value;
		}
	}

	//重载，对非迭代器传入用memset处理
	inline void fill(char* first, char *last, const char& value) {
		memset(first, static_cast<unsigned char>(value), last - first);
	}

	inline void fill(wchar_t* first, wchar_t* last, const wchar_t& value) {
		memset(first, static_cast<unsigned char>(value), last - first);
	}

	template<class OutputIterator,class Size,class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
		for (; n > 0; n--,++first) {
			*first = value;//将连续n块区域赋予该值
		}
		return first;
	}

	//对非迭代器输入进行重载
	template<class Size>
	char* fill_n(char* first, Size n, const char& value) {
		memset(first, static_cast<unsigned char>(value), n);
		return first + n;
	}

	template<class Size>
	char* fill_n(char* first, Size n, const wchar_t& value) {
		memset(first, static_cast<unsigned char>(value), n);
		return first + n;
	}

	//取较小值
	template<class T>
	const T& min(const T& a,const T& b) {
		return a < b ? a : b;
	}

	template<class T, class Compare>
	const T& min(const T& a, const T& b, Compare comp) {
		return comp(a, b) ? a : b;
	}

	//取较大值
	template<class T>
	const T& max(const T& a, const T& b) {
		return a < b ? b : a;
	}

	template<class T,class Compare>
	const T& max(const T&a, const T&b, Compare comp) {
		return comp(a, b) ? b : a;
	}

	//堆排序上浮
	template<class RandomAccessIterator,class Compare>
	static void up(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator head, Compare comp) {
		if (first != last) {
			int index = last - head;
			auto parentIndex = (index - 1) / 2;
			for (auto cur = last; parentIndex >= 0 && cur != head; parentIndex = (index - 1) / 2) {
				auto parent = head + parentIndex;
				if (comp(*parent, *cur)) {
					myTinySTL::swap(*parent, *cur);
				}
				cur = parent;
				index = cur - head;
			}
		}
	}
	//堆排序下降
	template<class RandomAccessIterator,class Compare>
	static void down(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator head, Compare comp) {
		if (first != last) {
			int index = last - head;
			auto leftChildIndex = 1 + index * 2;
			for (auto cur = first; leftChildIndex < (last - head + 1) && cur < last; leftChildIndex = index * 2 + 1) {
				auto child = head + leftChildIndex;
				if ((child + 1) <= last && *(child + 1) > *(child))
					child += 1;
				if (comp(*cur, *child)) {
					myTinySTL::swap(*cur, *child);
				}
				cur = child;
				index = cur - head;
			}
		}
	}

	//建堆
	template<class RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
		myTinySTL::make_heap(first,last,
			typename myTinySTL::less<myTinySTL::iterator_traits<RandomAccessIterator>::value_type>())
	}//没有比较函数则默认调用less
	//有比较函数的建堆
	template<class RandomAccessIterator, class Compare>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		const auto range = last - first;
		for (auto cur = first + range / 2 + 1; cur >= first; --cur) {
			myTinySTL::down(cur, last - 1, first, comp);//逐一执行down
			if (cur == first)	return;
		}
	}

	//放入节点的函数：默认节点已经在数组最末端
	template<class RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
		myTinySTL::push_heap(first,last,
		typename myTinySTL::less<myTinySTL::iterator_traits< RandomAccessIterator>::value_type>());
	}

	template<class RandomAccessIterator,class Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		myTinySTL::up(first, last - 1, first, comp);
	}

	//移除堆顶的数（本质是移动到last位置使之无效化）
	template<class RandomAccessIterator>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
		myTinySTL::pop_heap(first, last,
			typename myTinySTL::less<myTinySTL::iterator_traits< RandomAccessIterator>::value_type>());
	}

	template<class RandomAccessIterator,class Compare>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		myTinySTL::swap(*first, *(last - 1));
		if (last - first >= 2) {
			myTinySTL::down(first, last - 2, first, comp);
		}
	}

	//堆排序
	template<class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
		myTinySTL::sort_heap(first, last,
			myTinySTL::less<typename myTinySTL::iterator_traits< RandomAccessIterator>::value_type>());
	}

	template<class RandomAccessIterator,class Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		for (auto cur = last; cur != first; --cur) {
			myTinySTL::pop_heap(first, cur, comp);
		}
	}

	//判断是不是堆
	template<class RandomAccessIterator>
	bool is_heap(RandomAccessIterator first, RandomAccessIterator last) {
		return myTinySTL::is_heap(first, last,
			myTinySTL::less<typename myTinySTL::iterator_traits< RandomAccessIterator>::value_type>());
	}

	template<class RandomAccessIterator,class Compare>
	bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		const auto range = last - first;
		auto index = range / 2 - 1;
		for (auto cur = first + range / 2 - 1; cur >= first; --cur, --index) {
			if (*(first + (index * 2 + 1)) > *cur ||//left child > cur
				((first + (index * 2 + 2)) <= last && *(first + (index * 2 + 2)) > *cur))//right child > cur
				return false;
			if (cur == first)
				break;
		}
		return true;
	}

	template<class InputIterator,class UnaryPredicate>
	bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
		for (; first != last; first++) {
			if (pred(*first))
				return true;
		}
		return false;
	}

	template<class InputIterator,class UnaryPredicate>
	bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
		for (; first != last; ++first) {
			if (!pred(*first))
				return false;
		}
		return true;
	}

	template<class InputIterator, class UnaryPredicate>
	bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
		for (; first != last; first++) {
			if (pred(*first))
				return false;
		}
		return true;
	}

	template<class InputIterator,class Function>
	Function for_each(InputIterator first, InputIterator last, Function fn) {
		for (; first != last; first++)
			fn(*first);
		return fn;
	}

	template<class InputIterator,class T>
	InputIterator find(InputIterator first, InputIterator last, const T& val) {
		for (; first != last; first++) {
			if (*first == val) {
				break;
			}
		}
		return first;
	}

	template<class InputIterator, class UnaryPredicate>
	InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred) {
		for (; first != last; first++) {
			if (pred(*first)) {
				break;
			}
		}
		return first;
	}

	template<class InputIterator, class UnaryPredicate>
	InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPredicate pred) {
		for (; first != last; first++) {
			if (!pred(*first)) {
				break;
			}
		}
		return first;
	}

	template<class ForwardIterator1,class ForwardIterator2>
	ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2) {//在序列1中找序列2的最后一次出现点
		if (first2 == last2)
			return last1;
		ForwardIterator1 ret = last1;
		while (first1 != last1) {
			ForwardIterator1 it1 = first1;
			ForwardIterator2 it2 = first2;
			while (*it1 == *it2) {
				++it1;
				++it2;
				if (it2 == last2) {
					ret = first1;
					break;
				}
				if (it1 == last1)	return ret;
			}
			++first1;
		}
		return ret;
	}

	template<class ForwardIterator1,class ForwardIterator2,class BinaryPredicate>
	ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2,
		ForwardIterator2 last2, BinaryPredicate pred) {
		if (first2 == last2)	return last1;
		ForwardIterator1 ret = last1;
		while (first1 != last1) {
			ForwardIterator1 it1 = first1;
			ForwardIterator2 it2 = first2;
			while (pred(*it1, *it2)) {
				++it1; ++it2;
				if (it1 == last1)	return ret;
				if (it2 == last2) {
					ret = first1;
					break;
				}
			}
			++first1;
		}
		return ret;
	}

	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2) {//以序列2内的元素作为查找目标，寻找他们在序列1中第一次出现的地点
		for (; first1 != last1; ++first1) {
			for (auto it = first2; it != last2; ++it) {
				if (*first1 == *it)
					return first1;
			}
		}
		return last1;
	}

	template<class ForwardIterator1,class ForwardIterator2,class BinaryPredicate>
	ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
		ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate pred) {
		for (; first1 != last1; ++first1) {
			for (auto it = first2; it != last2; it++) {
				if (pred(*first1 == *it)) {
					return first1;
				}
			}
		}
		return last1;
	}

	template<class ForwardIterator>
	//找出第一组满足条件的相邻元素的位置
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
		return myTinySTL::adjacent_find(first, last,
			equal_to<iterator_traits<typename ForwardIterator>::value_type>());
	}

	template<class ForwardIterator,class BinaryPredicate>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,BinaryPredicate) {
		for (; first != last; ++first) {
			if (first + 1 != last && pred(*first, *(first + 1))) {
				break;
			}
		}
		return first;
	}

	template<class InputIterator, class T>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const T& val) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; first++) {
			if (*first == val) {
				++n;
			}
		}
		return n;
	}

	template<class InputIterator,class UnaryPredicate>
	typename iterator_traits<InputIterator>::difference_type 
		count_if(InputIterator first, InputIterator last, UnaryPredicate pred) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; first++) {
			if (pred(*first))	++n;
		}
		return n;
	}

	template<class InputIterator1,class InputIterator2>
	pair<InputIterator1,InputIterator2>
		mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
		//用来平行比较两个序列，指出两者之间的第一个不匹配点。返回一对迭代器，分别指向两序列中不匹配点
		for (; first1 != last1; ++first1, ++first2) {
			if (*first1 != *first2)	break;
		}
		return make_pair(first1, first2);
	}

	template<class InputIterator1,class InputIterator2,class BinaryPredicate>
	pair<InputIterator1,InputIterator2>
		mismatch(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, BinaryPredicate pred) {
		for (; first1 != last1; ++first1, ++first2) {
			if (!pred(*first1, *first2))	break;
		}
		return make_pair(first1, first2);
	}
}
#endif // !_ALGORITHM_H_
