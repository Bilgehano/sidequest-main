#include "query.h"

#include <optional>

#include "database.h"
#include "statement_cache.h"
#include "column_cache.h"

namespace Sidequest 
{

	namespace Server 
	{

		Query::Query( Database* database, std::string statement_sql)
			: database(database)
		{
			prepared_statement = database->statement_cache->get_statement(statement_sql);
			if (prepared_statement == nullptr)
			{
				prepared_statement = database->statement_cache->add_statement(statement_sql);
			}
			error_code = INITIAL;
		}

		Query::Query( const Query& other )
			: database(other.database)
			, prepared_statement(other.prepared_statement)
			, error_code(other.error_code)
		{
		}

		Query::~Query()
		{
			reset();
		}

		const Query& Query::operator=(const Query& other)
		{
			database = other.database;
			prepared_statement = other.prepared_statement;
			error_code = other.error_code;
			return *this;
		}

		void Query::bind(int parameter_index, std::string value)
		{
			error_code = sqlite3_bind_text(prepared_statement, parameter_index, value.c_str(), -1, SQLITE_TRANSIENT);
			if (error_code != SQLITE_OK)
				throw ParameterBindException("error binding parameter " + std::to_string(parameter_index) + " to " + value, error_code);
		}

		void Query::bind(int parameter_index, unsigned int value)
		{
			error_code = sqlite3_bind_int(prepared_statement, parameter_index, value);
			if (error_code != SQLITE_OK)
				throw ParameterBindException("error binding parameter " + std::to_string(parameter_index) + " to " + std::to_string(value), error_code);
		}

		void Query::bind_null(int parameter_index)
		{
			error_code = sqlite3_bind_null(prepared_statement, parameter_index);
			if (error_code != SQLITE_OK)
				throw ParameterBindException("error binding parameter " + std::to_string(parameter_index) + " to NULL", error_code);
		}

		int Query::last_insert_rowid()
		{
			return sqlite3_last_insert_rowid( database->handle );
		}

		void Query::next_row()
		{
			error_code = sqlite3_step(prepared_statement);
			n_changes = sqlite3_changes(database->handle);
		}

		std::optional<long> Query::optional_int_value(std::string column_name)
		{
			int column_index = database->column_cache->get_column_index(prepared_statement, column_name);
			if (sqlite3_column_type(prepared_statement, column_index) == SQLITE_NULL)
				return std::nullopt;
			int result = static_cast<long>(sqlite3_column_int64(prepared_statement, column_index));
			return result;
		}

		long Query::int_value(std::string column_name)
		{
			int column_index = database->column_cache->get_column_index(prepared_statement, column_name);
			int result = static_cast<long>(sqlite3_column_int64(prepared_statement, column_index));
			return result;
		}

		std::string Query::text_value(std::string column_name)
		{
			int column_index = database->column_cache->get_column_index(prepared_statement, column_name);
			auto c_str = reinterpret_cast<const char*>(sqlite3_column_text(prepared_statement, column_index));
			std::string result(c_str);
			return result;
		}

		bool Query::has_rows()
		{
			return error_code == SQLITE_ROW;
		}

		bool Query::is_done()
		{
			return error_code == SQLITE_DONE;
		}

		unsigned int Query::changes()
		{
			return n_changes;

		}

		void Query::reset()
		{
			sqlite3_reset(prepared_statement);
		}
	}

}