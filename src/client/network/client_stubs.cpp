#pragma once

#include "client_stubs.h"

#include "httplib.h"

#include <string>
#include <iostream>

#include <json.hpp>
using json = nlohmann::json;

#include "network/serialisable_user.h"
#include "network/serialisable_quest.h"

namespace Sidequest
{
    namespace Client
    {

        RemoteCallFailedException::RemoteCallFailedException(const std::string& message)
            : runtime_error( message )
        {
        }

        RemoteCallFailedException::RemoteCallFailedException(const httplib::Error& error_code)
            : runtime_error(httplib::to_string( error_code ) )
        {
        }

        Stubs::Stubs(httplib::Client&http_connection)
            : _http_client( http_connection )
        {
        }

        Stubs::~Stubs()
        {
        }

        Id Stubs::create_user(SerialisableUser* user)
        {
            std::string body = user->to_json().dump();
            auto result = _http_client.Put("/api/user/create", body, "text/plain");
            if (result.error() != httplib::Error::Success)
                throw RemoteCallFailedException( result.error() );
            if (result->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("create user failed");

            auto json_response = Json::parse(result.value().body); 

            Id id = json_response.at("id");
            user->id = id;
            return id;
        }

        SerialisableUser* Stubs::read_user(Id id)
        {
            std::string id_string = std::to_string(id);
            auto response = _http_client.Get("/api/user/" + id_string + "/read");
            if (response->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("read user failed");
            auto json = Json::parse(response->body);
            SerialisableUser* user = new SerialisableUser();
            user->from_json(json);
            return user;
        }

        void Stubs::update_user(SerialisableUser* user)
        {
            std::string body = user->to_json().dump();
            std::string id_string = std::to_string( user->id );
            auto response = _http_client.Put("/api/user/" + id_string + "/update", body, "text/plain");
            if (response->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("update user failed");
        }

        void Stubs::delete_user(Id id)
        {
            std::string id_string = std::to_string(id);
            auto response = _http_client.Delete("/api/user/" + id_string + "/delete");
            if (response->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("read user failed");
        }

        Id Stubs::create_quest(SerialisableQuest* quest)
        {
            std::string body = quest->to_json().dump();
            auto response = _http_client.Put("/api/quest/create", body, "text/plain");
            if (response.error() != httplib::Error::Success)
                throw RemoteCallFailedException(response.error());
            if (response->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("create quest failed");

            auto json_response = Json::parse(response.value().body);

            Id id = json_response.at("id");
            quest->id = id;
            return id;
        }

        SerialisableQuest* Stubs::read_quest(Id id)
        {
            std::string id_string = std::to_string(id);
            auto response = _http_client.Get("/api/quest/" + id_string + "/read");
            if (response->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("read quest failed");
            auto json = Json::parse(response->body);
            SerialisableQuest* quest = new SerialisableQuest();
            quest->from_json(json);
            return quest;
        }

        void Stubs::update_quest(SerialisableQuest* quest)
        {
            std::string body = quest->to_json().dump();
            std::string id_string = std::to_string(quest->id);
            auto response = _http_client.Put("/api/quest/" + id_string + "/update", body, "text/plain");
            if (response->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("update quest failed");
        }

        void Stubs::delete_quest(Id id)
        {
            std::string id_string = std::to_string(id);
            auto response = _http_client.Delete("/api/quest/" + id_string + "/delete");
            if (response->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("read user failed");
        }

        std::list<SerialisableQuest*> Stubs::quests_by_parent(Id id)
        {
            auto result = std::list<SerialisableQuest*>();
            std::string id_string = std::to_string(id);
            auto response = _http_client.Get("/api/quest/byparent/" + id_string + "/");
            if (response->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("get quest by parent failed");
            auto json_quest_list = Json::parse(response->body);
            for (auto& json_quest : json_quest_list)
            {
                SerialisableQuest* quest = new SerialisableQuest();
                quest->from_json(json_quest);
                result.push_back(quest);
            }
            return result;
        }
      
        std::list<SerialisableQuest*> Stubs::quests_by_owner(Id id)
        {
            auto result = std::list<SerialisableQuest*>();
            std::string id_string = std::to_string(id);
            auto response = _http_client.Get("/api/quest/byowner/" + id_string + "/");
            if (response->status != httplib::StatusCode::OK_200)
                throw RemoteCallFailedException("get quest by parent failed");
            auto json_quest_list = Json::parse(response->body);
            for (auto& json_quest : json_quest_list)
            {
                SerialisableQuest* quest = new SerialisableQuest();
                quest->from_json(json_quest);
                result.push_back(quest);
            }
            return result;
        }


    } // namespace Server
} // namespace Sidequest