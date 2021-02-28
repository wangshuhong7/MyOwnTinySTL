#pragma once
#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_
#include<new>
#include"TypeTraits.h"
namespace myTinySTL {
	template<class T1,class T2>
	inline void construct(T1* p, const T2& value) {
		new(p)T1(value);//用new来给对应地址分配空间
	}

	template<class T>
	inline void destroy(T* ptr) {//第一个版本，接受一个指针
		ptr->~T();
	}

	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		//第二版本，接受两个指针,此函数设法找出元素的数值行别，进而利用
		//_type_traits<>求取最适当措施
		__destroy(first, last, value_type(first));
	}

	//判断元素的数值型别是否有trivial destructor
	template<class ForwardIterator,class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*) {
		typedef typename _type_traits<T>::has_trivial_destructr trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}

	//如果元素的数值型别有non_trivial destructor
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, _false_type) {
		for (; first < last; ++first) {
			destroy(&*first);
		}
	}

	//如果元素的数值型别有trivial destructor
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator lase, _true_type) {}

	/*template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		typedef typename _type_traits<T>::is_POD_type is_POD_type;
		_destroy(first, last, is_POD_type);
	}*/
	
}
#endif // !_CONSTRUCT_H_

