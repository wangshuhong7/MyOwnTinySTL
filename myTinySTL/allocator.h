#pragma once
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include"alloc.h"
#include"construct.h"
#include<cassert>
#include<new>

namespace myTinySTL {
	//�ռ����������Ա�����ĿΪ��λ����
	template<class T>
	class allocator {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;//ptrdiff_t���ͱ���ͨ��������������ָ����������Ľ����

	public:
		static T *allocate();
		static T *allocate(size_t n);
		static void deallocate(T *ptr);
		static void deallocate(T *ptr, size_t n);

		static void construct(T *ptr);
		static void construct(T *ptr, const T& value);
		static void destroy(T *ptr);
		static void destroy(T *first, T *last);
	};
	template<class T>
	T *allocator<T>::allocate() {
		//static_cast��һ��C++������������ǰ�һ�����ʽת��Ϊĳ�����ͣ���û������ʱ���ͼ������֤ת���İ�ȫ��
		return static_cast<T*>(alloc::allocate(sizeof(T)));//alloc���allocate�����allocate��һ����
	}

	template<class T>
	T *allocator<T>::allocate(size_t n) {
		if (n == 0)	return;
		return static_cast<T*>(alloc::allocate(sizeof(T)*n));//һ����������T��С�Ŀռ�
	}

	template<class T>
	void allocator<T>::deallocate(T *ptr) {
		alloc::deallocate(static_cast<void*>(ptr), sizeof(T));//ע��ǿ��ת�ͣ�����alloc�е�deallocate
	}

	template<class T>
	void allocator<T>::deallocate(T *ptr, size_t n) {
		alloc::deallocate(static_cast<void*>(ptr), sizeof(T)*n);
	}

	template<class T>
	void allocator<T>::construct(T *ptr) {
		new(ptr)T();//��newΪ��ַptr���ù��캯��
	}

	template<class T>
	void allocator<T>::construct(T *ptr, const T& value) {
		new(ptr)T(value);
	}

	template<class T>
	void allocator<T>::destroy(T *ptr) {
		ptr->~T();
	}

	template<class T>
	void allocator<T>::destroy(T *first, T *last) {
		for (; first < last; ++first) {
			first->~T();//��һ������������
		}
	}
}
#endif // !_ALLOCATOR_H_
