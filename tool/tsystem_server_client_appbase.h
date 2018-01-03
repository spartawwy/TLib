// By Xixin Silas Cheng

#ifndef TSYSTEM_SERVER_CLIENT_APPBASE_H_
#define TSYSTEM_SERVER_CLIENT_APPBASE_H_

#include <mutex>

#include "TLib/core/tsystem_appbase.h"
#include "TLib/core/tsystem_communication_dock.h"
#include "TLib/core/tsystem_message_system.h"
#include "TLib/core/tsystem_topic_service.h"

#include "TLib/tool/tsystem_client_socket.h"
#include "TLib/tool/tsystem_generic_request.h"
#include "TLib/tool/tsystem_layinfo_resource.h"

namespace TSystem
{
	// base class for both server and client
	// both server and client need to have abilities to:
	// a) communicate
	// b) async processing message
	// c) load resource
	// It needs to be noticed that an application can be BOTH server and clients at the same time
	// therefore there is no separate base class for server and client
	// the traditional desgin of separate server and client base is because server need to listen to a port
	// while client need to connect to a port. These has been handled in the CommunicationDock class.
	class ServerClientAppBase: public AppBase
	{
	public:

		virtual void Shutdown() override;
		
		const layinfo::AppResourceManager&	resource_manager() const { return resource_manager_; }

		const MessageSystem& msg_system() const { return msg_system_; }

		// connection to a resource by name
		// throw TException when the resource is not connectable 
		std::shared_ptr<communication::Connection> ConnectResource(const layinfo::AppResource& res, TSystem::TError& err);
		void AsyncConnectResource(const layinfo::AppResource& res, communication::ConnectingHandler&& handler);

		// reload .cfg files
		void ReloadConfigs();

	protected:

		// constructor, for derived class only
		ServerClientAppBase(const std::string& type, const std::string& name, const std::string& version);		

		// throw TException if register resource failed
		void Initiate();

		//--------------------
		// node relay related
		// node relay protocol:
		//  1) call ConnectNode() to connect to node relay
		//  2) receive node relay handeshake
		//  3) in HandleNodeHandShake(), set handshake() to true, call SetupNodeRelayConnection for routine initiating procedure 
		//          (setup logger, setup node relay topic) and customize initiating procedure
		//-------------------

		// connect node , throw TException if node relay not found
		void ConnectNode();

		// setup node relay connection, to be called after node relay handshake is validated
		void SetupNodeRelayConnection(std::shared_ptr<communication::Connection>& pconn);

		// pure virtual handshake and disconnect handler for node relay connection
		virtual void HandleNodeHandShake(communication::Connection* p, const Message& msg) = 0;
		virtual void HandleNodeDisconnect(std::shared_ptr<communication::Connection>& pconn
			, const TError& te)    = 0;

		
		//---------------------------
		// other virtual functions
		//---------------------------

		// this function will be called in ReloadConfigs() after configs_ is reloaded
		// put configs of service components that need to be updated on fly in this function
		virtual void OnLoadConfigs() {};
				
		//-----------------
		// initiate options
		//-----------------

		// init options
		void option_validate_app(bool b) { std::get<0>(init_option_) = b; }
		bool option_validate_app() const { return std::get<0>(init_option_); }

		void option_load_class_id(bool b) { std::get<1>(init_option_) = b; }
		bool option_load_class_id() const { return std::get<1>(init_option_); }
		
		// the resource manager
		layinfo::AppResourceManager       resource_manager_;

		// the message system and handler group	
		MessageSystem			          msg_system_;	

		// the generic request kernel
		communication::GenericRequestKernel		generic_request_kernel_;

		// the communication dock
		communication::CommunicationDock  comm_dock_;	   

		// the node relay socket
		ClientSocket								node_relay_socket_;    
		communication::TopicData                    node_relay_topic_;

	private:

		// (validate_app, load_class_id) default = (true, true)
		std::tuple<bool, bool>	init_option_;

		// for reloading configs
		std::mutex		config_mutex_;
	};

	// send app's info as handshake
	void SendHandShake(const ServerClientAppBase& app, std::shared_ptr<communication::Connection>& p);
}

#endif  // TSYSTEM_SERVER_APPBASE_H_