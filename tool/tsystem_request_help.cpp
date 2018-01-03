#include "tsystem_request_help.h"

#include "TLib/core/tsystem_serialization.h"
#include "TLib/core/tsystem_utility_functions.h"
#include "TLib/tool/tsystem_server_client_appbase.h"

using namespace TSystem;

RequestHelpHandler::RequestHelpHandler(ServerClientAppBase& app, const communication::GenericRequestKernel& kernel)
	: RequestHandler("help", "print supported command list")
	, app_(app)
	, kernel_(kernel)
{}

void RequestHelpHandler::HandleRequest(communication::Connection* p, unsigned short sender_pid, GenericRequest& req)
{
	app_.local_logger().LogLocal( utility::FormatStr("receive help, pid: %d", sender_pid) );
	if( req.ack() )
	{
		RequestAck ack;
		ack.set_request_id(req.request_id());
		for( const auto& handler : kernel_.handlers() )
			ack.mutable_print()->add_std_out( handler.second->name() + " : " + handler.second->info() );
		std::sort(ack.mutable_print()->mutable_std_out()->begin(), ack.mutable_print()->mutable_std_out()->end());
		p->AsyncSend( Encode(ack, app_.msg_system(), Message::HeaderType(0, app_.pid(), sender_pid)) );
	}
}