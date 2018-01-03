// By Xixin Silas Cheng
// header file for reading configuration file

#ifndef TSYSTEM_APP_CONFIG_H_
#define TSYSTEM_APP_CONFIG_H_

#include <atomic>
#include <chrono>
#include <vector>
#include "TLib/core/tsystem_configuration.h"

namespace TSystem
{
	// a dictionary and configuration pair, with config search dir and load function.
	// to cater the univerality and complexity of config string stored in the config file,
	// the GetVal() functions do realtime type conversion, which has performance issues.
	// The right way to use this config file is to set up a check point when objects, which need
	// to read parameters from this, check the last_update(), and update parameters from this into
	// their own config data structure. Notice that Load() and GetVal() functions are not thread safe

	// sample dictionary file
	//	FrontEndTradeServer		^\S+$
	//	ExchangeID				^\S+$
	//	AccountID				^\d+$
	//	Password				^\d+$
	//	BrokerID				^\d+$
	//	InvestorID				^\d+$

	// sample config file
	//	FrontEndTradeServer		tcp://180.168.146.178:31205
	//	ExchangeID              CFFEX
	//	ExchangeID              CFFEX						\*+ctp\
	//  // here is the comment line
	//	AccountID				<00016, 1027, 1023>
	//	Password                123456
	//	BrokerID                0001
	//	InvestorID              000000000016

	class AppConfiguration
	{
	public:
		AppConfiguration();
		~AppConfiguration();

		// add config search path
		void AddDir(const std::string& path);

		// load dict and config
		// throw TException when can't find files in dirs or bad parameter setting
		bool Load(const std::string& name);

		// last update time
		std::chrono::system_clock::time_point last_update() const { return last_update_; }

		// read / test default value
		bool	GetVal(const std::string& name, int* res = nullptr) const;
		bool    GetVal(const std::string& name, double* res = nullptr) const;
		bool	GetVal(const std::string& name, std::string* res = nullptr) const;

		// versions throw TException
		void	GetVal(const std::string& name, int& res) const;
		void	GetVal(const std::string& name, double& res) const;
		void	GetVal(const std::string& name, std::string& res) const;

		// read / test value for named tag
		bool	GetVal(const std::string& name, const std::string& group, int* res = nullptr) const;
		bool    GetVal(const std::string& name, const std::string& group, double* res = nullptr) const;
		bool	GetVal(const std::string& name, const std::string& group, std::string* res = nullptr) const;

		// versions throw exception
		void	GetVal(const std::string& name, const std::string& group, int& res) const;
		void	GetVal(const std::string& name, const std::string& group, double& res) const;
		void	GetVal(const std::string& name, const std::string& group, std::string& res) const;

	private:
		// configuration file search dir
		std::vector<std::string>    cfg_dirs_;

		// the dictionary
		CFDictionary			  dict_;

		// the parameter
		CFParams			      params_;

		// last time update
		std::atomic<std::chrono::system_clock::time_point>       last_update_;
	};

	// load config setting, throw TException if fail
	class AppInfo; 
	void LoadAppConfig(AppInfo& info, AppConfiguration& config);
}

#endif // TSYSTEM_APP_CONFIG_H_