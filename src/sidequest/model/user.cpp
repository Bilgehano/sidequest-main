#include "user.h"

#include <stdlib.h>

namespace Sidequest 
{

	User::User()
	{
	}

	User::User(Id id)
		:id(id)
	{
	}

	User::User(std::string email, std::string display_name, std::string password)
		: id(0), email(email), display_name(display_name), password(password)
	{
	}

	User::~User()
	{
	}

}
