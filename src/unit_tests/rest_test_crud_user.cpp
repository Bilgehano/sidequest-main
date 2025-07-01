#include <gtest/gtest.h>

#include <iostream>

#include "server_application.h"
#include "storage/database.h"
#include "client_application.h"
#include "network/client_stubs.h"
#include "network/serialisable_user.h"

using namespace Sidequest;

#include "rest_test.h"

namespace Sidequst {

	TEST_F(RestTest, USER_CREATE_THEN_DELETE)
	{
		auto user = new SerialisableUser("test@hs-aalen.de", "REST Test User", "");
		auto id = client->stubs()->create_user(user);

		client->stubs()->delete_user(id);
	}

	TEST_F(RestTest, USER_CREATE_THEN_READ)
	{
		auto user1 = new SerialisableUser("test@hs-aalen.de", "REST Test User", "");
		auto id = client->stubs()->create_user(user1);

		auto user2 = client->stubs()->read_user(id);

		ASSERT_EQ(id, user2->id);
		ASSERT_EQ(user1->display_name, user2->display_name);
		ASSERT_EQ(user1->email, user2->email);

		delete(user1);
		delete(user2);
	}

	TEST_F(RestTest, USER_CREATE_THEN_UPDATE_THEN_READ)
	{
		auto user1 = new SerialisableUser("test@hs-aalen.de", "REST Test User", "");
		auto id = client->stubs()->create_user(user1);

		auto user2 = client->stubs()->read_user(id);

		ASSERT_EQ(id, user2->id);
		ASSERT_EQ(user1->display_name, user2->display_name);
		ASSERT_EQ(user1->email, user2->email);

		user2->display_name = "UPDATED Test User";

		client->stubs()->update_user(user2);

		auto user3 = client->stubs()->read_user(id);

		ASSERT_EQ(id, user3->id);
		ASSERT_EQ("UPDATED Test User", user3->display_name);
		ASSERT_EQ(user1->email, user3->email);

		delete(user1);
		delete(user2);
		delete(user3);
	}

}