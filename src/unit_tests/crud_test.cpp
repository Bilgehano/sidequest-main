#include <gtest/gtest.h>
#include "storage/database.h"
#include "model/server_user.h"
#include "model/server_quest.h"

class CRUDTests : public ::testing::Test
{
protected:
	Sidequest::Server::Database* database;

	void SetUp() override {
		database = new Sidequest::Server::Database(":memory:");

		// Tabelle für Benutzer
		database->execute("CREATE TABLE user(email TEXT PRIMARY KEY, display_name TEXT, password TEXT);");

		// Tabelle für Quests
		database->execute("CREATE TABLE quest(id INTEGER PRIMARY KEY AUTOINCREMENT, caption TEXT, parent_id INTEGER);");
	}


	void TearDown() override {
		delete database;
	}
};

using namespace Sidequest::Server;

TEST_F(CRUDTests, OPEN_DATABASE)
{
	// NOP
}

TEST_F(CRUDTests, CRUD_USER_CREATE)
{
	auto user = new ServerUser(database, "crud_user_create@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	auto user2 = new ServerUser(database, "crud_user_create@hs-aalen.de");
	user2->read_on_database();

	EXPECT_EQ(user2->display_name, "Temporary User");
	delete(user2);
}

TEST_F(CRUDTests, CRUD_USER_CREATE_DOUBLE)
{
	auto user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	try {
		auto user_dup = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User 2", "");
		user_dup->create_on_database();
		delete(user_dup); // sollte nicht erreicht werden
		FAIL();
	}
	catch (const UnableToCreateObjectException&) {
		// erwartet
	}
}

TEST_F(CRUDTests, CRUD_USER_READ)
{
	auto user = new ServerUser(database, "crud_user_read@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	auto user2 = new ServerUser(database, "crud_user_read@hs-aalen.de");
	user2->read_on_database();

	EXPECT_EQ(user2->display_name, "Temporary User");
	delete(user2);
}

TEST_F(CRUDTests, CRUD_USER_UPDATE)
{
	auto user = new ServerUser(database, "crud_user_update@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	user->display_name = "Changed Display Name";
	user->update_on_database();
	delete(user);

	auto user2 = new ServerUser(database, "crud_user_update@hs-aalen.de");
	user2->read_on_database();

	EXPECT_EQ(user2->display_name, "Changed Display Name");
	delete(user2);
}

TEST_F(CRUDTests, CRUD_USER_DELETE)
{
	auto user = new ServerUser(database, "crud_user_delete@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	auto user2 = new ServerUser(database, "crud_user_delete@hs-aalen.de");
	user2->delete_on_database();
	delete(user2);

	try {
		auto user3 = new ServerUser(database, "crud_user_delete@hs-aalen.de");
		user3->read_on_database();
		delete(user3); // falls doch erfolgreich
		FAIL();
	}
	catch (const UnableToReadObjectException&) {
		// erwartet
	}
}

TEST_F(CRUDTests, CRUD_QUEST_CREATE)
{
	auto quest = new ServerQuest(database, "crud_quest_create");
	quest->create_on_database();
	delete quest;

	SUCCEED(); // ID nicht bekannt → kein direkter Check
}

TEST_F(CRUDTests, CRUD_QUEST_READ)
{
	auto quest = new ServerQuest(database, "crud_quest_read");
	quest->create_on_database();

	unsigned int id = database->read_int_value(
		database->prepare("SELECT id FROM quest WHERE caption = 'crud_quest_read'"),
		"id");

	auto quest2 = new ServerQuest(database, id);
	quest2->read_on_database();

	EXPECT_EQ(quest2->caption, "crud_quest_read");

	delete quest;
	delete quest2;
}

TEST_F(CRUDTests, CRUD_QUEST_UPDATE)
{
	auto quest = new ServerQuest(database, "crud_quest_update");
	quest->create_on_database();

	unsigned int id = database->read_int_value(
		database->prepare("SELECT id FROM quest WHERE caption = 'crud_quest_update'"),
		"id");

	auto quest2 = new ServerQuest(database, id);
	quest2->read_on_database();
	quest2->caption = "crud_quest_update_changed";
	quest2->update_on_database();
	delete quest2;

	auto quest3 = new ServerQuest(database, id);
	quest3->read_on_database();

	EXPECT_EQ(quest3->caption, "crud_quest_update_changed");

	delete quest;
	delete quest3;
}

TEST_F(CRUDTests, CRUD_QUEST_DELETE)
{
	auto quest = new ServerQuest(database, "crud_quest_delete");
	quest->create_on_database();

	unsigned int id = database->read_int_value(
		database->prepare("SELECT id FROM quest WHERE caption = 'crud_quest_delete'"),
		"id");

	auto quest2 = new ServerQuest(database, id);
	quest2->delete_on_database();
	delete quest;
	delete quest2;

	try {
		auto quest3 = new ServerQuest(database, id);
		quest3->read_on_database();
		delete quest3;
		FAIL();
	}
	catch (const UnableToReadObjectException&) {
		// Erfolgreich gelöscht
	}
}

