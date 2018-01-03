#include "tsystem_request_shutdown.h"

#include "TLib/core/tsystem_utility_functions.h"
#include "TLib/tool/tsystem_server_client_appbase.h"

using namespace TSystem;

RequestShutdownHandler::RequestShutdownHandler(ServerClientAppBase& app)
	: RequestHandler("shutdown", "shutdown the resource")
	, app_(app)
{}

void RequestShutdownHandler::HandleRequest(communication::Connection* p, unsigned short sender_pid, GenericRequest& req)
{
	app_.local_logger().LogLocal( utility::FormatStr("receive shutdown, pid: %d", sender_pid) );
	app_.Shutdown();
}