#ifndef TSYSTEM_CLIENT_SOCKET_H_
#define TSYSTEM_CLIENT_SOCKET_H_

#include "TLib/core/tsystem_communication_common.h"
#include "Tlib/core/tsystem_message_system.h"

namespace TSystem
{
	namespace layinfo
	{
		struct AppResource;
	}

	class ServerClientAppBase;

	//-------------------------------------------------------------------------------------------
	// this client socket class is used to replace the 
	// base client socket in traditional c++ degisn.
	// instead of deriving from base class and overwrite the 
	// virtual function, this class allow easy client socket implementation.
	// it can connect a server with connection setting, specifing if reconnect / max re-connect
	// it also allow user to connect to different servers as long as the handler for 
	// on-connect / on-disconnect / msg handlers are the same
	//-------------------------------------------------------------------------------------------

	class ClientSocket
	{
	public:

		//--------------------------
		// async-connection setting:
		// 
		//  tot_try:                   if set none zero, number of try before abort
		//  reconnect_on_disconnect:   if reconnect after disconnection (not connecting fail)
		//--------------------------
		struct ConnectingSetting
		{
			ConnectingSetting(): tot_try(0), reconnect_on_disconnect(true){}

			ConnectingSetting(unsigned short total_try, bool reconnect)
				: tot_try(total_try), reconnect_on_disconnect(reconnect){}

			unsigned short  tot_try;				// if set none zero, number of try before abort
			bool reconnect_on_disconnect;           // if reconnect after disconnection (not connecting fail)
		};

		//---------------------------------------------------
		// connection network events handlers
		// 1. on handshake received from server, this means the connection is ready to use,
		//    connection's life span is managed by the system, pass raw pointer
		// 2. on disconnect from server, this means doing any cleaning work
		//    connection's life span is released by the system, pass shared pointer
		//---------------------------------------------------

		// on connect handler, the msg is the handshake message, including id info
		typedef std::function<void(communication::Connection* p
			, const Message& msg)>				OnHandshakeHandlerType;

		// on disconnect handler
		typedef std::function<void(std::shared_ptr<communication::Connection>& pconn
			, const TError& err
			, const layinfo::AppResource& res)> OnDisconnectHandlerType;

		typedef OnDisconnectHandlerType OnConnectAbortHandlerType;

		explicit ClientSocket(ServerClientAppBase& app);
		virtual ~ClientSocket();

		//----------
		// setups
		//----------
		MessageHandlerGroup& msg_handlers() { return msg_handlers_; }

		// throw TException when register handshake fail
		void on_handshake_handler(OnHandshakeHandlerType&& handler);

		void on_disconnect_handler(OnDisconnectHandlerType&& handler)
		{on_disconnect_handler_ = std::move(handler); }

		void on_abort_handler(OnConnectAbortHandlerType&& handler)
		{on_abort_handler_ = std::move(handler); }

		//-----------------------
		// advanced async-connect
		//-----------------------

		// async connect resource using resource name
		// throw exception when resource name is not unique
		void AsyncConnect(const std::string& res_name, const ConnectingSetting& setting);

		// async connect resource using AppResource struct
		void AsyncConnect(const layinfo::AppResource& res, const ConnectingSetting& setting);

	private:

		void AsyncConnect(const layinfo::AppResource& res, const ConnectingSetting& setting, int try_num );

		void SetupConnection(const layinfo::AppResource& res, const ConnectingSetting& setting, int try_num
			, std::shared_ptr<communication::Connection>& pconn
			, const TError& te);

		ServerClientAppBase&                app_;

		OnHandshakeHandlerType              on_handshake_handler_;
		OnDisconnectHandlerType             on_disconnect_handler_;
		OnConnectAbortHandlerType           on_abort_handler_;

		MessageHandlerGroup					msg_handlers_;
	};
}

#endif //TSYSTEM_CLIENT_SOCKET_H_
