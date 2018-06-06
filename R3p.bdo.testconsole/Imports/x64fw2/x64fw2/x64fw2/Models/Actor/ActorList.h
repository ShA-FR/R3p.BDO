#ifndef H_ACTORLIST
#define H_ACTORLIST
#include "ActorData.h"
#include "../Container/LinkedList.h"

class ActorList : public MemoryObject
{
private:
	const int o_Container = 0x00;

	LinkedList* m_Container;
	std::vector<ActorData*> m_List;

public:	
	ActorList(__int64 address);
	~ActorList();

	std::vector<ActorData*> getList();
};

#endif