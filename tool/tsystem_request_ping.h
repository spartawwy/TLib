#ifndef TSYSTEM_REQUEST_PING_H_
#define TSYSTEM_REQUEST_PING_H_

#include "TLib/tool/tsystem_generic_request.h"

namespace TSystem
{
	class ServerClientAppBase;

	class RequestPingHandler 
		: public RequestHandler
	{
	public:

		explicit RequestPingHandler(ServerClientAppBase& app);

		virtual void HandleRequest(communication::Connection* p, unsigned short sender_pid, GenericRequest& req);

	private:
		ServerClientAppBase& app_;
	};
}

#endif // TSYSTEM_REQUEST_PING_H_