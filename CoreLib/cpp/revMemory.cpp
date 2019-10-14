#include <new>
#include <stdlib.h>
#include "../include/revMemory.h"

#define ONLY_MSPACES (1)
#define MALLOC_ALIGNMENT (8) 

#include <errno.h>

// TODO:make custom allocator 
//#include "../include/dlmalloc.c"

unsigned char revMemory::instance_memory[sizeof(revMemory)];
revMemory* revMemory::instance = nullptr;

revMemory& revMemory::Get()
{
	if (instance == nullptr){
		//cs.Lock();
		if (instance == nullptr){
			//#pragma push_macro("new")
			//#undef new
			instance = ::new(instance_memory)revMemory;
			//#pragma pop_macro("new")
		}
		//cs.UnLock();
	}
	return *instance;
}

revMemory::revMemory()
{
}

void revMemory::StartUp(int total_heap_size)
{
	for(auto heap : heaps){
		heap = nullptr;
	}
	//heaps[static_cast<uint8>(HEAP_TYPE::ORIGIN)] = create_mspace(total_heap_size, 0);
}

void revMemory::ShutDown()
{
	for(auto heap : heaps){
		if(heap != nullptr){
			//destroy_mspace(heap);
			heap = nullptr;
		}
	}
}

void* revMemory::Allocate(unsigned int size, HEAP_TYPE heap_type) 
{
	return malloc(size);
	// TODO: ASSERT
	//return mspace_malloc(heaps[static_cast<uint8>(heap_type)], size);
}

void revMemory::Free(void* pointer, HEAP_TYPE heap_type)
{
	free(pointer);
	// TODO: ASSERT
	//mspace_free(heaps[static_cast<uint8>(heap_type)], pointer);
}

void* operator new(size_t size)
{
	return revMemory::Get().Allocate(size, HEAP_TYPE::ORIGIN);
}

void* operator new[](size_t size)
{
	return revMemory::Get().Allocate(size, HEAP_TYPE::ORIGIN);
}

void operator delete(void* pv) noexcept
{
	revMemory::Get().Free(pv, HEAP_TYPE::ORIGIN);
}

void operator delete[](void* pv) noexcept
{
	revMemory::Get().Free(pv, HEAP_TYPE::ORIGIN);
}

