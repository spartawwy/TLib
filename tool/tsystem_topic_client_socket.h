#ifndef TSYSTEM_TOPIC_CLIENT_SOCKET_H_
#define TSYSTEM_TOPIC_CLIENT_SOCKET_H_

#include "TLib/core/tsystem_topic_service.h"
#include "TLib/core/tsystem_serialization.h"
#include "TLib/tool/tsystem_topic_request.pb.h"

namespace TSystem
{
	class ServerClientAppBase;

	//------------------------------------------------------
	// TopicClientSocket
	// this is the class used to sent request
	// normally used as local variable when sending request
	//------------------------------------------------------
	class TopicClientSocket
	{
	public:

		TopicClientSocket( ServerClientAppBase& app, const std::shared_ptr<communication::Connection>& p )
			: app_(app), pconn_(p)
		{
			assert(pconn_);
		}

		//--------------
		// subscribe
		//--------------

		// subscribe all
		void Subscribe(unsigned short topic_id, int request_id
			, communication::TopicData::DataStartPos pos);
		
		// subscribe one
		void Subscribe(unsigned short topic_id, const std::string& key, int request_id
			, communication::TopicData::DataStartPos pos, bool add_key);

		// subscribe some
		template<typename IteratorType>
		void Subscribe(unsigned short topic_id, IteratorType iter_begin, IteratorType iter_end, int request_id
			, communication::TopicData::DataStartPos pos, bool add_key)
		{
			assert(pconn_);
			TopicRequest req;
			req.set_request_id(request_id);
			req.set_topic_id(topic_id);
			req.set_add_key(add_key);
			req.set_data_pos(static_cast<int>(pos));
			while( iter_begin != iter_end )
				req.add_keys(*(iter_begin++));

			LogRequest(req);
			pconn_->AsyncSend(Encode(req, app_.msg_system(), Message::HeaderType(0, app_.pid(), 0))); 
		}

		//--------------
		// unsubscribe
		//--------------

		// unsubscribe all
		void Unsubscribe(unsigned short topic_id, int request_id);
		
		// unsubscribe one
		void Unsubscribe(unsigned short topic_id, const std::string& key, int request_id);

		// unsubscribe some
		template<typename IteratorType>
		void Unsubscribe(unsigned short topic_id, IteratorType iter_begin, IteratorType iter_end, int request_id)
		{
			assert(pconn_);
			TopicRequest req;
			req.set_request_id(request_id);
			req.set_topic_id(topic_id);
			while( iter_begin != iter_end )
				req.add_keys(*(iter_begin++));
			req.set_type(TopicRequest::Unsubscribe);

			LogRequest(req);
			pconn_->AsyncSend(Encode(req, app_.msg_system(), Message::HeaderType(0, app_.pid(), 0))); 
		}

		//--------------
		// request
		//--------------

		// request all
		void Request(unsigned short topic_id, int request_id
			, communication::TopicData::DataStartPos pos);

		// request one
		void Request(unsigned short topic_id, const std::string& key, int request_id
			, communication::TopicData::DataStartPos pos);

		// request some
		template<typename IteratorType>
		void Request(unsigned short topic_id, IteratorType iter_begin, IteratorType iter_end, int request_id
			, communication::TopicData::DataStartPos pos)
		{
			assert(pconn_);
			TopicRequest req;
			req.set_request_id(request_id);
			req.set_topic_id(topic_id);
			while( iter_begin != iter_end )
				req.add_keys(*(iter_begin++));
			req.set_type(TopicRequest::Request);
			req.set_data_pos(static_cast<int>(pos));

			LogRequest(req);
			pconn_->AsyncSend(Encode(req, app_.msg_system(), Message::HeaderType(0, app_.pid(), 0))); 
		}

	private:

		void LogRequest(const TopicRequest& req);

		std::shared_ptr<communication::Connection>  pconn_;
		ServerClientAppBase&						app_;
	};
}

#endif // TSYSTEM_TOPIC_CLIENT_SOCKET_H_