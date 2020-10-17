#pragma once
#ifndef _ALLOC_H
#define _ALLOC_H
#include<cstdlib>
namespace myTinySTL {

	
	class alloc {

	private:
		enum EAlign { ALIGN = 8 };//小型区块的上调边界
		enum EMaxBytes { MAXBYTES = 128 };//上限，超过次上限直接调用malloc更佳
		enum ENFreeLists { NFREELISTS = (EMaxBytes::MAXBYTES / EAlign::ALIGN) };//有多少个区块，就要有多少freelist来保证其回收
		enum ENObjs { NOBJS = 20 };//每次增添的节点数

		//freelists的节点构造
		union obj
		{//利用union节省空间，这个区块既可以放下一块的指针，又可以存放数据。
			//union共享内存，其大小取决于占用空间最大的一个成员的大小。其内可以定有多种不同的数据类型
			union obj *next;
			char client[1];
		};

		//使用静态的原因是这些值都是唯一的。一旦改变，全局就跟着改变了
		static char *start_free;//内存池的起始位置
		static char *end_free;//内存池的结束位置
		static size_t heap_size;//堆的大小
		//16个freelists
		static obj *free_list[ENFreeLists::NFREELISTS];
	private:
		//将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + EAlign::ALIGN - 1) / EAlign::ALIGN);
		}
		//根据区块的大小，决定使用第n号free_list,n从0开始
		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
		}
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到 free list
		static void *refill(size_t bytes);

		//配置一大块空间，可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有所不便（其实就是没有这么多连续内存拿的出来），nojbs可能会降低
		static char *chunk_alloc(size_t size, size_t &nobjs);
	public:
		//外部可以调用的部分
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	};
	//以下是 static data member 的定义与初值设定
	char* alloc::start_free = 0;
	char* alloc::end_free = 0;
	size_t alloc::heap_size = 0;
	alloc::obj *alloc::free_list[ENFreeLists::NFREELISTS] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	void *alloc::allocate(size_t bytes) {
		if (bytes > EMaxBytes::MAXBYTES) {
			return malloc(bytes);
		}
		size_t index = FREELIST_INDEX(bytes);
		obj *list = free_list[index];
		if (list) {
			//次list还有空间
			free_list[index] = list->next;
			return list;
		}
		else {
			//次list没有足够空间，要从内存池取空间
			return refill(ROUND_UP(bytes));
		}
	}

	void alloc::deallocate(void *ptr, size_t bytes) {
		if (bytes > EMaxBytes::MAXBYTES) {
			free(ptr);
		}
		else {
			obj *node = static_cast<obj*>(ptr);
			size_t index = FREELIST_INDEX(bytes);
			node->next = free_list[index];
			free_list[index] = node;
		}
	}

	void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz) {
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);
	}

	//返回一个大小为n的对象，并且有时候会为适当的free list增加节点
	//假设bytes已经上调为8的倍数
	void *alloc::refill(size_t bytes) {
		size_t nobjs = ENObjs::NOBJS;
		//从内存池取空间
		char *chunk = chunk_alloc(bytes, nobjs);
		obj **my_free_list = 0;
		obj *result = 0;
		obj *current_obj = 0;
		obj *next_obj = 0;
		//如果只获得一个区块，这个区块就分配给调用者用，free list无新节点
		if (nobjs == 1)	return chunk;
		//否则准备调整free list,纳入新节点
		my_free_list = free_list + FREELIST_INDEX(bytes);
		//以下在chunk空间建立free list
		result = (obj*)chunk;//这一块准备返回给客户端
		//引导free list指向新配置的空间（取自内存池）
		*my_free_list = next_obj = (obj*)(chunk + bytes);
		//将取出的多余的空间加入到相应的free list里面去
		//即将free list 各节点串接起来
		for (int i = 1;; i++) {
			//从第1个开始，因为第0个将返回给客户端
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + bytes);
			if (nobjs - 1 == i) {
				current_obj->next = 0;
				break;
			}
			else {
				current_obj->next = next_obj;
			}
		}
		return result;
	}
	//假设bytes已经上调为8的倍数
	char *alloc::chunk_alloc(size_t bytes, size_t& nobjs) {
		char *result = 0;
		size_t total_bytes = bytes * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes) {//内存池剩余空间完全满足需要
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else if (bytes_left >= bytes) {//内存池剩余空间不能完全满足需要，但足够供应一个或以上的区块
			nobjs = bytes_left / bytes;
			total_bytes = nobjs * bytes;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else {//内存池剩余空间连一个区块的大小都无法提供
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0) {
				obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj *)start_free)->next = *my_free_list;
				*my_free_list = (obj *)start_free;
			}
			//配置heap用来补充内存池
			start_free = (char *)malloc(bytes_to_get);
			if (!start_free) {
				obj **my_free_list = 0, *p = 0;
				for (int i = 0; i <= EMaxBytes::MAXBYTES; i += EAlign::ALIGN) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (!p) {
						*my_free_list = p->next;
						start_free = (char *)p;
						end_free = start_free + i;
						return chunk_alloc(bytes, nobjs);
					}
				}
				end_free = 0;
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(bytes, nobjs);
		}
	}
}

#endif // !_ALLOC_H

