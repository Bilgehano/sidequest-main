#include <gtest/gtest.h>

#include "storage/database.h"
#include "model/server_user.h"

class CRUDTestsUser : public ::testing::Test 
{
protected:
	Sidequest::Server::Database* database;

	CRUDTestsUser()
	{
	}

	virtual ~CRUDTestsUser() {
	}

	virtual void SetUp() {
		database = new Sidequest::Server::Database(":memory:");
		database->execute_sql_statement("CREATE TABLE user(id INTEGER PRIMARY KEY AUTOINCREMENT, email TEXT unique, display_name TEXT, password TEXT);");
	}

	virtual void TearDown() {
		delete database;
	}
};

using namespace Sidequest::Server;

TEST_F(CRUDTestsUser, OPEN_DATABASE)
{
}

TEST_F(CRUDTestsUser, CRUD_USER_CREATE)
{
	auto user = new ServerUser( database, "crud_user_create@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	auto id = user->id;
	delete(user);

	auto user2 = new ServerUser(database);
	user2->read_on_database(id);

	EXPECT_EQ(user2->display_name, "Temporary User");
	delete(user2);
}

TEST_F(CRUDTestsUser, CRUD_USER_CREATE_DOUBLE)
{
	auto user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	try {
		user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User 2", "");
		user->create_on_database();
		FAIL();
	}
	catch (const UnableToCreateObjectException& expected) {
		delete(user);
	}
}

TEST_F(CRUDTestsUser, CRUD_USER_READ)
{
	auto user = new ServerUser(database, "crud_user_read@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	auto id = user->id;
	delete(user);

	user = new ServerUser(database);
	user->read_on_database(id);

	EXPECT_EQ(user->display_name, "Temporary User");
}

TEST_F(CRUDTestsUser, CRUD_USER_UPDATE)
{
	auto user = new ServerUser(database, "crud_user_update@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	auto id = user->id;
	user->display_name = "Changed Display Name";
	user->update_on_database();
	delete(user);

	auto user2 = new ServerUser(database);
	user2->read_on_database(id);

	EXPECT_EQ(user2->display_name, "Changed Display Name");
	delete(user2);
}

TEST_F(CRUDTestsUser, CRUD_USER_DELETE)
{
	auto user = new ServerUser(database, "crud_user_delete@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	auto id = user->id;
	delete(user);

	auto user2 = new ServerUser(database, id);
	user2->delete_on_database();
	delete(user2);

	auto user3 = new ServerUser(database);
	try {
		user3->read_on_database(id);
		FAIL();
	}
	catch (const UnableToReadObjectException& expected)
	{
		delete(user3);
	}
}

TEST_F(CRUDTestsUser, CRUD_USER_DELETE_NON_EXISTENT)
{
	auto user = new ServerUser(database, 15);

	try {
		user->delete_on_database();
		FAIL();
	}
	catch (const UnableToDeleteObjectException& expected) {
		delete(user);
	}
}
