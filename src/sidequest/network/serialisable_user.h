#pragma once

#include <string>
#include <vector>

#include "model/user.h"
#include "network/json_serialisable.h"

namespace Sidequest
{
	class Quest;

	class SerialisableUser : public User, public JsonSerialisable {
	public:
		SerialisableUser();
		SerialisableUser(Id id);
		SerialisableUser(std::string email, std::string display_name, std::string password);
		virtual ~SerialisableUser();

		// implementation of JsonSerialisable
		virtual Json to_json(bool full_serialise = true) override;
		virtual void from_json(const Json& json) override;
	};
}