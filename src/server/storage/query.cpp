#include "query.h"

#include "database.h"
#include "statement_cache.h"
#include "column_cache.h"


#include <stdexcept>

namespace Sidequest {
    namespace Server {

        // Konstruktor: Bereitet das SQL-Statement vor
        Query::Query(Database* db, const std::string& sql)
            : database(db), column_cache(db->column_cache)
        {
            statement = database->statement_cache->get_statement(sql);

            if (statement == nullptr) {
                statement = database->statement_cache->add_statement(sql);
            }

            is_done = false;
        }

        // Destruktor: Gibt Ressourcen automatisch frei
        Query::~Query()
        {
            finalize();
        }

        // Gibt das Statement frei
        void Query::finalize()
        {
            if (statement) {
                sqlite3_finalize(statement);
                statement = nullptr;
            }
        }

        // Bindet einen Text-Wert an eine SQL-Variable (z. B. WHERE email = ?)
        void Query::bind(int index, const std::string& value)
        {
            int code = sqlite3_bind_text(statement, index, value.c_str(), -1, SQLITE_TRANSIENT);
            if (code != SQLITE_OK) {
                throw std::runtime_error("Fehler beim Binden von String");
            }
        }

        // Bindet einen int-Wert (z. B. eine ID)
        void Query::bind(int index, unsigned int value)
        {
            int code = sqlite3_bind_int(statement, index, value);
            if (code != SQLITE_OK) {
                throw std::runtime_error("Fehler beim Binden von int");
            }
        }

        // Führt einen Schritt aus → springt zur nächsten Zeile
        bool Query::step()
        {
            int code = sqlite3_step(statement);
            if (code == SQLITE_ROW) {
                return true; // neue Zeile vorhanden
            }
            else if (code == SQLITE_DONE) {
                is_done = true;
                return false; // keine weiteren Zeilen
            }
            else {
                throw std::runtime_error("Fehler beim Ausführen von step()");
            }
        }

        // Holt einen Textwert aus der aktuellen Zeile
        std::string Query::get_text(const std::string& column_name)
        {
            int index = column_cache->get_column_index(statement, column_name);
            const char* value = reinterpret_cast<const char*>(sqlite3_column_text(statement, index));
            return std::string(value ? value : "");
        }

        // Holt eine Ganzzahl aus der aktuellen Zeile
        int Query::get_int(const std::string& column_name)
        {
            int index = column_cache->get_column_index(statement, column_name);
            return sqlite3_column_int(statement, index);
        }

        // Setzt das Statement zurück (für erneute Verwendung)
        void Query::reset()
        {
            sqlite3_reset(statement);
            is_done = false;
        }



        bool Query::step_done()
        {
            int code = sqlite3_step(statement);
            if (code == SQLITE_DONE) {
                is_done = true;
                return true;
            }
            else {
                return false;
            }
        }

    } // namespace Server
} // namespace Sidequest
