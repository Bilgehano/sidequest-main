#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <sqlite3.h>

#include "prepared_statement.h"
#include "query.h"

namespace Sidequest
{
	namespace Server
	{
		class Persistable;
		class StatementCache;
		class ColumnCache;

		class DatabaseNotFoundException : public std::runtime_error
		{
		public:
			explicit DatabaseNotFoundException(const std::string& message);
		};

		class ParameterBindException : public std::runtime_error
		{
		public:
			explicit ParameterBindException(const std::string& statement, int error_code);
			int error_code;
		};

		class UnableToReadObjectException : public std::runtime_error
		{
		public:
			explicit UnableToReadObjectException(const std::string& key);
		};

		class UnableToCreateObjectException : public std::runtime_error
		{
		public:
			explicit UnableToCreateObjectException(const std::string& key);
		};

		class UnableToUpdateObjectException : public std::runtime_error
		{
		public:
			explicit UnableToUpdateObjectException(const std::string& key);
		};

		class UnableToDeleteObjectException : public std::runtime_error
		{
		public:
			explicit UnableToDeleteObjectException(const std::string& key);
		};

		class Database
		{
		public:
			Database( std::string url );
			~Database();

			int execute_sql_statement(std::string sql_statement);

			StatementCache* statement_cache;
			ColumnCache* column_cache;

		protected:
			void open( std::string url );
			void close();

		protected:
			bool is_open = false;
			sqlite3* handle = nullptr;

			friend class StatementCache;
			friend class Query;
		};

	}
}