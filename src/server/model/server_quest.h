#pragma once

#include <string>
#include <vector>

#include <network/serialisable_quest.h>
#include <storage/persistable.h>
#include <storage/query.h>

namespace Sidequest
{
	namespace Server
	{
		class ServerUser;

		class ServerQuest : public Sidequest::SerialisableQuest, public Persistable {
		public:
			typedef unsigned long Id;

			ServerQuest(Database* database);
			ServerQuest(Database* database, Id id );
			ServerQuest(Database* database, Status status, std::string title, std::string description, ServerUser* owner, ServerUser* editor, Quest* parent );
			~ServerQuest();

			virtual void create_on_database() override;
			virtual void read_on_database(Id id) override;
			virtual void update_on_database() override;
			virtual void delete_on_database() override;

			virtual std::string class_id() override;

			void bind_all_parameters(Query& query);
			void read_from_query(Query& query);

		};

	}
}
