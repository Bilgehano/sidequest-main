#include <gtest/gtest.h>

#include <iostream>

namespace Sidequest 
{
	class SerialisableUser;
	class SerialisableQuest;

	namespace Server {
		class Database;
		class ServerApplication;
	}
	namespace Client {
		class ClientApplication;
	}

	class RestTest : public ::testing::Test
	{
	protected:

		RestTest();

		virtual ~RestTest();

		virtual void SetUp();

		virtual void TearDown();

		SerialisableQuest* create_subquest(SerialisableQuest* parent_quest, SerialisableUser* owner);

		Server::Database* database;
		Server::ServerApplication* server;
		std::thread thread;
		Client::ClientApplication* client;
	};

}