#ifndef TSYSTEM_REQUEST_SHUTDOWN_H_
#define TSYSTEM_REQUEST_SHUTDOWN_H_

#include "TLib/tool/tsystem_generic_request.h"

namespace TSystem
{
	class ServerClientAppBase;

	class RequestShutdownHandler 
		: public RequestHandler
	{
	public:
		explicit RequestShutdownHandler(ServerClientAppBase& app);

		virtual void HandleRequest(communication::Connection* p, unsigned short sender_pid, GenericRequest& req);

	private:
		ServerClientAppBase& app_;
	};
}

#endif // TSYSTEM_REQUEST_SHUTDOWN_H_