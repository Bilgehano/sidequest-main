#include "quest.h"

#include <stdexcept>

#include "user.h"

namespace Sidequest 
{

	Quest::Quest()
	{
	}

	Quest::Quest(Id id)
		: id(id)
	{
	}

	Quest::Quest(Status status, std::string title, std::string description, User* owner, User* editor, Quest* parent)
		: status(status)
		, title(title)
		, description(description)
		, owner(owner)
		, editor(editor)
		, parent(parent)
	{
		if (owner != nullptr)
			owner_id = owner->id;
		if (editor != nullptr)
			editor_id = editor->id;
		if (parent != nullptr)
			parent_id = parent->id;
	}

	Quest::~Quest()
	{
		for (auto subquest : subquests)
			delete(subquest);
	}

	std::string Quest::status_to_string( Status status )
	{
		if (status == Status::initial)
			return "initial";
		if (status == Status::done)
			return "done";
		if (status == Status::inactive)
			return "inactive";
		throw std::runtime_error("illegal status");
	}

	Quest::Status Quest::string_to_status(std::string status)
	{
		if (status == "initial")
			return Status::initial;
		if (status == "done")
			return Status::done;
		if (status == "inactive")
			return Status::inactive;
		throw std::runtime_error("illegal status");
	}

}