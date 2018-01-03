#include "tsystem_dump_reader.h"

#include <tuple>

#include "TLib/core/tsystem_core_error.h"

using namespace TSystem::utility;
using namespace TSystem;

DumpReader::DumpReader(std::istream& data)
	: data_stream_(data)
	, last_read_(0)
	, buffer_(64*1024, sizeof(communication::SizeHeaderType))
{
    auto check = buffer_.p_ofset();
    auto of = buffer_.ofset();
    std::vector<char>& ck_rb = buffer_.read_buf();
    ck_rb[of];
    buffer_.data();

	if( data_stream_.good() )
		last_read_ = data_stream_.read( buffer_.data(), buffer_.size() ).gcount();
}

TSystem::SerialData DumpReader::ParseSerialData()
{
	while( last_read_ )
	{
		std::tuple<const char*, std::size_t> res = buffer_.ParseOneMessage(last_read_);
		if( std::get<0>(res) )
		{
			if( !CheckHeaderSize<SerialData>(std::get<1>(res)) )
			{
				ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT
					, "DumpReader::ParseSerialData"
					, "bad binary size for SerialData");
			}

			return SerialData(std::get<0>(res), std::get<1>(res));
		}else
		{
			last_read_ = data_stream_.read( buffer_.data(), buffer_.size() ).gcount();
		}
	}

	return SerialData(nullptr, 0);
}