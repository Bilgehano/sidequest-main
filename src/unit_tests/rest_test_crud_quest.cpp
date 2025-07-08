#include <gtest/gtest.h>

#include <iostream>

#include "server_application.h"
#include "storage/database.h"
#include "client_application.h"
#include "network/client_stubs.h"
#include "network/serialisable_quest.h"

using namespace Sidequest;

#include "rest_test.h"

namespace Sidequst {

	TEST_F(RestTest, QUEST_CREATE_THEN_DELETE)
	{
		auto quest = new SerialisableQuest(Quest::initial, "REST Test Quest", "", nullptr, nullptr, nullptr);
		auto id = client->stubs()->create_quest(quest);

		client->stubs()->delete_quest(id);
	}

	TEST_F(RestTest, QUEST_CREATE_THEN_READ)
	{
		auto quest1 = new SerialisableQuest(Quest::initial, "REST Test Quest", "", nullptr, nullptr, nullptr);
		auto id = client->stubs()->create_quest(quest1);

		auto quest2 = client->stubs()->read_quest(id);

		ASSERT_EQ(id, quest2->id);
		ASSERT_EQ(quest1->description, quest2->description);
		ASSERT_EQ(quest1->status, quest2->status);

		delete(quest1);
		delete(quest2);
	}

}