#pragma once

#include <string>

#include "httplib.h"

namespace Sidequest
{
	namespace Server
	{
		typedef httplib::Request Request;
		typedef httplib::Response Response;

		class CommandHandler
		{
		public:
			virtual void execute(const Request& request, Response& response) = 0;

			std::function<void(const Request&, Response&)> get_function();
		};

	}
}