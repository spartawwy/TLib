#ifndef TSYSTEM_CORE_PATHS_H_
#define TSYSTEM_CORE_PATHS_H_

#include <string>

#include "TLib/core/tsystem_utility_functions.h"

namespace TSystem
{
namespace utility
{
	// dev status, updated at compile time
	extern const std::string& DevTag();
	inline const std::string&  DevTag()
	{
#if defined(PRODUCTION_BUILD_) && !defined(DEVTAG_)
		static std::string s = "Prod";
#elif defined(DEVTAG_)
		static std::string s = DEVTAG_;
#else
		static std::string s = "Dev";
#endif
		return s;
	}

	// if production build
	inline bool  IsProd()
	{
		return DevTag() == "Prod";
	}

	// get / set project tag, default to empty tag
	// check /assert the returned tag if necessary
	const std::string& ProjectTag(const char* p_tag = nullptr);
	
	// K dir
	inline const std::string& KDir()
	{
		static std::string s = "Z://";
		return s;
	}

	// project dir
	inline std::string ProjectDir()
	{
		return FormatStr("%s%s//", KDir().c_str(), ProjectTag().c_str());
	}

	// c dir
	inline std::string LocalDir()
	{
		return FormatStr("C://local_files//%s//", ProjectTag().c_str());
	}

	// working root dir
	inline std::string RootDir()
	{
		return FormatStr("%s%s//", ProjectDir().c_str(), DevTag().c_str());
	}

	// local shared dir
	inline std::string MemoryMapDir()
	{
		return LocalDir() + "memory_map//";
	}

	inline std::string DataDir()
	{
		return ProjectDir() + "data//";
	}

	inline std::string DatabaseDir()
	{
		return DataDir() + "database//";
	}

	inline std::string RootConfigDir()
	{
		return RootDir() + "config//";
	}

	inline std::string RootLogDir()
	{
		return RootDir() + "logs//";
	}

	inline std::string RootTempDir()
	{
		static std::string s = RootDir() + "temp//";
		return s;
	}

	// create local dir structure
	inline void CreateLocalDirs()
	{
		CreateDir(LocalDir());
		CreateDir(MemoryMapDir());
	}

	// create root dir structure
	inline void CreateRootDirs()
	{
		CreateDir(ProjectDir());
		CreateDir(RootDir());
		CreateDir(RootConfigDir());
		CreateDir(RootLogDir());
		CreateDir(RootTempDir());
	}
}
}

#endif //TSYSEM_CORE_PATHS_H_