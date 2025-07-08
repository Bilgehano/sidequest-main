#pragma once

#include <string>
#include <vector>
#include <optional>

#include <model/id.h>

namespace Sidequest
{
	class User;

	class Quest {
	public:
		enum Status {
			initial,
			done,
			inactive
		};

		Quest();
		Quest(Id id);
		Quest( Status status, std::string title, std::string description, User* owner, User* editor, Quest* parent );
		virtual ~Quest();

		static std::string status_to_string( Status status );
		static Status Quest::string_to_status(std::string status);

		Id id;
		std::string title = "";
		std::string description = "";
		Status status = Status::initial;

		User* owner = nullptr;
		std::optional<Id> owner_id;

		User* editor = nullptr;
		std::optional<Id> editor_id;

		Quest* parent = nullptr;
		std::optional<Id> parent_id;

		std::vector<Quest*> subquests;
	};
}