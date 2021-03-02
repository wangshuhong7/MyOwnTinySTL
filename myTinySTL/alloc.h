#ifndef MYTINYSTL_ALLOC_H_
#define MYTINYSTL_ALLOC_H_

// ���ͷ�ļ�����һ���� alloc�����ڷ���ͻ����ڴ棬���ڴ�صķ�ʽʵ��
//
// �� v2.0.0 �汾��ʼ��������ʹ���ڴ�أ�����ļ��������ã�����ʱ����
//
// ע�⣡����
// ����ļ���ܶ�ʵ���Ǵ��,�����ڡ�STLԴ����������ʹ���˸�˼�룬��ʱ����

#include <new>

#include <cstddef>
#include <cstdio>

namespace myTinySTL
{

	// ������: FreeList
	// ��������ķ�ʽ�����ڴ���Ƭ�����������С�ڴ棨<=4K������
	union FreeList
	{
		union FreeList* next;  // ָ����һ������
		char data[1];          // ���汾���ڴ���׵�ַ
	};

	// ��ͬ�ڴ淶Χ���ϵ���С
	enum
	{
		EAlign128 = 8,
		EAlign256 = 16,
		EAlign512 = 32,
		EAlign1024 = 64,
		EAlign2048 = 128,
		EAlign4096 = 256
	};

	// С������ڴ��С
	enum { ESmallObjectBytes = 4096 };

	// free lists ����
	enum { EFreeListsNumber = 56 };

	// �ռ������� alloc
	// ����ڴ�ϴ󣬳��� 4096 bytes��ֱ�ӵ��� std::malloc, std::free
	// ���ڴ��Сʱ�����ڴ�ع���ÿ������һ����ڴ棬��ά����Ӧ����������
	class alloc
	{
	private:
		static char*  start_free;                      // �ڴ����ʼλ��
		static char*  end_free;                        // �ڴ�ؽ���λ��
		static size_t heap_size;                       // ���� heap �ռ丽��ֵ��С

		static FreeList* free_list[EFreeListsNumber];  // ��������

	public:
		static void* allocate(size_t n);
		static void  deallocate(void* p, size_t n);
		static void* reallocate(void* p, size_t old_size, size_t new_size);

	private:
		static size_t M_align(size_t bytes);
		static size_t M_round_up(size_t bytes);
		static size_t M_freelist_index(size_t bytes);
		static void*  M_refill(size_t n);
		static char*  M_chunk_alloc(size_t size, size_t &nobj);
	};

	// ��̬��Ա������ʼ��

	char*  alloc::start_free = nullptr;
	char*  alloc::end_free = nullptr;
	size_t alloc::heap_size = 0;

	FreeList* alloc::free_list[EFreeListsNumber] = {
	  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	  nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
	};

	// �����СΪ n �Ŀռ䣬 n > 0
	inline void* alloc::allocate(size_t n)
	{
		FreeList* my_free_list;
		FreeList* result;
		if (n > static_cast<size_t>(ESmallObjectBytes))
			return std::malloc(n);
		my_free_list = free_list[M_freelist_index(n)];
		result = my_free_list;
		if (result == nullptr)
		{
			void* r = M_refill(M_round_up(n));
			return r;
		}
		my_free_list = result->next;
		return result;
	}

	// �ͷ� p ָ��Ĵ�СΪ n �Ŀռ�, p ����Ϊ 0
	inline void alloc::deallocate(void* p, size_t n)
	{
		if (n > static_cast<size_t>(ESmallObjectBytes))
		{
			std::free(p);
			return;
		}
		FreeList* q = reinterpret_cast<FreeList*>(p);
		FreeList* my_free_list;
		my_free_list = free_list[M_freelist_index(n)];
		q->next = my_free_list;
		my_free_list = q;
	}

	// ���·���ռ䣬������������������һΪָ���¿ռ��ָ�룬������Ϊԭ���ռ�Ĵ�С��������Ϊ����ռ�Ĵ�С
	inline void* alloc::reallocate(void* p, size_t old_size, size_t new_size)
	{
		deallocate(p, old_size);
		p = allocate(new_size);
		return p;
	}

	// bytes ��Ӧ�ϵ���С
	inline size_t alloc::M_align(size_t bytes)
	{
		if (bytes <= 512)
		{
			return bytes <= 256
				? bytes <= 128 ? EAlign128 : EAlign256
				: EAlign512;
		}
		return bytes <= 2048
			? bytes <= 1024 ? EAlign1024 : EAlign2048
			: EAlign4096;
	}

