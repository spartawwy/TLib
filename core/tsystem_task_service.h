// By Xixin Silas Cheng
// task pool

#ifndef TSYSTEM_TASK_POOL_H_
#define TSYSTEM_TASK_POOL_H_

#include <chrono>
#include <functional>
#include <future>
#include <memory>
#include <vector>

namespace TSystem
{
	// forward declaration
	class PersistService;
	class ServiceStrand;
	class ServiceTimer;
	class ServiceSignalSet;
	class TError;

	typedef std::function<void()>								QueuedTask;
	typedef std::function<void(const TSystem::TError&)>			TimedTask;
	typedef std::function<void(const TSystem::TError&, int)>	SignaledTask;

	// task pool
	// use thread pool to execute task in the pool
	// execution order undefined
	class TaskPool
	{
	public:
	
		TaskPool();
		~TaskPool();
		
		// moving constructor
		TaskPool( TaskPool&& in );

		// accessor for PersistService, in case io_service needs to be exposed
		PersistService& Pool() const;

		//-------------------
		// worker thread related
		//-------------------    

		// add worker into thread pool, return thread id
		void AddWorker();

		// add this thread as worker, will block this thread
		void AddThisThread();

		// use this thread as worker to run only on task from pool
		// return number of task executed
		// return 0 if there is no task
		size_t RunOneInThisThread();

		// number of workers
		size_t WorkerSize() const;

		// shutdown task pool
		// set flag such that worker exits if no job
		void Shutdown();

		void FireShutdown();

		// check if shutdown 
		bool HasShutdown() const;

		// join all workers
		void Join();

		// check if worker thread has exited because of too many execution 
		// or terminated because of uncaught exception 
		// will fill the worker if the first case 
		// and throw the uncaught exception in the second case
		void CheckWorkers();

		// post non-sequential task
		// message processing order depends on the thread pool
		void PostTask(QueuedTask&& task) const;
		void PostTask(const QueuedTask& task) const;

		// dispatch task
		// the calling function is in the worker thread, task
		// will be executed within the same calling function
		void DispatchTask(QueuedTask&& task);
		void DispatchTask(const QueuedTask& task) const;

	private:

		// future of the thread and the thread
		// use future to catch exception in worker thread 
		typedef std::future<size_t>          WorkerStatus;

		// the persist service object
		std::unique_ptr<PersistService>		pool_;

		// worker threads
		std::vector<WorkerStatus>    workers_;
	};

	// task strand
	// task strand represents a subset of "queued" tasks in a task pool
	// execution order is the same as they are posted
	// however, when task is dispatched, if the calling function is running 
	// in a worker thread and there is no other task in the
	// strand running, the task will run immediately in the calling function
	// behavior like "jump queue"
	class TaskStrand
	{
	public:
		
		explicit TaskStrand( const TaskPool& pool );
		~TaskStrand();

		// moving constructor
		TaskStrand( TaskStrand&& in );

		// the task pool
		const TaskPool& GetTaskPool() const;

		// post sequential task
		void PostTask(QueuedTask&& task) const;
		void PostTask(const QueuedTask& task) const;

		// dispatch task
		// jump queue 
		void DispatchTask(QueuedTask&& task) const;
		void DispatchTask(const QueuedTask& task) const;

	private:

		// the pareent task pool 
		const TaskPool&                         task_pool_;

		// the strand object
		std::unique_ptr<ServiceStrand>    strand_;
	};

	// task timer
	// task timer represents a group of timed task in a task pool
	class TaskTimer
	{
	public:

		explicit TaskTimer( const TaskPool& pool );
		~TaskTimer();

		// moving constructor
		TaskTimer( TaskTimer&& in );

		// the task pool
		const TaskPool& GetTaskPool() const;

		// cancel all async waiting
		void Cancel() const;

		// set expire time, will cancel any async waiting
		void ExpirationTimePoint(const std::chrono::system_clock::time_point& t) const;
		void ExpirationDuration(const std::chrono::system_clock::duration& dur) const;

		// async wait
		void PostTask(TimedTask&& timed_task) const;
		void PostTask(const TimedTask& timed_task) const;

	private:

		// the pareent task pool 
		const TaskPool&                         task_pool_;

		// the timer object
		std::unique_ptr<ServiceTimer>     timer_;
	};

	// task signal 
	// task signal represents a group of tasks whchi will be trigger by system signal
	class TaskSignalSet
	{
	public:
		explicit TaskSignalSet(const TaskPool& pool);
		~TaskSignalSet();

		// moving constructor
		TaskSignalSet(TaskSignalSet&& in);

		// the task pool
		const TaskPool& GetTaskPool() const;

		// cancel all async waiting
		void Cancel() const;

		// add signal, may throw TException if add fail
		void Add(int signal_number) const;

		// async wait
		void PostTask( SignaledTask&& signaled_task ) const;
		void PostTask( const SignaledTask& signaled_task) const;

	private:
		// the parent task pool 
		const TaskPool&                         task_pool_;

		// the signal set object
		std::unique_ptr<ServiceSignalSet>     signal_set_;
	};
}

#endif //TSYSTEM_TASK_POOL_H_
