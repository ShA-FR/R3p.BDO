#ifndef H_LINKEDLIST
#define H_LINKEDLIST

#include "LinkedListEntry.h"

class LinkedList : public MemoryObject
{
private:
	const int o_FirstEntry = 0x00;

public:
	LinkedList(__int64 address);
	~LinkedList();

	template <typename T>
	std::vector<T*> getDataList();
};

#endif

template <typename T>
inline std::vector<T*> LinkedList::getDataList()
{
	std::vector<T*> listBuffer;

	LinkedListEntry* firstEntry = (LinkedListEntry*)(m_Address + o_FirstEntry);

	if (firstEntry == nullptr || firstEntry->getNext() == nullptr)
		return listBuffer;

	LinkedListEntry* currentEntry = firstEntry->getNext();

	while (currentEntry->m_Address != firstEntry->m_Address)
	{
		T* data = currentEntry->getData<T>();

		if (data != nullptr)
			listBuffer.push_back(data);

		currentEntry = currentEntry->getNext();
	}

	return listBuffer;
}