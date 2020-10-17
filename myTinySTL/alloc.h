#pragma once
#ifndef _ALLOC_H
#define _ALLOC_H
#include<cstdlib>
namespace myTinySTL {

	
	class alloc {

	private:
		enum EAlign { ALIGN = 8 };//С��������ϵ��߽�
		enum EMaxBytes { MAXBYTES = 128 };//���ޣ�����������ֱ�ӵ���malloc����
		enum ENFreeLists { NFREELISTS = (EMaxBytes::MAXBYTES / EAlign::ALIGN) };//�ж��ٸ����飬��Ҫ�ж���freelist����֤�����
		enum ENObjs { NOBJS = 20 };//ÿ������Ľڵ���

		//freelists�Ľڵ㹹��
		union obj
		{//����union��ʡ�ռ䣬�������ȿ��Է���һ���ָ�룬�ֿ��Դ�����ݡ�
			//union�����ڴ棬���Сȡ����ռ�ÿռ�����һ����Ա�Ĵ�С�����ڿ��Զ��ж��ֲ�ͬ����������
			union obj *next;
			char client[1];
		};

		//ʹ�þ�̬��ԭ������Щֵ����Ψһ�ġ�һ���ı䣬ȫ�־͸��Ÿı���
		static char *start_free;//�ڴ�ص���ʼλ��
		static char *end_free;//�ڴ�صĽ���λ��
		static size_t heap_size;//�ѵĴ�С
		//16��freelists
		static obj *free_list[ENFreeLists::NFREELISTS];
	private:
		//��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + EAlign::ALIGN - 1) / EAlign::ALIGN);
		}
		//��������Ĵ�С������ʹ�õ�n��free_list,n��0��ʼ
		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
		}
		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽 free list
		static void *refill(size_t bytes);

		//����һ���ռ䣬������nobjs����СΪsize������
		//�������nobjs�������������㣨��ʵ����û����ô�������ڴ��õĳ�������nojbs���ܻή��
		static char *chunk_alloc(size_t size, size_t &nobjs);
	public:
		//�ⲿ���Ե��õĲ���
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	};
	//������ static data member �Ķ������ֵ�趨
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
			//��list���пռ�
			free_list[index] = list->next;
			return list;
		}
		else {
			//��listû���㹻�ռ䣬Ҫ���ڴ��ȡ�ռ�
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

	//����һ����СΪn�Ķ��󣬲�����ʱ���Ϊ�ʵ���free list���ӽڵ�
	//����bytes�Ѿ��ϵ�Ϊ8�ı���
	void *alloc::refill(size_t bytes) {
		size_t nobjs = ENObjs::NOBJS;
		//���ڴ��ȡ�ռ�
		char *chunk = chunk_alloc(bytes, nobjs);
		obj **my_free_list = 0;
		obj *result = 0;
		obj *current_obj = 0;
		obj *next_obj = 0;
		//���ֻ���һ�����飬�������ͷ�����������ã�free list���½ڵ�
		if (nobjs == 1)	return chunk;
		//����׼������free list,�����½ڵ�
		my_free_list = free_list + FREELIST_INDEX(bytes);
		//������chunk�ռ佨��free list
		result = (obj*)chunk;//��һ��׼�����ظ��ͻ���
		//����free listָ�������õĿռ䣨ȡ���ڴ�أ�
		*my_free_list = next_obj = (obj*)(chunk + bytes);
		//��ȡ���Ķ���Ŀռ���뵽��Ӧ��free list����ȥ
		//����free list ���ڵ㴮������
		for (int i = 1;; i++) {
			//�ӵ�1����ʼ����Ϊ��0�������ظ��ͻ���
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
	//����bytes�Ѿ��ϵ�Ϊ8�ı���
	char *alloc::chunk_alloc(size_t bytes, size_t& nobjs) {
		char *result = 0;
		size_t total_bytes = bytes * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes) {//�ڴ��ʣ��ռ���ȫ������Ҫ
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else if (bytes_left >= bytes) {//�ڴ��ʣ��ռ䲻����ȫ������Ҫ�����㹻��Ӧһ�������ϵ�����
			nobjs = bytes_left / bytes;
			total_bytes = nobjs * bytes;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else {//�ڴ��ʣ��ռ���һ������Ĵ�С���޷��ṩ
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0) {
				obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj *)start_free)->next = *my_free_list;
				*my_free_list = (obj *)start_free;
			}
			//����heap���������ڴ��
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

