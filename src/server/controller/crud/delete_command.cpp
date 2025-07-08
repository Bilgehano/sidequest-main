#pragma once

#include "delete_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest
{
    namespace Server
    {

        template<class ModelClass>
        DeleteCommand<ModelClass>::DeleteCommand(Database* database)
            : database(database)
        {
        }

        template<class ModelClass>
        void DeleteCommand< ModelClass>::execute(const httplib::Request& request, httplib::Response& response)
        {
            Id object_id = std::stoul(request.path_params.at("id"));
            auto model_object = new ModelClass(database);
            model_object->id = object_id;

            try {
                model_object->delete_on_database();
            }
            catch (UnableToCreateObjectException& e) 
            {
                response.set_content(Json("unable to delete model_object"), "text/plain");
                response.status = httplib::StatusCode::BadRequest_400;
                return;
            }

            response.set_content("", "text/plain");
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
        template class DeleteCommand<ServerUser>;
        template class DeleteCommand<ServerQuest>;
    }
}