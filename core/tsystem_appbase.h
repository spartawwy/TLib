// By Xixin Silas Cheng
// header file for AppBse

#ifndef TSYSTEM_APPBASE_H_
#define TSYSTEM_APPBASE_H_

#include "TLib/core/tsystem_app_config.h"
#include "TLib/core/tsystem_task_service.h"
#include "TLib/core/tsystem_local_logger.h"

namespace TSystem
{
	// app information 
	class AppInfo
	{
	public:
		// type
		const std::string&  type() const { return type_; }

		// name
		const std::string&  name() const { return name_; }
		
		// version
		const std::string&  version() const { return version_; }

		// node
		const std::string&  node() const { return node_; }
		void                node(const std::string& n) { node_ = n; }

		// pid
		unsigned short      pid() const { return pid_; }
		void                pid(unsigned short pid) { pid_ = pid;  }

		// port
		unsigned short      port() const { return port_; }
		void                port(unsigned short n) { port_ = n; }

		// work dir
		const std::string& work_dir() const { return work_dir_; }
		void work_dir(const std::string& dir) { work_dir_ = dir; }

		// config dir
		const std::string& config_dir() const { return config_dir_; }
		void config_dir(const std::string& dir) { config_dir_ = dir; }

		// log dir
		const std::string& log_dir() const { return log_dir_; }
		void log_dir(const std::string& dir) { log_dir_ = dir; }

	protected:
		AppInfo(const std::string& type, const std::string& name, const std::string& version);

	private:
		// type of application
		std::string                     type_;

		// name of application
		std::string						name_;

		// version
		std::string                     version_;

		// work dir
		std::string						work_dir_;

		// config dir
		std::string                     config_dir_;

		// log dir
		std::string                     log_dir_;

		// node of the app
		std::string                     node_;

		// resource id
		unsigned short                  pid_;

		// server port if any
		unsigned short                  port_;
	};

	// the base class for an application, application should be derived from this
	// it has built in 
	// 1. thread pool, for task-based multi-threading. TLib discourage the use of lock
	// use lock only if async-task approach is almost impossible. 
	// 2. async-logger
	// 3. dictionary based configs
	//
	// when Initiate(), it will
	// 1. create application dirs based on DirType
	// 2. load config files in application dirs if option_load_config is true
	// 3. create one more extra worker thread, so by default there are two worker thread
	//
	// Default: work_dir = config_dir = log_dir = ./
	// STAND_ALONE_APP: work_dir = ./, log_dir = $RootLogDir , config_dir = $RootConfigDir/$AppName
	// SERVER_APP: work_dir = log_dir = $AppDir, config_dir = $RootConfigDir/$AppName

	// in derived class, remember to call FireShutdown in the destructor of last layer of 
	// implementation in order to catch exception from worker thread and release resources properly 
	class AppBase: public AppInfo
	{
	public:

		enum class DirType : char
		{
			STAND_ALONE_APP     =  0,
			SERVER_APP       
		};

		virtual ~AppBase();

		// shutdown all component, should not throw any exception
		virtual void Shutdown();

		// wait all task in task_pool_ finish
		void WaitShutdown();

		// stop all task, stop immediately.
		void FireShutdown();

		// has shutdown
		bool HasShutdown() const { return shutdown_; }

		const TaskPool& task_pool() const { return task_pool_; }

		// configs might be reloaded on fly. Unless you make sure configs_ is not
		// absolutely not loaded on fly, do not use this accessor when service is running
		// see ServerClientAppBase's OnLoadConfigs() declearation;
		const AppConfiguration& configs() const { return configs_; }
		
		LocalLogger& local_logger() { return local_logger_; }

	protected:
		
		AppBase(const std::string& type, const std::string& name, const std::string& version);

		void Initiate();

		// init options
		void option_dir_type(DirType t) { std::get<0>(init_option_) = t; }
		DirType option_dir_type() const { return std::get<0>(init_option_); }

		void option_load_config(bool b) { std::get<1>(init_option_) = b; }
		bool option_load_config() const { return std::get<1>(init_option_); }

		// configurations
		AppConfiguration configs_;

		// io_service
		TaskPool		 task_pool_;

		// logger
		LocalLogger	     local_logger_;

	private:

		std::atomic_bool	shutdown_;
		TaskSignalSet		shutdown_sig_;

		//(dir_type, load_config) default = (ServerApp, false)
		std::tuple<DirType, bool>	init_option_;
	};

	//---------------------------------------------------------
	// free utility function for AppInfo and AppBase
	//---------------------------------------------------------

	// appdir
	std::string   AppDir(const AppInfo& app);

	// print version string
	std::string   BuildInfo(const AppInfo& app);
}

#endif // TSYSTEM_APPBASE_H_
