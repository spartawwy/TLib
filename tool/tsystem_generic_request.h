#ifndef TSYSTEM_GENERIC_REQUEST_H_
#define TSYSTEM_GENERIC_REQUEST_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "TLib/tool/tsystem_cmdoption.h"
#include "TLib/tool/tsystem_generic_request.pb.h"

namespace TSystem
{
	struct Message;
	class MessageSystem;

namespace communication
{
	class Connection;
}

	class RequestHandler
	{
	public:

		virtual ~RequestHandler(){}

		virtual void HandleRequest(communication::Connection* p, unsigned short sender_pid, GenericRequest& req) = 0;

		const std::string& name() const { return name_; }
		const std::string& info() const { return info_; }

	protected:

		RequestHandler(const std::string& name, const std::string& info) : name_(name), info_(info), opts_() {}

		utility::CmdLineOption opts_;

	private:
		std::string            name_;
		std::string            info_;
	};

namespace communication
{
	class GenericRequestKernel
	{
	public:

		explicit GenericRequestKernel(const MessageSystem& msg_system);

		// throw TException if already registered
		// req_name are case sensitive
		void RegisterHandler(std::unique_ptr<RequestHandler>&& handler);

		// handle message
		void HandleMessage(Connection* p, const Message& msg);

		// all handlers
		const std::unordered_map<std::string, std::unique_ptr<RequestHandler>>& handlers() const 
		{ return handlers_; }

	private:
		
		const MessageSystem& msg_system_;

		std::unordered_map<std::string, std::unique_ptr<RequestHandler>>  handlers_;
	};
}
}

#endif // TSYSTEM_GENERIC_REQUEST_H_