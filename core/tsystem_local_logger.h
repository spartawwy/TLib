// By Xixin Silas Cheng
// logger file

#ifndef TSYSTEM_LOCAL_LOGGER_H_	
#define TSYSTEM_LOCAL_LOGGER_H_	

#include <atomic>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

#include "TLib/core/tsystem_error.h"
#include "TLib/core/tsystem_log_msg.pb.h"
#include "TLib/core/tsystem_task_service.h"

namespace TSystem
{
	// a log file, save file size information and file handle
	struct LogFile
	{
		LogFile(const std::string& name);
		~LogFile();
		
		// moving
		LogFile( LogFile&& in);

		// log message
		void Log(const std::string& msg, const std::string* p_time);

		// flush messages
		void Flush();

		std::string		name_;
		int             count_;
		size_t          file_size_;
		std::fstream	file_;
		bool            need_flush_;
	}; 

	namespace communication
	{
		class TopicData;
	}
	class MessageSystem;

	// a local logger that manages a group of log files
	// when file size larger than 150MB create new log file
	// thread safe
	class LocalLogger
	{
	public:
		explicit LocalLogger(const TaskPool& pool);
		~LocalLogger();

		// set log dir
		void SetDir( const std::string& dir) { log_dir_ = dir; }

		// set logger name
		void SetName(const std::string& name) { log_name_ = name; }

		// set remote
		void SetRemote(communication::TopicData* p_topic, unsigned short pid, const MessageSystem& msg_system)
		{ 
			remote_msg_.set_pid(pid); 
			log_topicdata_ = p_topic; 
			msg_system_ = &msg_system; 
		}

		// logging
		void         LogLocal(const std::string& tag, const std::string& msg);
		void         LogLocal(const std::string& tag, std::string&& msg);
		void         LogLocal(std::string&& tag, const std::string& msg);
		void         LogLocal(std::string&& tag, std::string&& msg);

		void         LogLocal(const std::string& msg);
		void         LogLocal(std::string&& msg);

		// remote logging, also log local
		void         LogRemote(const std::string& tag, const std::string& msg
						, LogMessage::WarnLevel level = LogMessage::LOG);

		void         LogRemote(const std::string& tag, std::string&& msg
						, LogMessage::WarnLevel level = LogMessage::LOG);
		
		void         LogRemote(std::string&& tag, const std::string& msg
						, LogMessage::WarnLevel level = LogMessage::LOG);
		
		void         LogRemote(std::string&& tag, std::string&& msg
						, LogMessage::WarnLevel level = LogMessage::LOG);

		void         LogRemote(const std::string& msg
						, LogMessage::WarnLevel level = LogMessage::LOG);

		void         LogRemote(std::string&& msg
						, LogMessage::WarnLevel level = LogMessage::LOG);

		// start flushing
		void StartFlush();

		// shutdown
		void Shutdown();

	private:

		// check file size and roll log file if necesary
		void RollFile(LogFile& logfile);

		// log message
		void Log(const std::chrono::system_clock::time_point& time
			, const std::string& tag
			, const std::string& msg);
		
		// remote message
		void Remote(const std::chrono::system_clock::time_point& time
			, LogMessage::WarnLevel level
			, const std::string& tag
			, const std::string& msg);

		// flush message
		void Flush();

		// file limit, set to 150MB s
		const static unsigned int size_limit;

		// shutdown flag
		std::atomic_bool												shutdown_; 

		// log strand, all operation is done in this strand
		TaskStrand														log_strand_;

		// flush log file timer
		TaskTimer                                                       flush_timer_;

		// local logging 
		std::string                                                     log_name_;
		std::string                                                     log_dir_;
		std::unordered_map<std::string, LogFile>						logfiles_;	

		// remote logging
		LogMessage                                                      remote_msg_;
		communication::TopicData*                                       log_topicdata_;
		const MessageSystem*                                            msg_system_;
	};

	// free functions
	inline void LogError( LogMessage::WarnLevel level, const TError& te, LocalLogger& logger)
	{
		if( level == LogMessage::LOG )
			logger.LogLocal( ErrorString(te) );
		else
			logger.LogRemote(ErrorString(te), level);
	};

	template<typename ErrorCode>
	void LogError( LogMessage::WarnLevel level
		, ErrorCode ec, std::string func_name, std::string other_info
		, LocalLogger& logger)
	{
		LogError(level, MakeTError( ec, std::move(func_name), std::move(other_info)), logger);
	};

	inline void LogError( LogMessage::WarnLevel level, const std::string& log_tag, const TError& te, LocalLogger& logger)
	{
		if( level == LogMessage::LOG )
			logger.LogLocal(log_tag, ErrorString(te));
		else
			logger.LogRemote(log_tag, ErrorString(te), level);
	};

	template<typename ErrorCode>
	void LogError(  LogMessage::WarnLevel level, const std::string& log_tag
		, ErrorCode ec, std::string func_name, std::string other_info
		, LocalLogger& logger)
	{
		LogError(log_tag, MakeTError( ec, std::move(func_name), std::move(other_info) ), level);
	};

	
}

#endif  //TSYSTEM_LOCAL_LOGGER_H_	
