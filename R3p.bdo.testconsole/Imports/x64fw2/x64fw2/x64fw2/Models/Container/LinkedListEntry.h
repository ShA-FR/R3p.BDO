#ifndef H_LINKEDLISTENTRY
#define H_LINKEDLISTENTRY

#include "../../Utilities/MemoryObject.h"

class LinkedListEntry : public MemoryObject
{
private:
	const int o_Next = 0x00;
	const int o_Previous = 0x08;
	const int o_PtrData = 0x18;

public:
	LinkedListEntry(__int64 address);
	~LinkedListEntry();

	LinkedListEntry* getNext();
	LinkedListEntry* getPrevious();

	template <typename T>
	T* getData();
};

#endif

template <typename T>
inline T* LinkedListEntry::getData()
{
	return (T*)(m_Address + o_PtrData);
}