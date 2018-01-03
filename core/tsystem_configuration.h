// by Xixin Silas Cheng
// implementation of configuration file

#ifndef TSYSTEM_CONFIGURATION_H_
#define TSYSTEM_CONFIGURATION_H_

#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace TSystem
{
	class TError;

	// configuration dictionary
	// check if lines format are correct
	class CFDictionary
	{
	public:

		// CfgSpec (name, valstr, group)
		typedef std::tuple<std::string, std::string, std::string> CfgSpec;

		CFDictionary();
		~CFDictionary();

		// load dictionary from file
		// throw std::regex_error exception when there is regex gramma error
		bool Load(const std::string& dict_file);

		// check a parameter's format and parse if provided output
		bool Parse(const std::string& param_str, TError& err, CfgSpec* spec = nullptr) const;

	private:
		std::unordered_map<std::string, std::regex> params_;
	};

	// configuration entry
	// search orders:
	// DefaultValue -> taged_map -> regex_list
	class ParamItem
	{
	public:
		static std::string                              empty_value;

		explicit ParamItem(const std::string& name);
		~ParamItem();

		ParamItem& operator=( ParamItem&& rh);
		
		// accessor
		const std::string& name() const {return name_; };

		// add value for a regex
		// later entry has higher priority
		// LogServer    192.168.0.1  .*      <- default
		// LogServer    192.168.0.2  Dev.*   <- specific for Dev.*
		void AddRegexGrp(const std::regex& pattern, const std::string& val);

		// add value for a named tag
		void AddTagedGrp(const std::string& tag, const std::string& val);

		// add default value
		void AddDefault(const std::string& val);
		
		// read / test default value
		const std::string& Val()  const;

		// read / test value for a tag, will search named tag then regex
		const std::string& Val(const std::string& grpname) const;

	private:
		typedef std::tuple<std::regex, std::string>		RegexPair;

		std::string										name_;
		std::vector<RegexPair>							regex_list_;
		std::unordered_map<std::string, std::string>	taged_map_;
	};

	// configuration file
	// save all config parameters
	class CFParams
	{
	public:
		explicit CFParams(const CFDictionary& dict);
		~CFParams();

		// load parameter, parse with a dictionary 
		// throw TException when there is bad config line
		bool Load(const std::string& cfg_file);  

		// read parameter 
		const std::string& GetParamStr(const std::string& name, const std::string& group) const;

	private:
		const CFDictionary& dict_;
		std::unordered_map<std::string, ParamItem>	params_;
	};
}

#endif //TSYSTEM_CONFIGURATION_H_