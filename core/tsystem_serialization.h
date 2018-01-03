#ifndef TSYSTEM_SERIALIZATION_H_
#define TSYSTEM_SERIALIZATION_H_

#include <cassert>

#include "TLib/core/tsystem_communication_common.h"

// forward declaration
namespace google
{
namespace protobuf
{
	class MessageLite;
}
}

namespace TSystem
{
	//--------------------------------
	// communication header/struct interfaces
	//--------------------------------

	// header size
	template<class CommunicationHeaderType>
	unsigned int HeaderSize();

	// encode header
	template<class CommunicationHeaderType>
	void EncodeHeader(const CommunicationHeaderType& info, char* p, unsigned int n);

	// check if enough header size
	template<class CommunicationStructType>
	bool CheckHeaderSize(unsigned int length)
	{ 
		return length > HeaderSize<CommunicationStructType::HeaderType>(); 
	}

	//-----------------
	// serial data
	//-----------------

	// serial data header
	struct SerialDataHeader
	{
		SerialDataHeader(): class_id(0) {}
		SerialDataHeader(unsigned short id): class_id(id) {}

		unsigned short class_id;
	};

	template<>
	inline unsigned int HeaderSize<SerialDataHeader>()
	{
		static unsigned short tot_size = sizeof(decltype(SerialDataHeader::class_id));
		return tot_size; 
	}

	// serialized data
	struct SerialData
	{
		typedef SerialDataHeader HeaderType;

		SerialData(const char* data, std::size_t length)
			: p_(data), size_(length)
		{
			assert(p_);
		}

		unsigned short ClassID() const
		{ 
			assert(p_);
			return *(reinterpret_cast<const unsigned short*>(p_));
		}

		const char* data() const { return p_; }

		const char* coded_data() const 
		{
			assert(p_);
			return p_ + HeaderSize<HeaderType>();
		}

		std::size_t data_size() const { return size_ - HeaderSize<HeaderType>(); }
		std::size_t tot_size() const { return size_; }

	private:

		const char*	   p_;
		std::size_t    size_;
	};

	//------------
	// message
	//------------

	// message header
	struct MessageHeader
	{
		// ctors
		MessageHeader()
		{msg_seq = sender_pid = recevr_pid = 0;}

		MessageHeader(unsigned int seq, unsigned short sender, unsigned short recevr)
			: msg_seq(seq), sender_pid(sender), recevr_pid(recevr)
		{}

		unsigned int msg_seq;
		unsigned short sender_pid;
		unsigned short recevr_pid;
	};

	template<>
	inline unsigned int HeaderSize<MessageHeader>()
	{ 
		static unsigned short tot_size = sizeof(decltype(MessageHeader::msg_seq)) 
			+ sizeof(decltype(MessageHeader::sender_pid))
			+ sizeof(decltype(MessageHeader::recevr_pid));
		return tot_size; 
	}

	template<>
	inline void EncodeHeader(const MessageHeader& info, char* p, unsigned int n)
	{
		assert(p && n >= HeaderSize<MessageHeader>());
				
		*reinterpret_cast<unsigned int*>(p)  = info.msg_seq;
		p += sizeof(info.msg_seq);

		*reinterpret_cast<unsigned short*>(p) = info.sender_pid;
		p += sizeof(info.sender_pid);

		*reinterpret_cast<unsigned short*>(p) = info.recevr_pid;
	}

	// struct for network message, including some other information
	struct Message
	{
		typedef MessageHeader HeaderType;

		// ctor
		Message(const char* data, std::size_t length)
			: p_start_(data)
			, data_(p_start_+HeaderSize<HeaderType>(), length-HeaderSize<HeaderType>())
		{assert(p_start_);}

		// accessors
		unsigned int MsgSeq() const 
		{
			assert(p_start_);
			return *(reinterpret_cast<const unsigned int*>(p_start_));
		}

