// by Xixin Silas Cheng
// interfaces for a connection

#ifndef TSYSTEM_CONNECTION_H_
#define TSYSTEM_CONNECTION_H_

#include <atomic>
#include <algorithm>
#include <type_traits>
#include <vector>

#include "TLib/core/tsystem_communication_common.h"

namespace TSystem
{
namespace communication
{

	// the default connection event handler functor struct
	// also define event handler type
	struct ConnectionEventHandlers
	{
		// handle message received event, called after a message is received
		typedef  std::function<void(const char*, std::size_t)>  OnMessageReceivedHandler;
	
		// handle connect event
		// called before connection reading loop begins
		typedef std::function<void()>     OnConnectHandler;

		// handle close event
		// called after connection is closed
		typedef std::function<void(const TSystem::TError&)>     OnDisconnectHandler;

		// handle 
		ConnectionEventHandlers()
			: on_msg_recv()
			, on_connect()
			, on_disconnect()
		{}

		OnMessageReceivedHandler	on_msg_recv;
		
		OnConnectHandler            on_connect;

		OnDisconnectHandler         on_disconnect;
	};

	// the connection
    class Connection
	{
	public:

		enum class Type:char
		{
			tcp  = 0
		};
		
		enum class Status:char
		{
			connected		= 0	,	// connected   
			drop                ,   // unexpected disconnect
			disconnected	        // normal disconnect
		};

		// virtual destructor
		virtual ~Connection();

		// virtual function to get shared_ptr for itself
		virtual std::shared_ptr<Connection> shared_this() = 0;

		// start connection message pulling, may throw TException on socket error
		void Start();

		// normal disconnect and close connection
		void Disconnect();

		// get type
		Type type() const  { return type_; }
		
		// get status
		Status status() const { return status_; }

		// get connection id 
		int  connid() const { return conn_id_; }

		// hand shaked
		bool hand_shaked() const { return hand_shaked_; }
		void hand_shaked(bool b) { hand_shaked_ = b;    }

		// message processed
		unsigned int message_processed() const { return message_processed_; } 

		// event handlers
		ConnectionEventHandlers& event_handlers() { return event_handlers_; }
		const ConnectionEventHandlers& event_handlers() const { return event_handlers_; }

		// async send 
		void AsyncSend(const CodedDataPtr& data )
		{
			if( data && data->size() )
				AysncQueueData(CodedDataPtr(data));
		}

		void AsyncSend(CodedDataPtr&& data )
		{
			if( data && data->size() )
				AysncQueueData(std::move(data));
		}

		void AsyncSend(const CodedDataPtr* data, std::size_t length)
		{
			if( data && length  )
				AysncQueueData(std::vector<CodedDataPtr>(data, data+length));
		}

		template<class InputIterType>
		void AsyncSend(InputIterType first, InputIterType last)
		{
			static_assert(std::is_same<CodedDataPtr, InputIterType::value_type>::value
				, "must be iterator to CodedDataPtr");
			if( first != last )
				AysncQueueData(std::vector<CodedDataPtr>(first, last));
		}
		
        // tmpcode
        //virtual void TriggerRead() {}

	protected:

		// ctor
		explicit Connection(Type t); 

		//----------------------
		// virtual interfaces
		//----------------------
		
		// connection initiation
		virtual void InitConnection()                    = 0;

		// connection clean
		virtual void CloseConnection(const TError& ec)   = 0;

		// prepare to read 
		virtual void StartRead()                         = 0;

		// prepare to write
		virtual void StartWrite()                        = 0;
		
		// async queuing data
		virtual void AysncQueueData( std::vector<CodedDataPtr>&& batch_data )     = 0;
		virtual void AysncQueueData( CodedDataPtr&& data)                         = 0;
	
		//-------------------------------------------------
		// aux. functions, to be called in derived classes
		//-------------------------------------------------
		void QueueBatchData( std::vector<CodedDataPtr>& batch_data )
		{
			for( CodedDataPtr& data : batch_data )
				if( data && data->size() )
					data_queue_.push_back(std::move(data));

			if( !is_writing_ )
			{
				is_writing_ = true;
				PrepareWrite();
			}
		}

		void QueueData( CodedDataPtr& data)
		{
			data_queue_.push_back(std::move(data));

			if( !is_writing_ )
			{
				is_writing_ = true;
				PrepareWrite();
			}
		}

		void PrepareWrite();

		// connection status
		std::atomic<Status>         status_;		
		
		// message processed
		unsigned int                message_processed_;

		// handlers
		ConnectionEventHandlers     event_handlers_;
		std::function<void(int)>       holder_remover_;

		std::vector<CodedDataPtr>	data_queue_;	

	private:

		// type
		Type						type_;

		// connection id
		int							conn_id_;

		// hand shaked flag
		std::atomic_bool            hand_shaked_;

		// queue data related
		bool                        is_writing_;
		
		friend class CommunicationDock;
        friend class TopicData;
        /*friend class TcpDock;*/
	};
}
}

#endif // TSYSTEM_CONNECTION_H_