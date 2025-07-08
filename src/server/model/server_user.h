#pragma once

#include <string>
#include <vector>

#include <network/serialisable_user.h>
#include <storage/persistable.h>

namespace Sidequest
{
	namespace Server
	{
		class Query;

		class ServerUser : public Sidequest::SerialisableUser, public Persistable {
		public:
			typedef unsigned long Id;

			ServerUser(Database* database);
			ServerUser(Database* database, Id id);
			ServerUser(Database* database, std::string email, std::string display_name, std::string password);
			~ServerUser();

			virtual void create_on_database() override;
			virtual void read_on_database(Id id) override;
			virtual void update_on_database() override;
			virtual void delete_on_database() override;

			void read_from_query( Query& query );

			virtual std::string class_id() override;
		};

	}
}
