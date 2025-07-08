#pragma once

#include <string>

#include "httplib.h"

#include "model/id.h"

namespace Sidequest
{
	class SerialisableUser;
	class SerialisableQuest;

	namespace Client
	{
		typedef httplib::Request Request;
		typedef httplib::Response Response;

		class RemoteCallFailedException : public std::runtime_error
		{
		public:
			explicit RemoteCallFailedException(const std::string& message);
			explicit RemoteCallFailedException(const httplib::Error& error_code);
		};

		class Stubs
		{
		public:
			Stubs( httplib::Client& http_connection );
			~Stubs();

			Id create_user(SerialisableUser* user);
			SerialisableUser* read_user(Id id);
			void update_user(SerialisableUser* user);
			void delete_user(Id id);

			Id create_quest(SerialisableQuest* quest);
			SerialisableQuest* read_quest(Id id);
			void update_quest(SerialisableQuest* quest);
			void delete_quest(Id id);

			std::list< SerialisableQuest* > quests_by_parent(Id id);
			std::list< SerialisableQuest* > quests_by_owner(Id id);

		protected:
			httplib::Client& _http_client;
		};

	}
}