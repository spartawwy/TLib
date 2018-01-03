#include "tsystem_request_ping.h"

#include "TLib/core/tsystem_serialization.h"
#include "TLib/core/tsystem_utility_functions.h"
#include "TLib/tool/tsystem_server_client_appbase.h"

using namespace TSystem;

RequestPingHandler::RequestPingHandler(ServerClientAppBase& app)
	: RequestHandler("ping", "ping the resource")
	, app_(app)
{}

void RequestPingHandler::HandleRequest(communication::Connection* p, unsigned short sender_pid, GenericRequest& req)
{
	app_.local_logger().LogLocal( utility::FormatStr("receive ping, pid: %d", sender_pid) );
	if( req.ack() )
	{
		RequestAck ack;
		ack.set_request_id(req.request_id());
		ack.mutable_print()->add_std_out("network connection is ok");
		p->AsyncSend( Encode(ack, app_.msg_system(), Message::HeaderType(0, app_.pid(), sender_pid)) );
	}
}