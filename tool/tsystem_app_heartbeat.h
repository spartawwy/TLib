#ifndef TSYSTEM_HEARTBEAT_H_
#define TSYSTEM_HEARTBEAT_H_

#include "TLib/core/tsystem_topic_service.h"
#include "TLib/tool/tsystem_heartbeat_msg.pb.h"

namespace TSystem
{
	class MessageSystem;

	class AppHeartbeat
	{
	public:

		AppHeartbeat(const TaskPool& pool, const MessageSystem& msg_system);

		void AddSubscriber(const std::shared_ptr<communication::Connection>& pconn);
		void AddSubscriber(std::shared_ptr<communication::Connection>&& pconn);

		void Start();
		void Shutdown();
	
		void UpdateStatus(Heartbeat::Status s, unsigned short pid);
		void UpdateStatus(Heartbeat::Status s, unsigned short pid, const std::string& info); 

	private:

		void Broadcast();
		void ScheduledBroadcast();

		std::mutex   heart_beat_mutex_;
		Heartbeat    heart_beat_; 

		std::atomic_bool         shutdown_;
		TaskTimer                broadcast_timer_;
		communication::TopicData topic_;

		const MessageSystem& msg_system_;
	};
}

#endif // TSYSTEM_HEARTBEAT_H_