#pragma once

#include "read_command.h"

#include <string>

#include "model/server_user.h"
#include "storage/database.h"

namespace Sidequest
{
    namespace Server
    {

        template<class ModelClass>
        ReadCommand<ModelClass>::ReadCommand(Database* database)
            : database(database)
        {
        }

        template<class ModelClass>
        void ReadCommand<ModelClass>::execute(const httplib::Request& request, httplib::Response& response)
        {
            Id object_id = std::stoul( request.path_params.at("id") );
            auto domain_object = new ModelClass(database);

            try {
                domain_object->read_on_database(object_id);
            }
            catch (UnableToReadObjectException& e) 
            {
                response.set_content(Json("no such domain object"), "text/plain");
                response.status = httplib::StatusCode::NotFound_404;
                return;
            }

            std::string object_as_json = domain_object->to_json().dump();
            response.set_content(object_as_json, "text/plain");
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
        template class ReadCommand<ServerUser>;
        template class ReadCommand<ServerQuest>;
    }
}
