#pragma once
#ifndef _ITERATOR_H_
#define _ITERATOR_H_
#include<stddef.h>
namespace myTinySTL {
	//五种迭代器类型
	struct input_iterator_tag {};
	struct output_iterator_tag{};
	struct forward_iterator_tag:public input_iterator_tag{};
	struct bidirectional_iterator_tag:public forward_iterator_tag{};
	struct random_access_iterator_tag:public bidirectional_iterator_tag{};

	template<class T, class Distance>
	struct input_iterator {
		typedef input_iterator_tag iterator_category;//迭代器本身
		typedef T value_type;//定义类型
		typedef Distance difference_type;//大小
		typedef T* pointer;//定义指针
		typedef T& reference;//定义引用
	};

	struct output_iterator {
		typedef output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;
	};

	template<class T, class Distance>
	struct forward_iterator {
		typedef forward_iterator_tag  iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template<class T,class Distance>
	struct bidirectional_iterator {
		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef Distance  difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template<class T, class Distance>
	struct random_access__iterator {
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef Distance  difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	//以防万一代码挂了，自行开发的迭代器最好集成下面这个std::iterator
	template<class Category,class T,class Distance = ptrdiff_t,
		class Pointer = T*,class Reference = T&>
		struct iterator {
		typedef Category iterator_category;
		typedef T value_type;
		typedef T* pointer;
		typedef Distance difference_type;
		typedef T& reference;
	};

	//榨汁机-traits
	template<class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
	};

	//针对原生指针（native pointer)而设计的traits偏特化版
	template<class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag iteratory_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template<class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag iteratory_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	//以下三个函数均使用强制转型，提取iterator中的元素
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator& It) {
		typedef typename iterator_traits<Iterator>::iteratory_category category;
		return category();
	}

	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator& It) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		difference_type(const Iterator& It) {
		return static_cast<typename iterator_traits<Iterator>::difference_typr*>(0);
	}
}
#endif // !_ITERATOR_H_
