#pragma once
#ifndef _VECTOR_H_
#define _VECTOR_H_
#include<algorithm>
#include<type_traits>

#include"algorithm.h"
#include"allocator.h"
#include"iterator.h"
#include"reverseIterator.h"
#include"uninitialized.h"
namespace myTinySTL {
	template<class T,class Alloc=allocator<T>>
	class vector {
	public:
		//vector的嵌套型别定义
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type* iterator;
		typedef value_type& reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	protected:
		typedef simple_alloc<value_type,Alloc> data_allocator;
		
		iterator start;//表示目前使用空间的头部
		iterator finish;//表示目前使用空间的尾部
		iterator end_of_storage;//表示可用空间的尾部

		void insert_aux(iterator position, const T& x) {
			if (finish != end_of_storage) {
				//还有备用空间
				construct(finish, *(finish - 1));
				++finish;
				T x_copy = x;
				copy_backward(position, finish - 2, finish - 1);//从最后一个元素向前复制到result中
				*position = x_copy;
			}
			else {
				//已经没有备用空间
				const size_type old_size = size();
				const size_type len = old_size != 0?2*old_size:1;
				iterator new_start = data_allocator::allocate(len);//实际配置
				iterator new_finish = new_start;
				try {
					//将原来vector内容拷贝到新的vector
					
					new_finish = uninitialized_copy(start, position, new_start);
					//为新元素设置初值
					construct(new_finish, x);
					++new_finish;
					//将原来vector备用空间中的内容也忠实的拷贝过来
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch(...){
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				//析构并释放原来vector
				destroy(begin(), end());
				deallocate();
				//调整迭代器指向新vector
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}
		void deallocate() {
			if (start) {
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}
		//配置空间并且填满
		iterator allocate_and_fill(size_type n, const T& x) {
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n_aux(result, n, x);
			return result;
		}
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}
		
		void insert(iterator position, size_type n, const T& x) {
			if (n != 0) {
				//备用空间大于等于“新增元素个数”
				T x_copy = x;
				//计算插入点之后的现有元素个数
				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n) {
					//插入点之后的现有元素个数大于新增元素个数
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backword(position, old_finish - n, old_finish);
					fill(position, position + n, x_copy);

				}
				else {
					uninitialized_fill_n_aux(finish, n - elems_after, x_copy);
					finish += n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}
			}
			else {
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);
				//以下配置新的vector空间
				iterator new_start = data_allocator::allocate(len);//实际配置
				iterator new_finish = new_start;
				try {
					//将原来vector内容拷贝到新的vector
					new_finish = uninitialized_copy(start, position, new_start);
					//为新元素设置初值
					new_finish = uninitialized_fill_n_aux(new_finish, n, x);
					//将原来vector备用空间中的内容也忠实的拷贝过来
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch(...){
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				//析构并释放原来vector
				destroy(begin(), end());
				deallocate();
				//调整迭代器指向新vector
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}
	public:
		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() { return size_type(end() - begin()); }
		size_type capacity() const { return size_type(end_of_storage - begin()); }
		bool empty() const {
			return begin() == end();
		}
		reference operator[](size_type n) { return *(begin() + n); }
		vector() :start(0), finish(0), end_of_storage(0) {}
		vector(size_type n, const T& value) { fill_initialize(n, value); }
		vector(int n, const T& value) { fill_initialize(n, value); }
		vector(long n, const T& value) { fill_initialize(n, value); }
		explicit vector(size_type n) { fill_initialize(n, T()); }

		~vector() {
			destroy(start, finish);
			deallocate();
		}
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }
		void push_back(const T& x) {
			if (finish != end_of_storage) {
				construct(finish, x);
				++finish;
			}
			else {
				insert_aux(end(), x);
			}
		}
		void pop_back(){
			--finish;
			destroy(finish);
		}
		iterator erase(iterator position) {
			if (position + 1 != end()) {
				uninitialized_copy(position, finish, position);//后续元素往前移动
				--finish;
				destroy(finish);
				return position;
			}
		}
		void resize(size_type new_size,const T& x) {
			if (new_size < size()) {
				erase(begin() + new_size, end());
			}
			else {
				insert(end(), new_size - size(), x);
			}
		}

		void resize(size_type new_size) { resize(new_size, T()); }
		void clear() { erase(begin(), end()); }
	};
	
}
#endif // !_VECTOR_H_
