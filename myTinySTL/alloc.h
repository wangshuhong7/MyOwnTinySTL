#pragma once
#ifndef _ALLOC_H
#define _ALLOC_H
#include<cstdlib>
namespace myTinySTL {

	enum EAign { ALIGN = 8 };//С��������ϵ��߽�
	enum EMaxBytes { MAXBYTES = 128 };//���ޣ�����������ֱ�ӵ���malloc����
	enum ENFreeLists { NFREELISTS = (EMaxBytes::MAXBYTES / EAign::ALIGN) };//�ж��ٸ����飬��Ҫ�ж���freelist����֤�����
	enum ENObjs { NOBJS = 20 };//ÿ������Ľڵ���

	//freelists�Ľڵ㹹��
	union obj
	{//����union��ʡ�ռ䣬�������ȿ��Է���һ���ָ�룬�ֿ��Դ�����ݡ�
		//union�����ڴ棬���Сȡ����ռ�ÿռ�����һ����Ա�Ĵ�С�����ڿ��Զ��ж��ֲ�ͬ����������
		union obj *next;
		char client[1];
	};

	class alloc {

	private:
		//ʹ�þ�̬��ԭ������Щֵ����Ψһ�ġ�һ���ı䣬ȫ�־͸��Ÿı���
		static char *start_free;//�ڴ�ص���ʼλ��
		static char *end_free;//�ڴ�صĽ���λ��
		static size_t heap_size;//�ѵĴ�С
		//16��freelists
		static obj *free_list[ENFreeLists::NFREELISTS];
	private:
		//��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + EAign::ALIGN - 1) / EAign::ALIGN);
		}
		//��������Ĵ�С������ʹ�õ�n��free_list,n��0��ʼ
		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+EAign::ALIGN - 1) / EAign::ALIGN - 1);
		}
		//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽 free list
		static void *refill(size_t n);

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
	obj* alloc::free_list[ENFreeLists::NFREELISTS] = {
		nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	};
	

}

#endif // !_ALLOC_H

