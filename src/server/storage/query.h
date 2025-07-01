#pragma once  // sorgt dafür, dass die Datei nur einmal eingebunden wird

#include <string>
#include "prepared_statement.h"



namespace Sidequest {
    namespace Server {

        class Database;
        class ColumnCache;

        class Query {
        public:
            // Konstruktor: SQL-Anfrage vorbereiten
            Query(Database* db, const std::string& sql);

            // Destruktor: Statement automatisch freigeben
            ~Query();

            // Werte an die SQL-Abfrage binden (z. B. WHERE email = ?)
            void bind(int index, const std::string& value);
            void bind(int index, unsigned int value);

            // Gehe zur nächsten Zeile im Ergebnis
            bool step();

            // Lese einen Wert aus der aktuellen Zeile
            std::string get_text(const std::string& column_name);
            int get_int(const std::string& column_name);

            // Setzt das Statement zurück (z. B. um es nochmal zu benutzen)
            void reset();
            bool step_done();
        protected:
            Database* database;             // Verbindung zur Datenbank
            PreparedStatement* statement;   // Das eigentliche SQL-Statement
            ColumnCache* column_cache;      // Hilft beim Zugriff auf Spalten
            bool is_done = false;           // Gibt an, ob step() durch ist
            void finalize();                // Interne Aufräumfunktion
        };

    } // namespace Server
} // namespace Sidequest
