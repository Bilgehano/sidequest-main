#pragma once

#include <string>

#include "network/command_handler.h"

namespace Sidequest
{
	namespace Server
	{
		class Database;
		class Query;

		class ListQuestCommand : public CommandHandler
		{
		public:
			ListQuestCommand(Database* database);
			virtual void execute(const httplib::Request& request, httplib::Response& response) override;

		protected:
			virtual Query* create_query(const httplib::Request& request) = 0;
			void execute_query_and_serialize_result_to_response(httplib::Response& response, Query* query);

			Database* database;
		};

		class QuestsByParentCommand : public ListQuestCommand
		{
		public:
			QuestsByParentCommand(Database* database);
		protected:
			Query* create_query(const httplib::Request& request) override;
		};

		class MainQuestsByOwnerCommand : public ListQuestCommand
		{
		public:
			MainQuestsByOwnerCommand(Database* database);
		protected:
			Query* create_query(const httplib::Request& request) override;
		};

	}
}