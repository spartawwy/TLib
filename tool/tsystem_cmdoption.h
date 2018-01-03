// By Xixin Silas Cheng
// a simple command line option parser

#ifndef TSYSTEM_CMDOPTION_H_
#define TSYSTEM_CMDOPTION_H_

#include <unordered_map>
#include <string>
#include <vector>

namespace TSystem
{
namespace utility
{
	class ParsedOptions
	{
	public:

		ParsedOptions( ParsedOptions&& in );

		// check if one option is provided
		bool Exist(const std::string& opt) const;

		// get value or default value, throw exception for boost
		const std::string&  GetStr(const std::string& opt) const;
		int                 GetInt(const std::string& opt) const;
		double              GetDouble(const std::string& opt) const;

		// remaining string
		const std::vector<std::string>& remain() const { return remains_; }
		std::vector<std::string>& remain() { return remains_; }

	private:

		ParsedOptions();

		// opt_name -> (has_value, value)
		std::unordered_map<std::string, std::tuple<bool, std::string>>  found_opts_;
		std::vector<std::string>	remains_;
		
		friend class CmdLineOption;
	};

	// command line option setting
	// support "-xxx" and "--xxx" format
	// option can have no value / scalar value
	// in case of need to input array value, 
	// seperate values by comma, and make sure
	// the value contains NO SPACE like "1,2,3,4"
	class CmdLineOption
	{
	public:

		CmdLineOption();

		// add option, option name / help info / has_value
		void AddOption(const std::string& opt, const std::string& help_info);

		// add option, option name / help info / default_value
		void AddOption(const std::string& opt, const std::string& help_info, const std::string& default_value);

		// print option help info
		std::string PrintHelp()	const;

		// will throw TException if no value found for option that needs a value
		// or on an unknown option
		template< typename IterType>
		ParsedOptions ParseArgv(const IterType& iter1, const IterType& iter2 ) const
		{
			ParsedOptions output;
			output.remains_.reserve(10);

			// current processing option
			OptionSetting* p_cur_opt(nullptr);

			std::for_each(iter1, iter2, [this, &p_cur_opt, &output](const std::string& word)
			{ ParseOneWord(word, p_cur_opt, output); });

			return output;
		}

		template< typename IterType, typename SizeType>
		ParsedOptions ParseArgv(const IterType& iter, SizeType size ) const
		{
			return ParseArgv(iter, iter+size);
		}

	private:

		// option struct
		struct OptionSetting
		{
			OptionSetting(const std::string& name, const std::string& help_info
				, bool has_value, const std::string& default_value)
				: name(name) 
				, help_info(help_info)
				, has_value(has_value)
				, value(default_value)
			{}

			OptionSetting(OptionSetting&& in)
				: name(std::move(in.name))
				, help_info(std::move(in.help_info))
				, has_value(in.has_value)
				, value(std::move(in.value))
			{}

			std::string name;		   // option name
			std::string help_info;     // help info
			bool        has_value;     // has value
			std::string value;         // option value
		};

		// will throw TException if no value found for option that needs a value
		// or on an unknown option
		void ParseOneWord(const std::string& word, OptionSetting*& p_cur_opt, ParsedOptions& output ) const;

		std::unordered_map<std::string, OptionSetting>      opts_;
	};
}
}

#endif // TSYSTEM_CMDOPTION_H_
