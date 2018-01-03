// by Xixin Silas Cheng

#ifndef TSYSTEM_ASYNC_OPERATION_H_
#define TSYSTEM_ASYNC_OPERATION_H_

#include <atomic>
#include <functional>
#include <mutex>
#include <unordered_map>

namespace TSystem
{
	class TError;

	// manager class for async operations
	class AsyncOperationManager
	{
	public:
		
		typedef std::function<void(TError& ec)>   HandlerType;

		AsyncOperationManager();

		// get next aysnc_op_seq;
		unsigned int NextAsyncSeq() { return ++async_op_seq_; }
		
		// register a handler for completion of the sequence id
		// throw TException if sequence id has been registered already
		void RegisterAsyncSeq(unsigned int seq, HandlerType&& handler);
		
		// set seq's opertion's return code.
		void NotifyAsyncSeq(unsigned int seq, TError& te);

		// cancel sequence id
		void CancelAsyncSeq(unsigned int seq);

	private:
		std::atomic_uint32_t	async_op_seq_; // the async operation sequence number    

		// current unfinished async operation seq
		std::mutex										 async_ops_lock_;
		std::unordered_map<unsigned int, HandlerType>	 async_ops_;           
	};

	// a scope handler wrapper, cancel handler when out of scope
	class ScopedHandler
	{
	public:

		// ctor, will throw TException
		ScopedHandler( AsyncOperationManager& mgt, AsyncOperationManager::HandlerType&& handler)
			: mgt_(mgt)
			, seq_(mgt_.NextAsyncSeq())
		{
			mgt.RegisterAsyncSeq(seq_, std::move(handler));
		}
		
		~ScopedHandler()
		{
			mgt_.CancelAsyncSeq(seq_);
		}

		unsigned int SequenceID() const { return seq_; };

	private:
		AsyncOperationManager& mgt_;
		unsigned int           seq_;
	};
}

#endif // TSYSTEM_ASYNC_OPERATION_H_