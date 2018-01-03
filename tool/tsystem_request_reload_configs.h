#ifndef TSYSTEM_REQUEST_RELOAD_CONFIGS_H_
#define TSYSTEM_REQUEST_RELOAD_CONFIGS_H_

#include "TLib/tool/tsystem_generic_request.h"

namespace TSystem
{
	class ServerClientAppBase;

	class RequestReloadConfigsHandler 
		: public RequestHandler
	{
	public:
		RequestReloadConfigsHandler(ServerClientAppBase& app);

		virtual void HandleRequest(communication::Connection* p, unsigned short sender_pid, GenericRequest& req);

	private:
		ServerClientAppBase& app_;
	};
}

#endif // TSYSTEM_REQUEST_RELOAD_CONFIGS_H_