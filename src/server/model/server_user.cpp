#include "server_user.h"

#include "storage/database.h"

#include <iostream>

namespace Sidequest 
{
	namespace Server 
	{
	
		ServerUser::ServerUser(Database* database)
			: Persistable(database)
			, SerialisableUser()
		{
		}

		ServerUser::ServerUser(Database* database, Id id)
			: Persistable(database)
			, SerialisableUser(id)
		{
		}

		ServerUser::ServerUser(Database* database, std::string email, std::string display_name, std::string password)
			: Persistable(database)
			, SerialisableUser(email, display_name, password)
		{
		}

		ServerUser::~ServerUser()
		{
		}

		void ServerUser::create_on_database()
		{
			auto query = Query(database, "INSERT INTO user(email, display_name, password) VALUES (?, ?, ?);" );
			query.bind( 1, email );
			query.bind( 2, display_name );
			query.bind( 3, password );
			query.next_row();
			if (!query.is_done())
				throw UnableToCreateObjectException(email);
			id = query.last_insert_rowid();
		}

		void ServerUser::read_on_database(Id id)
		{
			auto query = Query(database, "SELECT * FROM user WHERE id = ?;");
			query.bind( 1, id );
			query.next_row();
			if ( ! query.has_rows() )
				throw UnableToReadObjectException(email);
			this->id = id;
			read_from_query( query );
		}

		void ServerUser::update_on_database()
		{
			auto query = Query(database, "UPDATE user set email=?, display_name=?, password=? WHERE id=?;");
			query.bind( 1, email);
			query.bind( 2, display_name);
			query.bind( 3, password);
			query.bind( 4, id);
			query.next_row();
			if (! query.is_done() )
				throw UnableToUpdateObjectException(email);
		}

		void ServerUser::delete_on_database()
		{
			auto query = Query(database, "DELETE FROM user WHERE id=?;");
			query.bind( 1, id );
			query.next_row();
			if (!query.is_done())
				throw UnableToDeleteObjectException(email);
			if (query.changes() != 1)
				throw UnableToDeleteObjectException(email);
		}

		void ServerUser::read_from_query( Query& query )
		{
			display_name = query.text_value("display_name");
			email = query.text_value("email");
			password = query.text_value("password");
		}

		std::string ServerUser::class_id()
		{
			return "user";
		}

	}
}