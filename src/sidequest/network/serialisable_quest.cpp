#include "serialisable_quest.h"

#include <stdexcept>

#include "model/user.h"
#include "network/serialisable_user.h"

namespace Sidequest 
{

	SerialisableQuest::SerialisableQuest()
		: Quest()
	{
	}
	
	SerialisableQuest::SerialisableQuest(Id id)
		: Quest(id)
	{
	}

	SerialisableQuest::SerialisableQuest(Status status, std::string title, std::string description, User* owner, User* editor, Quest* parent)
		: Quest( status, title, description, owner, editor, parent )
	{
	}

	SerialisableQuest::~SerialisableQuest()
	{
	}

	Json SerialisableQuest::to_json( bool full_serialise )
	{
		if (!full_serialise)
			return Json( std::to_string(id) );

		Json json = {
			{ "id", id },
			{ "title", title },
			{ "description", description },
			{ "status", status },
		};

		json["parent"] = serialize_loadable_pointer(dynamic_cast<JsonSerialisable*>(parent), parent_id, serialise_parent_recursive);
		json["owner"]  = serialize_loadable_pointer(dynamic_cast<JsonSerialisable*>(owner),  owner_id,  serialise_owner_recursive);
		json["editor"] = serialize_loadable_pointer(dynamic_cast<JsonSerialisable*>(editor), editor_id, serialise_editor_recursive);

		return json;
	}

	void SerialisableQuest::from_json(const Json& json)
	{
		// id might not exist if the quest data is send as a create-quest request,
		// such the id is not yet assigned
		if (json.contains("id"))
			json.at("id").get_to(id);

		title = json.at("title");
		description = json.at("description");
		status = json.at("status");

		parent = deserialize_loadable_pointer<SerialisableQuest>(json.at("parent"), parent_id);
		owner  = deserialize_loadable_pointer<SerialisableUser> (json.at("owner"),  owner_id);
		editor = deserialize_loadable_pointer<SerialisableUser> (json.at("editor"), editor_id);
	}

}