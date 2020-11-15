#pragma once
#ifndef _UNINITIALIZED_FUNCTIONS_H_
#define _UNINITIALIZED_FUNCTIONS_H_
#include"algorithm.h"
#include"construct.h"
#include"iterator.h"
#include"TypeTraits.h"

namespace myTinySTL {
	/*template<class InputIterator,class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _true_type);
	template<class InputIterator,class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _false_type);*/

	//是否是POD类型采取方式不同
	template<class InputIterator,class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, InputIterator result) {
		typedef typename _type_traits<iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
		return _uninitialized_copy_aux(first, last, result, isPODType());
	}

	template<class InputIterator,class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _true_type) {
		memcpy(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}

	template<class InputIterator ,class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _false_type) {
		ForwardIterator cur = result;
		for (; first != last; ++first, ++cur) {
			construct(&*cur, *first);
		}
		return cur;
	}

	//对空白区域传参填入
	/*template<class ForwardIterator,class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& value, _true_type);
	template<class ForwardIterator,class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& calue, _false_type);*/

	template<class ForwardIterator,class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
		typedef typename _type_traits<T>::is_POD_type isPODType;
		_uninitialized_fill_aux(first, last, value, isPODType());
	}

	template<class ForwardIterator,class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& value, _true_type) {
		fill(first, last, value);
	} 

	template<class ForwardIterator,class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& value, _false_type) {
		for (; first != last; first++) {
			construct(first, value);
		}
	}

	//对局部区域赋值
	/*template<class ForwardIterator,class Size,class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& value, _true_type);
	template<class ForwardIterator,class Size,class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,Size n,
		const T& value, _false_type);*/

	template<class ForwardIterator,class Size,class T>
	inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x) {
		typedef typename _type_traits<T>::is_POD_Type isPODType;
		return _uninitialized_fill_n_aux(first, n, x, isPODType());
	}

	template<class ForwardIterator,class Size,class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& value, _true_type) {
		return fill_n(first, n, x);
	}

	template<class ForwardIterator,class Size,class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T& value, _false_type) {
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur) {
			construct(&*cur, value);
		}
		return cur;
	}
}
#endif // !_UNINITIALIZED_FUNCTIONS_H_
