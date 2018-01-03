#include "tsystem_topic_client_socket.h"

#include "TLib/core/tsystem_utility_functions.h"
#include "TLib/tool/tsystem_server_client_appbase.h"

using namespace TSystem;
//-------------------
// TopicClientSocket
//-------------------
void TopicClientSocket::Subscribe(unsigned short topic_id, int request_id
								  , communication::TopicData::DataStartPos pos)
{
	assert(pconn_);
	TopicRequest req;
	req.set_request_id(request_id);
	req.set_topic_id(topic_id);
	req.set_all_available(true);
	req.set_data_pos(static_cast<int>(pos));

	LogRequest(req);
	pconn_->AsyncSend(Encode(req, app_.msg_system(), Message::HeaderType(0, app_.pid(), 0)));
}

void TopicClientSocket::Subscribe(unsigned short topic_id,  const std::string& key, int request_id
								  , communication::TopicData::DataStartPos pos, bool add_key)
{
	assert(pconn_);
	TopicRequest req;
	req.set_request_id(request_id);
	req.set_topic_id(topic_id);
	req.add_keys(key);
   /* int pos_real = 0;
    switch(pos)
    {
        case communication::TopicData::DataStartPos::FIRST: pos_real = 0; break;
        case communication::TopicData::DataStartPos::LAST: pos_real = 1; break;
        case communication::TopicData::DataStartPos::LATEST: pos_real = 2; break;
    }
	req.set_data_pos(pos_real);*/
    req.set_data_pos(static_cast<unsigned int>(pos));
	req.set_add_key(add_key);

	LogRequest(req);
	pconn_->AsyncSend(Encode(req, app_.msg_system(), Message::HeaderType(0, app_.pid(), 0)));
}

void TopicClientSocket::Unsubscribe(unsigned short topic_id, int request_id)
{
	assert(pconn_);
	TopicRequest req;
	req.set_request_id(request_id);
	req.set_topic_id(topic_id);
	req.set_all_available(true);
	req.set_type(TopicRequest::Unsubscribe);

	LogRequest(req);
	pconn_->AsyncSend(Encode(req, app_.msg_system(), Message::HeaderType(0, app_.pid(), 0)));
}

void TopicClientSocket::Unsubscribe(unsigned short topic_id,  const std::string& key, int request_id)
{
	assert(pconn_);
	TopicRequest req;
	req.set_request_id(request_id);
	req.set_topic_id(topic_id);
	req.add_keys(key);
	req.set_type(TopicRequest::Unsubscribe);

	LogRequest(req);
	pconn_->AsyncSend(Encode(req, app_.msg_system(), Message::HeaderType(0, app_.pid(), 0)));
}

void TopicClientSocket::Request(unsigned short topic_id, int request_id
								, communication::TopicData::DataStartPos pos)
{
	assert(pconn_);
	TopicRequest req;
	req.set_request_id(request_id);
	req.set_topic_id(topic_id);
	req.set_all_available(true);
	req.set_type(TopicRequest::Request);
	req.set_data_pos(static_cast<int>(pos));

	LogRequest(req);
	pconn_->AsyncSend(Encode(req, app_.msg_system(), Message::HeaderType(0, app_.pid(), 0)));
}

void TopicClientSocket::Request(unsigned short topic_id,  const std::string& key, int request_id
								, communication::TopicData::DataStartPos pos)
{
	assert(pconn_);
	TopicRequest req;
	req.set_request_id(request_id);
	req.set_topic_id(topic_id);
	req.add_keys(key);
	req.set_type(TopicRequest::Request);
	req.set_data_pos(static_cast<int>(pos));

	LogRequest(req);
	pconn_->AsyncSend(Encode(req, app_.msg_system(), Message::HeaderType(0, app_.pid(), 0)));
}

void TopicClientSocket::LogRequest(const TopicRequest& req)
{
	if( req.all_available() )
	{
		std::string msg;
		switch( req.type() )
		{
		case TopicRequest::Subscribe:
			msg = utility::FormatStr("subscribe topic from connid %d for: %d::All::%c"
				, pconn_->connid(), req.topic_id(), static_cast<char>(req.data_pos()));
			break;
		case TopicRequest::Request:
			msg = utility::FormatStr("request topic from connid %d for: %d::All::%c"
				, pconn_->connid(), req.topic_id(), static_cast<char>(req.data_pos()));
			break;
		case TopicRequest::Unsubscribe:
			msg = utility::FormatStr("unsubscribe topic from connid %d for: %d::All::%c"
				, pconn_->connid(), req.topic_id(), static_cast<char>(req.data_pos()));
			break;
		}
		app_.local_logger().LogLocal(std::move(msg));
	}
	else
	{
		switch( req.type() )
		{
		case TopicRequest::Subscribe:
			std::for_each(std::begin(req.keys()), std::end(req.keys()), [&req, this](const std::string& key)
			{
				app_.local_logger().LogLocal( utility::FormatStr("subscribe topic from connid %d for: %d::%s::%c"
					, pconn_->connid(), req.topic_id(), key.c_str(), static_cast<char>(req.data_pos())) );
			});
			break;
		case TopicRequest::Request:
			std::for_each(std::begin(req.keys()), std::end(req.keys()), [&req, this](const std::string& key)
			{
				app_.local_logger().LogLocal( utility::FormatStr("request topic from connid %d for: %d::%s::%c"
					, pconn_->connid(), req.topic_id(), key.c_str(), static_cast<char>(req.data_pos())) );
			});
			break;
		case TopicRequest::Unsubscribe:
			std::for_each(std::begin(req.keys()), std::end(req.keys()), [&req, this](const std::string& key)
			{
				app_.local_logger().LogLocal( utility::FormatStr("unsubscribe topic from connid %d for: %d::%s::%c"
					, pconn_->connid(), req.topic_id(), key.c_str(), static_cast<char>(req.data_pos())) );
			});
			break;
		}
	}
}