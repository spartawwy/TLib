#include "tsystem_generic_request.h"

#include "TLib/core/tsystem_core_error.h"
#include "TLib/core/tsystem_message_system.h"
#include "TLib/core/tsystem_serialization.h"

using namespace TSystem;
using namespace TSystem::communication;

GenericRequestKernel::GenericRequestKernel(const MessageSystem& msg_system)
	: msg_system_(msg_system)
	, handlers_(512)
{}

void GenericRequestKernel::RegisterHandler(std::unique_ptr<RequestHandler>&& handler)
{
	assert( handler );
	assert(msg_system_.FindClassID("GenericRequest", std::nothrow));

	auto iter = handlers_.find(handler->name());
	if( iter != handlers_.end() )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "GenericRequestKernel::RegisterHandler"
			, "Handler already registered: " + handler->name());

	handlers_.insert(std::make_pair(handler->name(), std::move(handler)));
}

void GenericRequestKernel::HandleMessage(Connection* p, const Message& msg)
{
	GenericRequest req;
	if( TSystem::Decode(msg, req, msg_system_) )
	{
		auto iter = handlers_.find(req.req_name());
		if( iter != handlers_.end() )
			iter->second->HandleRequest(p, msg.SenderPid(), req);
	}
}