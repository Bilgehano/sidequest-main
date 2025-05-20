#include "server_user.h"
#include "storage/query.h"
#include "storage/database.h"

namespace Sidequest
{
    namespace Server {

        // Standard-Konstruktor
        ServerUser::ServerUser(Database* database)
            : Persistable(database)
        {
        }

        // Konstruktor mit nur E-Mail
        ServerUser::ServerUser(Database* database, std::string email)
            : Persistable(database)
            , User(email)
        {
        }

        //Fehlender Konstruktor mit allen Feldern
        ServerUser::ServerUser(Database* database, std::string email, std::string display_name, std::string password)
            : Persistable(database)
            , User(email, display_name, password)
        {
        }

        // Destruktor
        ServerUser::~ServerUser()
        {
        }

        // CREATE
        void ServerUser::create_on_database()
        {
            Query query(database, "INSERT INTO user(email, display_name, password) VALUES (?, ?, ?);");
            query.bind(1, email);
            query.bind(2, display_name);
            query.bind(3, password);

            if (!query.step_done()) {
                throw UnableToCreateObjectException(email);
            }
        }

        // READ
        void ServerUser::read_on_database()
        {
            Query query(database, "SELECT * FROM user WHERE email = ?;");
            query.bind(1, email);




            if (!query.step()) {
                throw UnableToReadObjectException(email);
            }

            display_name = query.get_text("display_name");
            password = query.get_text("password");
        }

        // UPDATE
        void ServerUser::update_on_database()
        {
            Query query(database, "UPDATE user SET display_name = ?, password = ? WHERE email = ?;");
            query.bind(1, display_name);
            query.bind(2, password);
            query.bind(3, email);

            if (!query.step_done()) {
                throw UnableToUpdateObjectException(email);
            }
        }

        // DELETE
        void ServerUser::delete_on_database()
        {
            Query query(database, "DELETE FROM user WHERE email = ?;");
            query.bind(1, email);

            if (!query.step_done()) {
                throw UnableToDeleteObjectException(email);
            }
        }

        // ID-Kennung für Typ
        std::string ServerUser::class_id()
        {
            return "user";
        }

    }
}
