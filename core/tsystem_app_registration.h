// by Xixin Silas Cheng

#ifndef TSYSTEM_APP_REGISTRATION_H_
#define TSYSTEM_APP_REGISTRATION_H_

#include <memory>
#include <string>

namespace TSystem
{
	class SharedMemory;

	class AppRegistration
	{
	public:

		// throw TException when shared memroy fail
		AppRegistration();
		~AppRegistration();

		// register pid in shared_memory
		bool CheckAndRegisterPid(unsigned short pid);

	private:
		// clean registraion, remove entrys not in living process id
		// throw TException when shared memroy fail
		void CleanRegistration();

		// the shared memory for process registration
		std::unique_ptr<TSystem::SharedMemory> registration_;
	
		// the local pointer to shared memory
		int* p_pid_;
	};
}

#endif