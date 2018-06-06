#include "LinkedListEntry.h"

LinkedListEntry::LinkedListEntry(long long address) : MemoryObject(address)
{
}

LinkedListEntry::~LinkedListEntry()
{
}

LinkedListEntry* LinkedListEntry::getNext()
{
	return (LinkedListEntry*)(m_Address + o_Next);
}

LinkedListEntry* LinkedListEntry::getPrevious()
{
	return (LinkedListEntry*)(m_Address + o_Previous);
}


