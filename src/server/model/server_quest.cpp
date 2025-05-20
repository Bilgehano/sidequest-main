#include "server_quest.h"
#include "storage/query.h"
#include "storage/database.h"

namespace Sidequest {
	namespace Server {

		ServerQuest::ServerQuest(Database* database)
			: Persistable(database) {
		}

		ServerQuest::ServerQuest(Database* database, Id id)
			: Persistable(database) {
			this->id = id;
		}

		ServerQuest::ServerQuest(Database* database, const std::string& caption, Quest* parent)
			: Persistable(database) {
			this->caption = caption;
			this->parent = parent;
			this->id = 0; // noch nicht gesetzt
		}

		ServerQuest::~ServerQuest() {}

		// CREATE
		void ServerQuest::create_on_database() {
			Query query(database, "INSERT INTO quest(caption, parent_id) VALUES (?, ?);");
			query.bind(1, caption);
			query.bind(2, parent ? static_cast<unsigned int>(parent->id) : 0);

			if (!query.step_done()) {
				throw UnableToCreateObjectException(caption);
			}
		}

		// READ
		void ServerQuest::read_on_database() {
			Query query(database, "SELECT * FROM quest WHERE id = ?;");
			query.bind(1, static_cast<unsigned int>(id));

			if (!query.step()) {
				throw UnableToReadObjectException(std::to_string(id));
			}

			caption = query.get_text("caption");
			int parent_id = query.get_int("parent_id");
			if (parent_id != 0) {
				parent = new Quest();
				parent->id = parent_id;
				// Optional: parent->read_on_database() rekursiv aufrufen
			}
		}

		// UPDATE
		void ServerQuest::update_on_database() {
			
		}

		// DELETE
		void ServerQuest::delete_on_database() {
			
		}





		std::string ServerQuest::class_id() {
			return "quest";
		}

	}
}
