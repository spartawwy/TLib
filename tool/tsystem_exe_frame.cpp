#include "tsystem_exe_frame.h"

#include <algorithm>
#include <iostream>



using namespace TSystem;


ExecutableFrame::ExecutableFrame()
{}


int ExecutableFrame::Launch(int argc, char* argv[])
{	
	try
	{
		int res = main(argc, argv);
		return res;
	}catch(const TException& e)
	{
		PrintTException(e);
		return -1;
	}catch(const std::exception& e)
	{
		PrintStdException(e);
		return -1;
	}
}

int ExecutableFrame::LaunchServer(int argc, char* argv[])
{
	try
	{
		PrintLaunching(argc, argv);
		int res = main(argc, argv);
		PrintShutdown(true);
		return res;
	}catch(const TException& e)
	{
		PrintTException(e);
		PrintShutdown(false);
		return -1;
	}catch(const std::exception& e)
	{
		PrintStdException(e);
		PrintShutdown(false);
		return -1;
	}
}

void ExecutableFrame::PrintAppInfo(const AppBase& app)
{
	std::cout << "AppInfo: " << BuildInfo(app) << std::endl; 
}

void ExecutableFrame::PrintLaunchDone()
{
	std::cout << "Launched" << std::endl;
}

void ExecutableFrame::PrintTException(const TException& e )
{
	std::cout << "Exception: TSystem::TException | " << ErrorString(e.error()) << std::endl;
}

void ExecutableFrame::PrintStdException(const std::exception& e)
{
	std::cout << "Exception: std::exception | " << e.what() << std::endl;
}

void ExecutableFrame::PrintLaunching(int argc, char* argv[])
{
	std::cout << "Launching:";
	std::for_each(argv, argv+argc, [](const char* arg)
	{
		std::cout << " " << arg;
	});
	std::cout << std::endl;
}

void ExecutableFrame::PrintShutdown(bool is_normal)
{
	if( is_normal )
	{
		std::cout << "Shutdown: normal" << std::endl;
	}else
	{
		std::cout << "Shutdown: exception" << std::endl;
	}
}