#ifndef TSYSTEM_STRING_FUNCTIONS_H_
#define TSYSTEM_STRING_FUNCTIONS_H_

#include <cassert>
#include <cstdarg>
#include <vector>

namespace TSystem
{
namespace utility
{
	// wrapper of traditional sprintf, use this function to return buffer directly, avoid two steps code
	inline const char*  FormatCStr(char* buffer, std::size_t len, const char* format, ...)
	{
		va_list args;
		va_start (args, format);
#ifdef NDEBUG
		vsprintf_s(buffer, len, format, args);
#else
		assert(vsprintf_s(buffer, len, format, args) != -1 );
#endif
		va_end (args);
		return buffer;
	}

	// the performance of this function is very low, do not call this one in
	// high frequency / high performance functions
	inline std::string FormatStr(const char* format, ...)
	{
		char buffer[1024];
		va_list args;
		va_start (args, format);
		vsprintf_s(buffer,format, args);
		va_end (args);
		return buffer;
	}

	// split, seperator = ' ', quotation = '"'
	// the performance of this function is very low, do not call this one in
	// high frequency / high performance functions
	std::vector<std::string> split(const std::string& line, const std::string& seperator = " ", 
		const std::string& quotation = "\"");
	
    std::string&   replace_all(std::string& str, const std::string& old_value, const std::string& new_value);
    std::string&   replace_all_distinct(std::string& str, const  std::string& old_value, const std::string& new_value);
}
}

#endif // TSYSTEM_STRING_FUNCTIONS_H_