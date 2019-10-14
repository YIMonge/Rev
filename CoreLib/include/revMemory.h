#ifndef __REVMEMORY_H__
#define __REVMEMORY_H__

#include "revTypedef.h"

#define MEM_DEFINE(type, block) \
	void* type##::operator new(size_t size)\
{\
	return revMemory::Get().Allocate(size, block); \
}\
	void* type##::operator new[](size_t size)\
{\
	return revMemory::Get().Allocate(size, block); \
}\
	void type##::operator delete(void* pv)\
{\
	revMemory::Get().Free(pv, block); \
}\
	void type##::operator delete[](void* pv)\
{\
	revMemory::Get().Free(pv, block); \
}\

#define SAFE_DELETE(ptr) if(ptr) delete ptr;

enum class HEAP_TYPE : uint8 {
	ORIGIN,				// origin heap, it includes below heaps
	RESOURCE,			// for load resource
	GAME_SCENE,			// for runtime gameobject
	VATIAVLE_ARRAY,		// for variavle array 
	PATICLE_UNIT,		// for particle
	MAX_NUM,
};

struct revHeapInfo
{
	HEAP_TYPE type;
	int size;
};

inline int ToKiloByte(int byte)
{
	return byte * 1024;
}

inline int ToMegaByte(int byte)
{
	return ToKiloByte(byte) * 1024;
}

inline int RoundUpAlignment(unsigned int value, unsigned int alignment){
	return (((value)+((alignment)-1)) & ~((alignment)-1));
}

class revMemory /* : public revSingleton<revMemory> */ // don't extend revSingleton 
{
public:
	static revMemory& Get();

	revMemory();

	void StartUp(int total_heap_size);
	void ShutDown();

	// malloc and free
	void* Allocate(unsigned int size, HEAP_TYPE heap_type);
	void Free(void* pointer, HEAP_TYPE heap_type);

private:
	// static CriticalSection cs;
	void* heaps[static_cast<uint8>(HEAP_TYPE::MAX_NUM)];
	static revMemory* instance;
	static unsigned char instance_memory[];	// memory for instance
};

#endif
