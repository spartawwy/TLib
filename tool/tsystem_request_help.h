#ifndef TSYSTEM_REQUEST_HELP_H_
#define TSYSTEM_REQUEST_HELP_H_

#include "TLib/tool/tsystem_generic_request.h"

namespace TSystem
{
	class ServerClientAppBase;

	class RequestHelpHandler 
		: public RequestHandler
	{
	public:
		RequestHelpHandler(ServerClientAppBase& app, const communication::GenericRequestKernel& kernel); 

		virtual void HandleRequest(communication::Connection* p, unsigned short sender_pid, GenericRequest& req);

	private:
		ServerClientAppBase& app_;
		const communication::GenericRequestKernel& kernel_; 
	};
}

#endif // TSYSTEM_REQUEST_HELP_H_