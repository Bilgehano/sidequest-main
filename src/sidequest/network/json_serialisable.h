#pragma once

#include <optional>

#include <json.hpp>
typedef nlohmann::json Json;

#include "model/id.h"

namespace Sidequest
{

	/* All classes that can be transferred via network should implement this interface */
	class JsonSerialisable
	{
	public:
		virtual Json to_json( bool full_serialise = true ) = 0;
		virtual void  from_json( const Json& json ) = 0;

		Json serialize_loadable_pointer(JsonSerialisable* ptr, std::optional<Id> id, bool full_serialise);

		template<class T>
		T* deserialize_loadable_pointer(const Json& json, std::optional<Id>& id)
		{
			if (json.is_null())
			{
				id = std::nullopt;
				return nullptr;
			}
			if (json.is_number_unsigned())
			{
				Id _id;
				json.get_to(_id);
				id = _id;
				return nullptr;
			}
			if ( json.is_object() )
			{
				T* object = new T( );
				object->from_json( json );
				id = object->id;
				return object;
			}
			throw std::runtime_error("illegal json object");
		}
	};

}