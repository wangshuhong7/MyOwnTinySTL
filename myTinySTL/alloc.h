#pragma once
#ifndef _ALLOC_H
#define _ALLOC_H
#include<cstdlib>
namespace myTinySTL {

	
	class alloc {

	private:
		enum EAign { ALIGN = 8 };//小型区块的上调边界
		enum EMaxBytes { MAXBYTES = 128 };//上限，超过次上限直接调用malloc更佳
		enum ENFreeLists { NFREELISTS = (EMaxBytes::MAXBYTES / EAign::ALIGN) };//有多少个区块，就要有多少freelist来保证其回收
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
			return ((bytes + EAign::ALIGN - 1) / EAign::ALIGN);
		}
		//根据区块的大小，决定使用第n号free_list,n从0开始
		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+EAign::ALIGN - 1) / EAign::ALIGN - 1);
		}
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到 free list
		static void *refill(size_t n);

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
	void *alloc::refill(size_t n) {

	}

}

#endif // !_ALLOC_H

