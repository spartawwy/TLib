#ifndef TSYSTEM_SERVER_APPBASE_H_
#define TSYSTEM_SERVER_APPBASE_H_

#include "TLib/tool/tsystem_app_heartbeat.h"
#include "TLib/tool/tsystem_server_client_appbase.h"

namespace TSystem
{

	// Base class for a server, a server can / has
	// a) connect to the system relay of the node
	// b) send heart beat to client and system relay
	// c) start server port and mesasge hanlders for clients
	// d) one topic server
	class ServerAppBase: public ServerClientAppBase
	{
	public:

		virtual void Shutdown() override;
		
		// roll server state
		void RollState(); 

	protected:

		ServerAppBase(const std::string& type, const std::string& name, const std::string& version);

		// may throw TException if fail to register app
		void Initiate();

		//---------------
		// init options
		//---------------
		void option_link_topic_server(bool b) { std::get<0>(init_option_) = b; }
		bool option_link_topic_server() const { return std::get<0>(init_option_); }
		
		//-------------------
		// inbound connection related
		// inbound protocol:
		//  1) server accept inbound connection
		//  2) inbound connection sends handshake 
		//  3) in HandleInboundHandShake() validates handshake
		//	     a) if pass, set handshake() to true, call SetupInboundConnection() for routine initiating procedure 
		//          (send server handshake, server heartbeat) and customize initiating procedure
		//       b) if fail, just return, server will disconnect the inbound connection
		//-------------------

		// start server port, may throw TException
		void StartPort();

		// setup inbound connection, to be called after inbound handshake is validated
		void SetupInboundConnection(std::shared_ptr<communication::Connection>& pconn, const layinfo::AppResource& res);
	
		// pure handshake and disconnect handler for inbound connection to the msg_handlers_;
		virtual void HandleInboundHandShake(communication::Connection* p, const Message& msg) = 0;
		virtual void HandleInboundDisconnect(std::shared_ptr<communication::Connection>& pconn
			, const TError& te)    = 0;

		//-------------
		// other
		//-------------

		// server state transition function, called from RollState()
		virtual void UpdateState() = 0;

		// setup node relay connection, to be called after node relay handshake is validated
		void SetupNodeRelayConnection(std::shared_ptr<communication::Connection>& pconn);

		// heart beat
		AppHeartbeat				heart_beat_;
			
		// the default handlers
		MessageHandlerGroup			msg_handlers_;

		// the topic server
		communication::TopicServer              topic_server_;

	private:

		void HandleTopicRequest(communication::Connection* p, const Message& msg);

		TSystem::TaskTimer    state_timer_;

		// (link_topic_server) default = (true)
		std::tuple<bool> init_option_;
	};


}

#endif // TSYSTEM_SERVER_APPBASE_H_