	// �� bytes �ϵ�����Ӧ�����С
	inline size_t alloc::M_round_up(size_t bytes)
	{
		return ((bytes + M_align(bytes) - 1) & ~(M_align(bytes) - 1));
	}

	// ���������С��ѡ��� n �� free lists
	inline size_t alloc::M_freelist_index(size_t bytes)
	{
		if (bytes <= 512)
		{
			return bytes <= 256
				? bytes <= 128
				? ((bytes + EAlign128 - 1) / EAlign128 - 1)
				: (15 + (bytes + EAlign256 - 129) / EAlign256)
				: (23 + (bytes + EAlign512 - 257) / EAlign512);
		}
		return bytes <= 2048
			? bytes <= 1024
			? (31 + (bytes + EAlign1024 - 513) / EAlign1024)
			: (39 + (bytes + EAlign2048 - 1025) / EAlign2048)
			: (47 + (bytes + EAlign4096 - 2049) / EAlign4096);
	}

	// ������� free list
	void* alloc::M_refill(size_t n)
	{
		size_t nblock = 10;
		char* c = M_chunk_alloc(n, nblock);
		FreeList* my_free_list;
		FreeList* result, *cur, *next;
		// ���ֻ��һ�����飬�Ͱ�������鷵�ظ������ߣ�free list û�������½ڵ�
		if (nblock == 1)
			return c;
		// �����һ������������ߣ�ʣ�µ����� free list ��Ϊ�½ڵ�
		my_free_list = free_list[M_freelist_index(n)];
		result = (FreeList*)c;
		my_free_list = next = (FreeList*)(c + n);
		for (size_t i = 1; ; ++i)
		{
			cur = next;
			next = (FreeList*)((char*)next + n);
			if (nblock - 1 == i)
			{
				cur->next = nullptr;
				break;
			}
			else
			{
				cur->next = next;
			}
		}
		return result;
	}

	// ���ڴ����ȡ�ռ�� free list ʹ�ã�����������ʱ������� nblock
	char* alloc::M_chunk_alloc(size_t size, size_t& nblock)
	{
		char* result;
		size_t need_bytes = size * nblock;
		size_t pool_bytes = end_free - start_free;

		// ����ڴ��ʣ���С��ȫ������������������
		if (pool_bytes >= need_bytes)
		{
			result = start_free;
			start_free += need_bytes;
			return result;
		}

		// ����ڴ��ʣ���С������ȫ�����������������ٿ��Է���һ����һ�����ϵ����飬�ͷ�����
		else if (pool_bytes >= size)
		{
			nblock = pool_bytes / size;
			need_bytes = size * nblock;
			result = start_free;
			start_free += need_bytes;
			return result;
		}

		// ����ڴ��ʣ���С��һ�����鶼�޷�����
		else
		{
			if (pool_bytes > 0)
			{ // ����ڴ�ػ���ʣ�࣬��ʣ��Ŀռ���뵽 free list ��
				FreeList* my_free_list = free_list[M_freelist_index(pool_bytes)];
				((FreeList*)start_free)->next = my_free_list;
				my_free_list = (FreeList*)start_free;
			}
			// ���� heap �ռ�
			size_t bytes_to_get = (need_bytes << 1) + M_round_up(heap_size >> 4);
			start_free = (char*)std::malloc(bytes_to_get);
			if (!start_free)
			{ // heap �ռ�Ҳ����
				FreeList* my_free_list, *p;
				// ���Ų�������δ�����飬�������㹻��� free list
				for (size_t i = size; i <= ESmallObjectBytes; i += M_align(i))
				{
					my_free_list = free_list[M_freelist_index(i)];
					p = my_free_list;
					if (p)
					{
						my_free_list = p->next;
						start_free = (char*)p;
						end_free = start_free + i;
						return M_chunk_alloc(size, nblock);
					}
				}
				std::printf("out of memory");
				end_free = nullptr;
				throw std::bad_alloc();
			}
			end_free = start_free + bytes_to_get;
			heap_size += bytes_to_get;
			return M_chunk_alloc(size, nblock);
		}
	}

} // namespace myTinySTL
#endif // !MYTINYSTL_ALLOC_H_


