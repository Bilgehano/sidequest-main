#pragma once

#include <string>
#include <vector>

#include "id.h"

namespace Sidequest
{
	class Quest;

	class User {
	public:
		User();
		User(Id id);
		User(std::string email, std::string display_name, std::string password);
		virtual ~User();

		Id id;
		std::string display_name;
		std::string email;
		std::string password;

		std::vector<Quest*> main_quests;
	};
}