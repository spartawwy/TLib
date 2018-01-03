// By Xixin Silas Cheng

#ifndef TSYSTEM_MESSAGE_SYSTEM_H_
#define TSYSTEM_MESSAGE_SYSTEM_H_

#include <string>
#include <unordered_map>

#include "TLib/core/tsystem_communication_common.h"

namespace TSystem
{

	//-------------------
	// message system
	//-------------------
	struct Message;

	class MessageSystem
	{
	public:

		MessageSystem();
		MessageSystem(MessageSystem&& in);
		MessageSystem& operator=(MessageSystem&& in);

		// register class name with id, throw TException when name / id duplication found
		void RegisterClassID(const std::string& name, unsigned short id); 

		// find class id from registered class, throw TException when name not found
		unsigned short FindClassID( const std::string& name) const;

		// find class id with no throw, return 0 if not found
		unsigned short FindClassID( const std::string& name, const std::nothrow_t& tag) const; 

	private:

		// class ids ( class name, class id)
		std::unordered_map<std::string, unsigned short>      class_id_map_;
	};

	//-----------------------
	// message handler group
	//-----------------------
	class MessageHandlerGroup
	{
	public:

		explicit MessageHandlerGroup(const MessageSystem& msg_system);

		// MessageHandler(unsigned short connid, const Message& msg)
		typedef std::function<void(communication::Connection*, const Message&)> MessageHandler;

		// throw exception if name is not registered with a class id
		// multiple register will overwrite the old handler
		void RegisterHandler(const std::string& name, MessageHandler&& handler);

		// check if has handler
		bool HasHandler(const std::string& name) const;

		// invoke handler
		void InvokeHandler(communication::Connection* p, const Message& msg) const;

		// link connection to this message handler group 
		void LinkConnection(std::shared_ptr<communication::Connection>& pconn) const;
        //temp codes
        std::string PrintHandler() const;

	private:

		const MessageSystem& msg_system_;

		// handlers, used class_id as index
		// use vector instead of unordered_map to achieve best
		// random access performance, class_id begins from 1
		std::vector<MessageHandler>   handlers_;
	};
}

#endif // TSYSTEM_MESSAGE_SYSTEM_H_