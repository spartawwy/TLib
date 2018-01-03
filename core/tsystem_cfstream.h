// By Xixin Silas Cheng
// a text file stream reader

#ifndef TSYSTEM_CFSTREAM_H_
#define TSYSTEM_CFSTREAM_H_

#include <fstream>
#include <vector>
#include <string>

namespace TSystem
{
namespace utility
{
	// configuration file stream
	// can handle c++ type coment, and #include preprocesser
	class CFStream
	{
	public:
		CFStream();
		~CFStream();

		// open file, throw 
		bool Open(const std::string& file);

		// close file
		void Close();

		// next non-empty line, hanlde all preprocesser
		// throw TException when preprocessor is wrong
		bool ReadLine();				

		// get line string
		const std::string& GetLineStr() const { return line_; }

	private:
			
		bool IsPreProc(std::string& pre_cmd, std::string& rest) const;

		std::vector<std::ifstream>   files_;
		std::string					 line_;
	};
}
}
#endif // TSYSTEM_CFSTREAM_H_