#pragma once

#include "list_quest_command.h"

#include <string>

#include "storage/database.h"

#include "model/server_user.h"
#include "model/server_quest.h"


namespace Sidequest
{
    namespace Server
    {

        ListQuestCommand::ListQuestCommand(Database* database)
            : database(database)
        {
        }

        void ListQuestCommand::execute(const httplib::Request& request, httplib::Response& response)
        {
            auto query = create_query(request);
            execute_query_and_serialize_result_to_response(response, query);
        }


        void ListQuestCommand::execute_query_and_serialize_result_to_response(httplib::Response& response, Query* query)
        {
            Json result = { };
            query->next_row();
            while (query->has_rows())
            {
                ServerQuest* quest;
                try {
                    quest = new ServerQuest(database);
                    quest->read_from_query(*query);
                }
                catch (UnableToReadObjectException& e)
                {
                    response.set_content(Json("no such domain object"), "text/plain");
                    response.status = httplib::StatusCode::NotFound_404;
                    return;
                }

                result.push_back(quest->to_json());
                query->next_row();
            }

            std::string result_as_json_string = result.dump();
            response.set_content(result_as_json_string, "text/plain");
            response.status = httplib::StatusCode::OK_200;
        }

        QuestsByParentCommand::QuestsByParentCommand(Database* database)
            : ListQuestCommand(database)
        {
        }

        Query* QuestsByParentCommand::create_query(const httplib::Request& request)
        {
            Id parent_id = std::stoul(request.path_params.at("id"));
            auto query = new Query(database, "SELECT * FROM quest WHERE parent = ?;");
            query->bind(1, parent_id);
            return query;
        }

        MainQuestsByOwnerCommand::MainQuestsByOwnerCommand(Database* database)
            : ListQuestCommand(database)
        {
        }

        Query* MainQuestsByOwnerCommand::create_query(const httplib::Request& request)
        {
            Id owner_id = std::stoul(request.path_params.at("id"));
            auto query = new Query(database, "SELECT * FROM quest WHERE parent IS NULL and owner = ?; ");
            query->bind(1, owner_id);
            return query;
        }

    } // namespace Server
} // namespace Sidequest
