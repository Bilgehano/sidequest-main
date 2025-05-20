#pragma once

#include <string>
#include <vector>
#include <model/quest.h>
#include <storage/persistable.h>

namespace Sidequest {
	namespace Server {

		class ServerQuest : public Sidequest::Quest, public Persistable {
		public:
			ServerQuest(Database* database);
			ServerQuest(Database* database, Id id);
			ServerQuest(Database* database, const std::string& caption, Quest* parent = nullptr);
			~ServerQuest();

			virtual void create_on_database() override;
			virtual void read_on_database() override;
			virtual void update_on_database() override;
			virtual void delete_on_database() override;

			virtual std::string class_id() override;
		};

	}
}
