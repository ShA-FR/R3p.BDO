#include "ActorList.h"

ActorList::ActorList(__int64 address) : MemoryObject(address)
{	
	m_Container = new LinkedList(m_Address);
	m_List = std::vector<ActorData*>();
}

ActorList::~ActorList()
{
	delete m_Container;
	m_List.clear();
}

std::vector<ActorData*> ActorList::getList()
{
	if(m_Container != nullptr)
	{
		m_List = m_Container->getDataList<ActorData>();
	}

	return m_List;
}
