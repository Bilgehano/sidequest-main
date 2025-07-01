#include "json_serialisable.h"

namespace Sidequest 
{

	Json JsonSerialisable::serialize_loadable_pointer(JsonSerialisable* ptr, std::optional<Id> id, bool full_serialise)
	{
		if (!id.has_value())
			return Json();

		if (!full_serialise)
			return Json(id.value());

		if (ptr == nullptr)
			return Json(id.value());

		return ptr->to_json();
	}

}