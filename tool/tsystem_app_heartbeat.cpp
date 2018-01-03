#include "tsystem_app_heartbeat.h"

#include "TLib/core/tsystem_error.h"
#include "TLib/core/tsystem_serialization.h"
#include "TLib/core/tsystem_time.h"

using namespace TSystem;

AppHeartbeat::AppHeartbeat(const TaskPool& pool, const MessageSystem& msg_system)
	: heart_beat_mutex_(), heart_beat_()
	, shutdown_()
	, broadcast_timer_(pool), topic_(pool) 
	, msg_system_(msg_system)
{
	shutdown_ = false;
}

void AppHeartbeat::Start()
{
	if( !shutdown_ )
	{
		broadcast_timer_.ExpirationDuration(std::chrono::seconds(1));
		broadcast_timer_.PostTask([this](const TSystem::TError& te)
		{
			if( !te )
				this->ScheduledBroadcast();
            else
               printf("fail\n");
		});
	}
}

void AppHeartbeat::Shutdown()
{
	shutdown_ = true;
	broadcast_timer_.Cancel();
}

void AppHeartbeat::AddSubscriber(const std::shared_ptr<communication::Connection>& pconn)
{
	topic_.AsyncAddDestination(pconn, communication::TopicData::DataStartPos::LAST);
}

void AppHeartbeat::AddSubscriber(std::shared_ptr<communication::Connection>&& pconn)
{
	topic_.AsyncAddDestination(std::move(pconn), communication::TopicData::DataStartPos::LAST);
}

void AppHeartbeat::UpdateStatus(Heartbeat::Status s, unsigned short pid)
{
	std::lock_guard<std::mutex> lock(heart_beat_mutex_); 
	if( heart_beat_.status() != s )
	{
		heart_beat_.set_status(s);
		heart_beat_.set_pid(pid);
		heart_beat_.clear_info();

		Broadcast();
	}
}

void AppHeartbeat::UpdateStatus(Heartbeat::Status s, unsigned short pid, const std::string& info)
{
	std::lock_guard<std::mutex> lock(heart_beat_mutex_); 
	if( heart_beat_.status() != s )
	{
		heart_beat_.set_status(s);
		heart_beat_.set_pid(pid);
		heart_beat_.set_info(info);
		Broadcast();
	}
	
}

void AppHeartbeat::Broadcast()
{
	FillTime(TimePoint(GetClock().Now()), *heart_beat_.mutable_timestamp());
	topic_.AsyncUpdateData( Encode( heart_beat_, msg_system_, Message::HeaderType(0, heart_beat_.pid(), 0)) );
}

void AppHeartbeat::ScheduledBroadcast()
{
	std::lock_guard<std::mutex> lock(heart_beat_mutex_);
	Broadcast();
	Start();
}