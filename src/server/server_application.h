#pragma once

#include <string>

namespace Sidequest
{
	namespace Server
	{
		class Database;
		class ConnectionHandler;

		class ServerApplication
		{
		public:
			ServerApplication(std::string hostname, int port);
			ServerApplication(std::string hostname, int port, Database* database);
			~ServerApplication();

			void run();
			void stop();

		protected:
			void setup_commands();

		protected:
			Database* database;
			bool owns_database;
			ConnectionHandler* connection_handler;
		};

	}
}