		unsigned short SenderPid() const
		{
			assert(p_start_);
			return *(reinterpret_cast<const unsigned int*>(p_start_
				+sizeof(decltype(HeaderType::msg_seq))));
		}

		unsigned short ReceiverPid() const
		{
			assert(p_start_);
			return *(reinterpret_cast<const unsigned int*>(p_start_
				+sizeof(decltype(HeaderType::msg_seq))
				+sizeof(decltype(HeaderType::sender_pid))));
		}

		const SerialData& serial_data() const
		{
			return data_;
		}

		const char* data() const { return p_start_; }

		std::size_t tot_size() const { return data_.tot_size()+HeaderSize<HeaderType>() ; }

	private:
		const char* p_start_;
		
		SerialData  data_;
	};

	//------------------
	// encode / decode
	//------------------

	class MessageSystem;

	// utility function to encode:  tot size + reserved header length + serial data
	// return <CodedDataPtr, pointer to header>;
	// throw TException when proto buf api fail
	std::pair<communication::CodedDataPtr, char*> 
		EncodeHelper(const google::protobuf::MessageLite& object, unsigned int ext_header_length, const MessageSystem& msg_system);

	std::pair<communication::CodedDataPtr, char*> 
		EncodeHelper(const SerialData& data, unsigned int ext_header_length);

	// encode tot size + CommunicationHeader + serial data, for network communication
	// throw TException when proto buf api fail
	template<class CommunicationHeaderType>
	communication::CodedDataPtr Encode(const google::protobuf::MessageLite& object
		, const MessageSystem& msg_system
		, const CommunicationHeaderType& info)
	{
		auto res = EncodeHelper(object, HeaderSize<CommunicationHeaderType>(), msg_system);
		EncodeHeader(info, res.second, HeaderSize<CommunicationHeaderType>());
		return res.first;
	}

	template<class CommunicationHeaderType>
	communication::CodedDataPtr Encode(const SerialData& data, const CommunicationHeaderType& info)
	{
		auto res = EncodeHelper(data, HeaderSize<CommunicationHeaderType>());
		EncodeHeader(info, res.second, HeaderSize<CommunicationHeaderType>());
		return res.first;
	}

	// encode tot size + serial data, for local file serialization 
	// throw TException when proto buf api fail
	inline communication::CodedDataPtr Encode(const google::protobuf::MessageLite& object
		, const MessageSystem& msg_system)
	{
		return EncodeHelper(object, 0, msg_system).first;
	}

	inline communication::CodedDataPtr Encode(const SerialData& data)
	{
		return EncodeHelper(data, 0).first;
	}
    const communication::SizeHeaderType FORWORD_FLAG_VAL = ~0;
	// forward encoding, tot size + comm_struct
	template<class CommunicationStructType>
	communication::CodedDataPtr ForwardEncode(const CommunicationStructType& comm_struct)
	{
		communication::SizeHeaderType struct_bytes = static_cast<communication::SizeHeaderType>(HeaderSize<CommunicationStructType::HeaderType>()
			+ comm_struct.serial_data().tot_size());
		communication::CodedDataPtr res 
			=  std::make_shared<communication::CodedData>(struct_bytes + (unsigned int)sizeof(struct_bytes));

		//memcpy(res->data(), &struct_bytes, sizeof(struct_bytes));
        *(communication::SizeHeaderType*)res->data() = FORWORD_FLAG_VAL;

		memcpy(res->data()+sizeof(struct_bytes), comm_struct.data(), struct_bytes);
		return res;
	}

	// decode CommunicationHeader + serial data
	template<class CommunicationStructType>
	bool Decode(const CommunicationStructType& data, google::protobuf::MessageLite& object
		, const MessageSystem& msg_system)
	{ 
		return Decode(data.serial_data(), object, msg_system); 
	}

	// decode serial data
	template<>
	bool Decode(const SerialData& data, google::protobuf::MessageLite& object
		, const MessageSystem& msg_system);
}

#endif // TSYSTEM_SERIALIZATION_H_