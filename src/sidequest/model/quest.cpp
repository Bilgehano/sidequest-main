#include "quest.h"

namespace Sidequest 
{
	Quest::Quest()
		: id(0), parent(nullptr)
	{
	}

	
	


	Quest::Quest(Id id, std::string caption, Quest* parent, std::vector<Quest*> subquests)
		: id(id), caption(caption), parent(parent), subquests(subquests)
	{
	}
	
	
	Quest::~Quest()
	{
	}
}