#include "tsystem_client_socket.h"

#include <iostream>

#include "TLib/core/tsystem_core_common.h"
#include "TLib/tool/tsystem_server_client_appbase.h"

using namespace TSystem;

ClientSocket::ClientSocket(ServerClientAppBase& app)
	: app_(app)
	, on_handshake_handler_(), on_disconnect_handler_(), on_abort_handler_()
	, msg_handlers_(app_.msg_system())
{}

ClientSocket::~ClientSocket()
{}

void ClientSocket::on_handshake_handler(OnHandshakeHandlerType&& handler )
{
	on_handshake_handler_ = std::move(handler); 

	// register handshake
	if( !msg_handlers_.HasHandler("HandShake") )
	{
		msg_handlers_.RegisterHandler("HandShake", [this](communication::Connection* p, const Message& msg)
		{
			this->app_.local_logger().LogLocal( utility::FormatStr("receive handshake from connection: %d", p->connid()));
			if( this->on_handshake_handler_ )
				this->on_handshake_handler_(p, msg);
		});
	}
}

void ClientSocket::AsyncConnect(const std::string& res_name, const ConnectingSetting& setting)
{
	const layinfo::AppResource& res = app_.resource_manager().FindUnique(res_name);

	if( !res )
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT, "ClientSocket::AsyncConnect"
			, "No resource found: " + res_name);

	AsyncConnect(res, setting, 1);
}

void ClientSocket::AsyncConnect(const layinfo::AppResource& res, const ConnectingSetting& setting)
{
	AsyncConnect(res, setting, 1);
}

void ClientSocket::AsyncConnect(const layinfo::AppResource& res, const ConnectingSetting& setting, int try_num )
{
	app_.local_logger().LogLocal( utility::FormatStr("%d try to connect to resource: %s", try_num, res.name.c_str()));
	app_.AsyncConnectResource(res, 
		[res, setting, try_num, this](std::shared_ptr<communication::Connection>& pconn, const TError& te)
		{
			this->SetupConnection(res, setting, try_num, pconn, te);
            if( !te )
                this->msg_handlers().LinkConnection(pconn);
		});
}

void ClientSocket::SetupConnection(const layinfo::AppResource& res, const ConnectingSetting& setting, int try_num
			, std::shared_ptr<communication::Connection>& pconn, const TError& te)
{
	if( !te )
	{
		if( pconn ) // if not shutdown
		{
			app_.local_logger().LogLocal( utility::FormatStr("connect to resource success on %d try: %s, connid %d"
				, try_num, res.name.c_str(), pconn->connid()) );

			std::weak_ptr<communication::Connection> weak_pconn(pconn);
			
			// setup on_connect, sending handshake
			pconn->event_handlers().on_connect = [weak_pconn, this, res]()
			{
				std::shared_ptr<communication::Connection> p = weak_pconn.lock();
				if( p )
				{
					app_.local_logger().LogLocal( utility::FormatStr("sending handshake to resource: %s, connid %d"
						, res.name.c_str(), p->connid()) );
                    std::cout << utility::FormatStr("sending handshake to resource: %s, connid %d"
						, res.name.c_str(), p->connid());
					SendHandShake(app_, p);
				}
			};
			//on_handshake_handler(nullptr);
			// link msg_handlers = setup on_msg_recv
			//msg_handlers_.LinkConnection(pconn);

			// setup on_disconnect
			pconn->event_handlers().on_disconnect = [weak_pconn, res, setting, this](const TError& te)
			{
				std::shared_ptr<communication::Connection> p = weak_pconn.lock();

				if( p )
				{
					if( te )
					{
						this->app_.local_logger().LogLocal(ErrorString(te));
						this->app_.local_logger().LogLocal( utility::FormatStr("drop resource: %s, connid %d"
							, res.name.c_str(), p->connid()) );
					}else
					{
						this->app_.local_logger().LogLocal( utility::FormatStr("disconnect resource: %s, connid %d"
							, res.name.c_str(), p->connid()) );
					}

					if( this->on_disconnect_handler_ )
						this->on_disconnect_handler_(p, te, res);
				}

				if( te && setting.reconnect_on_disconnect )
					this->AsyncConnect(res, setting, 1);
			};

			// start message pulling
			pconn->Start();
		}else
		{
			app_.local_logger().LogLocal("connecting resource aborted due to shutdown: " + res.name);
		}
	}else
	{
		app_.local_logger().LogLocal(ErrorString(te));
		app_.local_logger().LogLocal(utility::FormatStr("connect to resource faild on %d try: %s", try_num, res.name.c_str()));

		// re-try connect when fail
		if( !setting.tot_try || try_num < setting.tot_try )
			this->AsyncConnect(res, setting, try_num+1);
		else
		{
			app_.local_logger().LogLocal("reach max try, stop trying to connect resource: " + res.name);

			if( this->on_abort_handler_ )
				this->on_abort_handler_(pconn, te, res);
		}
	}
}
