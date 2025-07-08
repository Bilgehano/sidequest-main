#pragma once

#include <string>
#include <vector>
#include <optional>
#include <sqlite3.h>

#include "prepared_statement.h"

#define INITIAL -1

/* Base class of all persistent domain model objects on the server
*  that implement CRUDS operations */
namespace Sidequest
{
	namespace Server
	{
		class Database;

		/* All persistent classes should derive from Persistable */
		class Query
		{
		public:
			Query(Database* database, std::string statement_sql);
			Query(const Query& other);
			virtual ~Query();

			const Query& operator=(const Query& other);

			void bind(int parameter_index, std::string value);
			void bind(int parameter_index, unsigned int value);
			void bind_null(int parameter_index);
			int last_insert_rowid();

			void next_row();

			long int_value(std::string column_name);
			std::optional<long> optional_int_value(std::string column_name);
			std::string text_value(std::string column_name);

			bool has_rows();
			bool is_done();
			unsigned int changes();

		protected:
			void reset();

		protected:
			Database* database;
			PreparedStatement* prepared_statement;
			int error_code;
			unsigned int n_changes;
		};

	}
}