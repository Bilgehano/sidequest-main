#pragma once

#include <string>

#include "httplib.h"

namespace Sidequest
{
	namespace Client
	{
		typedef httplib::Request Request;
		typedef httplib::Response Response;

		class Stubs;

		class ClientApplication
		{
		public:
			ClientApplication( std::string hostname, int port );
			virtual ~ClientApplication();

			virtual Stubs* stubs();
			httplib::Client& http_connection();

		protected:
			Stubs* _stubs = nullptr;
			httplib::Client _http_client;
		};

	}
}