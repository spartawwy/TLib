#include "tsystem_request_reload_configs.h"

#include "TLib/core/tsystem_utility_functions.h"
#include "TLib/tool/tsystem_server_client_appbase.h"

using namespace TSystem;

RequestReloadConfigsHandler::RequestReloadConfigsHandler(ServerClientAppBase& app)
	: RequestHandler("reload_configs", "reload resource's configs")
	, app_(app)
{}

void RequestReloadConfigsHandler::HandleRequest(communication::Connection* p, unsigned short sender_pid, GenericRequest& req)
{
	app_.local_logger().LogLocal( utility::FormatStr("receive reload_configs, pid: %d", sender_pid) );
	app_.ReloadConfigs();
}