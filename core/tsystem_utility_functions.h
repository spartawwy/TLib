// By Xixin Silas Cheng
// utility function

#ifndef TSYSTEM_UTILITY_FUNCTION_H_
#define TSYSTEM_UTILITY_FUNCTION_H_

#include <string>
#include "TLib/core/tsystem_string_functions.h"

namespace TSystem
{
namespace utility
{
	// create file dir
	void CreateDir(const std::string& dir);

	// host name, may throw TException when fail to get host name
	std::string host();

	// compile info, updated at compile time
	extern std::string CompileTime();
	inline std::string CompileTime()
	{
		return TSystem::utility::FormatStr("Compiled at %s %s", __DATE__, __TIME__);
	}
}
}

#endif // TSYSTEM_UTILITY_FUNCTION_H_