#include "tsystem_cmdoption.h"

#include <algorithm>
#include <sstream>
#include <regex>

#include <boost/lexical_cast.hpp>

#include "TLib/core/tsystem_core_error.h"


using namespace TSystem::utility;
using namespace TSystem;

//---------------------
// class ParsedOptions
//---------------------
ParsedOptions::ParsedOptions()
	: found_opts_(24)
	, remains_()
{}

ParsedOptions::ParsedOptions( ParsedOptions&& in )
	: found_opts_(std::move(in.found_opts_))
	, remains_(std::move(in.remains_))
{}

bool ParsedOptions::Exist(const std::string& opt) const
{
	return found_opts_.find(opt) != found_opts_.end();
}


const std::string& ParsedOptions::GetStr(const std::string& opt) const
{
	const auto& iter = found_opts_.find(opt);
	assert( iter != found_opts_.end() );
	assert( std::get<0>(iter->second) );
	return std::get<1>(iter->second);
}

int ParsedOptions::GetInt(const std::string& opt) const
{
	const auto& iter = found_opts_.find(opt);
	assert( iter != found_opts_.end() );
	assert( std::get<0>(iter->second) );
	try
	{ return boost::lexical_cast<int>(std::get<1>(iter->second)); }
	catch( const boost::bad_lexical_cast& e )
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT,
			"ParsedOptions::GetInt", 
			FormatThirdPartyError("boost", 1, e.what()) );
	}
}

double ParsedOptions::GetDouble(const std::string& opt) const 
{
	const auto& iter = found_opts_.find(opt);
	assert( iter != found_opts_.end() );
	assert( std::get<0>(iter->second) );
	try
	{ return boost::lexical_cast<double>(std::get<1>(iter->second)); }
	catch( const boost::bad_lexical_cast& e )
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT,
			"ParsedOptions::GetDouble", 
			FormatThirdPartyError("boost", 1, e.what()) );
	}
}


//---------------------
// class CmdLineOption
//---------------------
CmdLineOption::CmdLineOption()
	: opts_(24)
{}

void CmdLineOption::AddOption(const std::string& name, const std::string& help_info)
{
	bool res = opts_.insert( std::make_pair(name, OptionSetting(name, help_info, false, ""))).second;
	assert(res);
}

void CmdLineOption::AddOption(const std::string& name, const std::string& help_info, const std::string& default_value)
{
	bool res = opts_.insert( std::make_pair(name, OptionSetting(name, help_info, true, default_value))).second;
	assert(res);
}

std::string CmdLineOption::PrintHelp() const
{
	std::stringstream out;
	char msg[1024];
	sprintf_s(msg, 1024, "%s", "Options:");
	out << msg << std::endl;

	for( const auto& opt : opts_ )
	{
		sprintf_s(msg, 1024, "-%-5s %8s : %s", opt.first.c_str(), opt.second.value.c_str(), opt.second.help_info.c_str());
		out << msg << std::endl;
	}

	return out.str();
}

void CmdLineOption::ParseOneWord(const std::string& word, OptionSetting*& p_cur_opt, ParsedOptions& output ) const
{
	const static std::regex opt_regex1("-([A-Za-z]+[\\w]*)");
	const static std::regex opt_regex2("--([A-za-z]+[\\w]*)");

	std::smatch match_res;
	if( std::regex_match( word, match_res, opt_regex1) ||
		std::regex_match( word, match_res, opt_regex2) )  // if an option word
	{
		if( p_cur_opt ) // if still finding value for previous option, throw
		{
			ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT
					, "CmdLineOption::ParseArgv"
					, "No value specified for: " + p_cur_opt->name);
		}

		std::string name = match_res[1];
		auto iter = opts_.find(name);
		if( iter != opts_.end() )
		{
			output.found_opts_.insert(std::make_pair(iter->second.name, std::make_tuple(iter->second.has_value, iter->second.value))); 
			if( iter->second.has_value )
				p_cur_opt = const_cast<OptionSetting*>(&iter->second);  // begin searching value
		}else
		{   // unknown optoin, throw
			ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT
				, "CmdLineOption::ParseArgv"
				, "Unknown option: " + name);
		}
	}else   // if an non-option word
	{
		if( p_cur_opt )   // save value for previous option and end searching value
		{
			auto iter = output.found_opts_.find(p_cur_opt->name);
			assert( iter != output.found_opts_.end() && std::get<0>(iter->second) );

			std::get<1>(iter->second) = word;
			p_cur_opt = nullptr;
		}else            // save word into remain
		{
			output.remains_.push_back(word);
		}
	}
}