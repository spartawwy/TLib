#include "tsystem_async_operation.h"

#include "TLib/core/tsystem_core_error.h"

using namespace TSystem;

AsyncOperationManager::AsyncOperationManager()
	: async_op_seq_(0)
	, async_ops_lock_()
	, async_ops_(1024)
{}

void AsyncOperationManager::RegisterAsyncSeq(unsigned int seq, HandlerType&& handler)
{
	std::lock_guard<std::mutex> lock(async_ops_lock_);
	if( async_ops_.find(seq) == async_ops_.end() )
	{
		async_ops_.insert( std::make_pair(seq, std::move(handler)) );
	}else
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT, "AsyncOperationManager::RegisterAsyncSeq",
			"Sequence number already registered with an handler");
	}
}

void AsyncOperationManager::NotifyAsyncSeq(unsigned int seq, TError& te)
{
	HandlerType handler;
	{
		std::lock_guard<std::mutex> lock(async_ops_lock_);
		auto iter = async_ops_.find(seq);
		if( iter != async_ops_.end() && iter->second )
		{
				handler = std::move(iter->second);
		}
	}
	if( handler )
		handler(te);
}

void AsyncOperationManager::CancelAsyncSeq(unsigned int seq)
{
	std::lock_guard<std::mutex> lock(async_ops_lock_);
	async_ops_.erase(seq);
}