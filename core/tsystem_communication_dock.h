// by Xixin Silas Cheng
// a communication dock containers all types of connection owners

#ifndef TSYSTEM_COMMUNICATION_DOCK_H_
#define TSYSTEM_COMMUNICATION_DOCK_H_

#include <mutex>
#include <unordered_map>

#include "TLib/core/tsystem_connection.h"

namespace TSystem
{

	// forward declaration
	class TaskPool;

// forward declaration;
namespace communication
{
	// forward declaration
	class TcpDock;

	// a communication dock wraps all types of connection owners
	// currently, only tcp is implemented
	class CommunicationDock
	{
	public:
	
		
		explicit CommunicationDock(const TaskPool& pool);
		~CommunicationDock();
			
		// disconnect all connections 
		void Shutdown();

		// get a connection, return a weak_ptr in case of removed connid
		std::weak_ptr<Connection> GetConnection(int connid);


		//-----------------------
		// connection operations
		//-----------------------

		// start a tcp port, throw TException when socket fails
		void StartPort(unsigned short port, ConnectingHandler&& handler );

		// blocking connect to an (address, port) using tcp or udp, return connection
		std::shared_ptr<Connection> Connect(const std::string& address, int port, TSystem::TError& err
			, Connection::Type type);

		// async connect to an (address, port) using tcp or udp
		void AsyncConnect(const std::string& address, int port, ConnectingHandler&& handler 
			, Connection::Type type);

	private:

        const TaskPool& pool_;

		int  NextConnID();

		void Register(const std::shared_ptr<Connection>& pconn);

		std::shared_ptr<Connection> Unregister(int connid ); 

		std::atomic_bool                       shutdown_;

		// next connid
		std::atomic_int	                       next_connid_;

		// connection container lock
		std::recursive_mutex                             connections_lock_;

		// connid to connections, connection holder
		std::unordered_map<int, std::shared_ptr<Connection>>   connections_;

		// tcp utilities
		std::unique_ptr<TSystem::communication::TcpDock>			tcp_dock_;
		friend class TcpDock;
	};
}
}

#endif // TSYSTEM_COMMUNICATION_DOCK_H_