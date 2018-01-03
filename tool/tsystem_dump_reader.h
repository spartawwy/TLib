#ifndef TSYSTEM_DUMP_READER_H_
#define TSYSTEM_DUMP_READER_H_

#include <istream>

#include "TLib/core/tsystem_read_buffer.h"
#include "TLib/core/tsystem_serialization.h"

namespace TSystem
{
namespace utility
{
	class DumpReader
	{
	public:
		DumpReader(std::istream& is);

		TSystem::SerialData ParseSerialData();

	private:
		std::istream&	data_stream_;
		std::streamsize last_read_;
		ReadBuffer		buffer_;
	};
}
}

#endif // TSYSTEM_DUMP_READER_H_