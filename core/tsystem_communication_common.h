#ifndef TSYSTEM_COMMUNICATION_COMM_H_
#define TSYSTEM_COMMUNICATION_COMM_H_

#include <functional>
#include <memory>

namespace TSystem
{
	class TError;

namespace communication
{
	struct CodedData
	{
		explicit CodedData(unsigned int sz)
			: size_(sz)
		{
			data_ = size_ ? new char[size_] : nullptr;
		}

		CodedData(const char* src, unsigned int sz)
			: size_(sz)
		{
			data_ = size_ ? new char[size_] : nullptr;
			if( data_ && src )
				memcpy(data_, src, size_);
		}

		~CodedData()
		{
			delete[] data_;
		}

		char* data() { return data_; }
		const char* data() const { return data_; }

		unsigned int size() const { return size_; }

	private:
		char* data_;
		unsigned int   size_;
	};

	typedef std::shared_ptr<CodedData>   CodedDataPtr;
	typedef unsigned int				 SizeHeaderType;

	class Connection;	

	typedef std::function<void(std::shared_ptr<Connection>&, const TSystem::TError&)>  ConnectingHandler; 
}
}
#endif //TSYSTEM_COMMUNICATION_COMM_H_