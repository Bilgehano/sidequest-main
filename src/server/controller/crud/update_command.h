#pragma once

#include <string>

#include "network/command_handler.h"

namespace Sidequest
{
	namespace Server
	{
		class Database;

		template<class ModelClass>
		class UpdateCommand: public CommandHandler
		{
		public:
			UpdateCommand(Database* database);
			virtual void execute(const httplib::Request& request, httplib::Response& response) override;

		protected:
			Database* database;
		};

	}
}