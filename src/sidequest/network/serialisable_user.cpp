#include "serialisable_user.h"

#include <stdlib.h>

#include <json.hpp>
using namespace nlohmann;

namespace Sidequest 
{

	SerialisableUser::SerialisableUser()
	{
	}

	SerialisableUser::SerialisableUser(Id id)
		: User(id)
	{
	}

	SerialisableUser::SerialisableUser(std::string email, std::string display_name, std::string password)
		: User( email, display_name, password )
	{
	}

	SerialisableUser::~SerialisableUser()
	{
	}

	Json SerialisableUser::to_json( bool full_serialise )
	{
		if (!full_serialise)
			return Json(std::to_string(id));

		Json json = {
			{ "id", id },
			{ "display_name", display_name },
			{ "email", email }
		};

		return json;
	}

	void SerialisableUser::from_json(const Json& json)
	{
		// id might not exist of the user data is send as a create-user request,
		// such the id is not yet assigned
		if ( json.contains("id"))
			json.at("id").get_to(id);
		json.at("display_name").get_to(display_name);
		json.at("email").get_to(email);
	}
}
