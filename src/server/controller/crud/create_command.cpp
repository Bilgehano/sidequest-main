#pragma once

#include "create_command.h"

#include <string>

#include "storage/database.h"

#include "model/server_user.h"
#include "model/server_quest.h"


namespace Sidequest
{
    namespace Server
    {

        template<class ModelClass>
        CreateCommand<ModelClass>::CreateCommand(Database* database)
            : database(database)
        {
        }

        template<class ModelClass>
        void CreateCommand<ModelClass>::execute(const httplib::Request& request, httplib::Response& response)
        {
            auto json = Json::parse(request.body);
            auto model_object = new ModelClass(database);
            model_object->from_json(json);

            try {
                model_object->create_on_database();
            }
            catch (UnableToCreateObjectException& e) 
            {
                response.set_content(Json("unable to create model class"), "text/plain");
                response.status = httplib::StatusCode::BadRequest_400;
                return;
            }

            Json json_response;
            json_response["id"] = model_object->id;
            response.set_content(json_response.dump(), "text/plain");
            response.status = httplib::StatusCode::OK_200;
        }

    } // namespace Server
} // namespace Sidequest

#include "model/server_user.h"
#include "model/server_quest.h"

namespace Sidequest
{
    namespace Server
    {
        template class CreateCommand<ServerUser>;
        template class CreateCommand<ServerQuest>;
    }
}