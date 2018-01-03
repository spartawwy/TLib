// by Xixin Silas Cheng

#ifndef TSYSTEM_EXE_FRAME_H_
#define TSYSTEM_EXE_FRAME_H_


#include "TLib/core/tsystem_error.h"
#include "TLib/core/tsystem_appbase.h"

namespace TSystem
{
	// frame for executable
	// wrap try-catch block
	// print some formatted information for log parsing
	class ExecutableFrame
	{
	public:
		// launch normal program, no launching step stamps
		int Launch(int argc, char* argv[]);

		// launch server program, print launching step stamps
		int LaunchServer(int argc, char* argv[]);

	protected:
		ExecutableFrame();

		// the main program
		virtual int main(int argc, char* argv[]) = 0;

		// the usage
		virtual void PrintUsage(const char* exe_name){};

		// print launching done
		void PrintLaunchDone();
		
		// print app info
		void PrintAppInfo(const AppBase& app);

	private:
		
		// formatted launching step stamps
		void PrintTException(const TException& e );
		void PrintStdException(const std::exception& e);
		void PrintLaunching(int argc, char* argv[]);
		void PrintShutdown(bool is_normal);
	};
}
#endif // TSYSTEM_EXE_FRAME_H_
