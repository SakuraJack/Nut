#pragma once

#include <mutex>

namespace Nut {

	struct AllocationStats
	{
		size_t TotalAllocated = 0; // 总分配内存 
		size_t TotalFreed = 0; // 总释放内存
	};

	struct Allocation 
	{
		void* Memory = 0;
		size_t Size = 0;
		const char* Category = 0;
	};

	namespace Memory
	{
		const AllocationStats& GetAllocationStats();
	}

	template <class T>
	struct Mallocator
	{
		typedef T value_type;

		Mallocator() = default;
		template <class U> constexpr Mallocator(const Mallocator <U>&) noexcept {}

		T* allocate(std::size_t n)
		{
#undef max
			if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
				throw std::bad_array_new_length();

			if (auto p = static_cast<T*>(std::malloc(n * sizeof(T)))) {
				return p;
			}

			throw std::bad_alloc();
		}

		void deallocate(T* p, std::size_t n) noexcept {
			std::free(p);
		}
	};

	struct AllocatorData
	{
		using MapAlloc = Mallocator<std::pair<const void* const, Allocation>>;
		using StatsMapAlloc = Mallocator<std::pair<const char* const, AllocationStats>>;
		using AllocationStatsMap = std::map<const char*, AllocationStats, std::less<const char*>, StatsMapAlloc>;

		std::map<const void*, Allocation, std::less<const void*>, MapAlloc> m_AllocationMap;
		AllocationStatsMap m_AllocationStatsMap;

		std::mutex m_Mutex, m_StatsMutex;
	};

	class Allocator
	{
	public:
		static void Init();

		static void* AllocateRaw(size_t size);

		static void* Allocate(size_t size);
		static void* Allocate(size_t size, const char* desc);
		static void* Allocate(size_t size, const char* file, const char* line);
		static void Free(void* memory);

		static const AllocatorData::AllocationStatsMap& GetAllocationStats() { return s_Data->m_AllocationStatsMap; }

	private:
		inline static AllocatorData* s_Data = nullptr;
	};
}

#if NT_TRACK_MEMORY

#ifdef NT_PLATFORM_WINDOWS

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size);

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(SIZE) _VCRT_ALLOCATOR
void* __CRTDECL opertor new[](size_t size);

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size, const char* desc);

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t size, const char* desc);

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size, const char* file, int line);

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t size, const char* file, int line);

void __CRTDECL operator delete(void* memory);
void __CRTDECL operator delete(void* memory, const char* desc);
void __CRTDECL operator delete(void* memory, const char* file, int line);
void __CRTDECL operator delete[](void* memory);
void __CRTDECL operator delete[](void* memory, const char* desc);
void __CRTDECL operator delete[](void* memory, const char* file, int line);

#define nnew new(__FILE__, __LINE__)
#define ndelete delete

#else
#warning "非Windows平台无法使用内存跟踪！"
#define nnew new
#define ndelete delete

#endif

#else 

#define nnew new
#define ndelete delete

#endif
