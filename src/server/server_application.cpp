#include <server_application.h>

#include <iostream>
#include <chrono>
#include <thread>

#include "storage/database.h"
#include "network/connection_handler.h"

#include "model/server_user.h"
#include "model/server_quest.h"

#include "controller/crud/read_command.h"
#include "controller/crud/create_command.h"
#include "controller/crud/update_command.h"
#include "controller/crud/delete_command.h"

#include "view/list_quest_command.h"

namespace Sidequest
{
	namespace Server
	{

		ServerApplication::ServerApplication(std::string hostname, int port)
		{
			connection_handler = new ConnectionHandler( hostname, port );
			database = new Sidequest::Server::Database("sidequest.db");
			owns_database = true;
			setup_commands();
		}

		ServerApplication::ServerApplication(std::string hostname, int port, Database* database)
			: database(database)
		{
			connection_handler = new ConnectionHandler(hostname, port);
			owns_database = false;
			setup_commands();
		}

		ServerApplication::~ServerApplication()
		{
			if (owns_database)
				delete(database);
			delete(connection_handler);
		}

		void ServerApplication::run()
		{
			connection_handler->listen();
		}

		void ServerApplication::stop()
		{
			connection_handler->stop();
		}

		void ServerApplication::setup_commands()
		{
			connection_handler->register_put_command("/api/user/create", new Sidequest::Server::CreateCommand<ServerUser>(database));
			connection_handler->register_get_command("/api/user/:id/read", new Sidequest::Server::ReadCommand<ServerUser>(database));
			connection_handler->register_put_command("/api/user/:id/update", new Sidequest::Server::UpdateCommand<ServerUser>(database));
			connection_handler->register_delete_command("/api/user/:id/delete", new Sidequest::Server::DeleteCommand<ServerUser>(database));

			connection_handler->register_put_command("/api/quest/create", new Sidequest::Server::CreateCommand<ServerQuest>(database));
			connection_handler->register_get_command("/api/quest/:id/read", new Sidequest::Server::ReadCommand<ServerQuest>(database));
			connection_handler->register_put_command("/api/quest/:id/update", new Sidequest::Server::UpdateCommand<ServerQuest>(database));
			connection_handler->register_delete_command("/api/quest/:id/delete", new Sidequest::Server::DeleteCommand<ServerQuest>(database));

			connection_handler->register_get_command("/api/quest/byparent/:id/", new Sidequest::Server::QuestsByParentCommand(database));
			connection_handler->register_get_command("/api/quest/byowner/:id/", new Sidequest::Server::MainQuestsByOwnerCommand(database));
		}

	}
}