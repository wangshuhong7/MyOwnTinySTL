#pragma once
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include"alloc.h"
#include"construct.h"
#include<cassert>
#include<new>

namespace myTinySTL {
	//空间适配器，以变量数目为单位分配
	template<class T>
	class allocator {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;//ptrdiff_t类型变量通常用来保存两个指针减法操作的结果。

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
		//static_cast是一个C++运算符，功能是把一个表达式转换为某种类型，当没有运行时类型检查来保证转换的安全性
		return static_cast<T*>(alloc::allocate(sizeof(T)));//alloc里的allocate和这个allocate不一样！
	}

	template<class T>
	T *allocator<T>::allocate(size_t n) {
		if (n == 0)	return;
		return static_cast<T*>(alloc::allocate(sizeof(T)*n));//一次性申请多个T大小的空间
	}

	template<class T>
	void allocator<T>::deallocate(T *ptr) {
		alloc::deallocate(static_cast<void*>(ptr), sizeof(T));//注意强制转型，调用alloc中的deallocate
	}

	template<class T>
	void allocator<T>::deallocate(T *ptr, size_t n) {
		alloc::deallocate(static_cast<void*>(ptr), sizeof(T)*n);
	}

	template<class T>
	void allocator<T>::construct(T *ptr) {
		new(ptr)T();//用new为地址ptr调用构造函数
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
			first->~T();//逐一调用析构函数
		}
	}
}
#endif // !_ALLOCATOR_H_
