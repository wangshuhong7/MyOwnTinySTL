#pragma once
#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_
#include<new>
#include"TypeTraits.h"
namespace myTinySTL {
	template<class T1,class T2>
	inline void construct(T1* p, const T2& value) {
		new(p)T1(value);//��new������Ӧ��ַ����ռ�
	}

	template<class T>
	inline void destroy(T* ptr) {//��һ���汾������һ��ָ��
		ptr->~T();
	}

	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		//�ڶ��汾����������ָ��,�˺����跨�ҳ�Ԫ�ص���ֵ�б𣬽�������
		//_type_traits<>��ȡ���ʵ���ʩ
		__destroy(first, last, value_type(first));
	}

	//�ж�Ԫ�ص���ֵ�ͱ��Ƿ���trivial destructor
	template<class ForwardIterator,class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*) {
		typedef typename _type_traits<T>::has_trivial_destructr trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}

	//���Ԫ�ص���ֵ�ͱ���non_trivial destructor
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, _false_type) {
		for (; first < last; ++first) {
			destroy(&*first);
		}
	}

	//���Ԫ�ص���ֵ�ͱ���trivial destructor
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator lase, _true_type) {}

	/*template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		typedef typename _type_traits<T>::is_POD_type is_POD_type;
		_destroy(first, last, is_POD_type);
	}*/
	
}
#endif // !_CONSTRUCT_H_

