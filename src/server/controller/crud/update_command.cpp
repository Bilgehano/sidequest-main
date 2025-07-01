#pragma once

#include "update_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest
{
    namespace Server
    {

        template<class ModelClass>
        UpdateCommand<ModelClass>::UpdateCommand(Database* database)
            : database(database)
        {
        }

        template<class ModelClass>
        void UpdateCommand<ModelClass>::execute(const httplib::Request& request, httplib::Response& response)
        {
            auto json = Json::parse(request.body);
            auto model_object = new ModelClass(database);
            model_object->from_json(json);

            try {
                model_object->update_on_database();
            }
            catch (UnableToCreateObjectException& e) 
            {
                response.set_content(Json("unable to update model_object"), "text/plain");
                response.status = httplib::StatusCode::BadRequest_400;
                return;
            }

            response.set_content(Json().dump(), "text/plain");
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
        template class UpdateCommand<ServerUser>;
        template class UpdateCommand<ServerQuest>;
    }
}
