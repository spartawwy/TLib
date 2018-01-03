#include "tsystem_layinfo_tradehour.h"

#include <algorithm>

#include <SQLite/sqlite_connection.h>

#include "TLib/core/tsystem_core_paths.h"
#include "TLib/core/tsystem_core_error.h"
#include "TLib/core/tsystem_time.h"

#include "TLib/tool/tsystem_layinfo.h"

using namespace SQLite;
using namespace TSystem::layinfo;
using namespace TSystem;

//------------------------
// IntradayTimePoint
//------------------------

IntradayTimePoint::IntradayTimePoint()
	: day_offset(0), time_of_day(),time_with_day()
{};

IntradayTimePoint::IntradayTimePoint(IntradayTimePoint&& in)
	: day_offset(in.day_offset)
	, time_of_day(std::move(in.time_of_day))
	, time_with_day(std::move(in.time_with_day))
{}

IntradayTimePoint TSystem::layinfo::MakeIntradayTimePoint(int day_offset, int hour, int min, int sec)
{
	IntradayTimePoint res;
	res.day_offset = day_offset;
	res.time_of_day = std::chrono::hours(hour) + std::chrono::minutes(min) + std::chrono::seconds(sec);
	return res;
}

//--------------
// TradeSession
//--------------

TradeSession::TradeSession()
	: open_time()
	, open_type(OpenCloseType::REGULAR_OPN)
	, close_time()
	, close_type(OpenCloseType::REGULAR_CLS)
{}

TradeSession::TradeSession( TradeSession&& in )
	: open_time( std::move(in.open_time) )
	, open_type( in.open_type )
	, close_time( std::move(in.close_time) )
	, close_type( in.close_type )
{}


const std::string& TSystem::layinfo::OpenCloseTypeToString( TradeSession::OpenCloseType t)
{
	static std::string regular_open = "REGL_O";
	static std::string regular_close = "REGL_C";
	static std::string break_open = "BREK_O";
	static std::string break_close = "BREK_C";

	switch (t)
	{
	case TSystem::layinfo::TradeSession::OpenCloseType::REGULAR_OPN:
		return regular_open;
	case TSystem::layinfo::TradeSession::OpenCloseType::REGULAR_CLS:
		return regular_close;
	case TSystem::layinfo::TradeSession::OpenCloseType::BREAK_OPN:
		return break_open;
	case TSystem::layinfo::TradeSession::OpenCloseType::BREAK_CLS:
		return break_close;
	}

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "OpenCloseTypeToString"
		, utility::FormatStr("unknown OpenCloseType: %d", static_cast<short>(t))); 

	// should never come to this line
	assert(0);
	return regular_open;
}

TradeSession::OpenCloseType TSystem::layinfo::StringToOpenCloseType( const std::string& s )
{
	static std::string regular_open = "REGL_O";
	static std::string regular_close = "REGL_C";
	static std::string break_open = "BREK_O";
	static std::string break_close = "BREK_C";

	if( s == regular_open)
		return TradeSession::OpenCloseType::REGULAR_OPN;
	else if( s == regular_close)
		return TradeSession::OpenCloseType::REGULAR_CLS;
	else if( s == break_open)
		return TradeSession::OpenCloseType::BREAK_OPN;
	else if( s == break_close)
		return TradeSession::OpenCloseType::BREAK_CLS;
	
	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "StringToOpenCloseType"
		, utility::FormatStr("unknown OpenCloseType string: %s", s.c_str())); 

	// should never come to this line
	assert(0);
	return TradeSession::OpenCloseType::REGULAR_OPN;
}

//--------------
// TradeHour
//--------------
TradeHour::TradeHour(const std::shared_ptr<Exchange>& p_exchange)
	: name_()
	, exchange_(p_exchange)
	, trading_date_(0)
	, trade_sessions_()
{
	if( !p_exchange )
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
		"TradeHour::TradeHour",
		"empty exchange pointer");

	trade_sessions_.reserve(8);
}

void TradeHour::InitiateSessions(int longdate)
{
	assert( exchange_->IsBusDay(longdate) );

	// day offset to timepoint
	std::map<int, std::chrono::system_clock::time_point> offset_map;
	auto offset_to_tp = [&offset_map, this](int offset)->const std::chrono::system_clock::time_point&
	{
		auto iter = offset_map.find(offset);
		if( iter == std::end(offset_map) )
		{
			DateComponent day_part = FromLongdate(this->exchange_->NextBusDay(this->trading_date_, offset));
			iter = offset_map.insert(std::make_pair(offset,  MakeTimePoint(std::get<0>(day_part), std::get<1>(day_part), std::get<2>(day_part)))).first;
		}

		return iter->second;
	};

	trading_date_ = longdate;

	// initiate time_with_day
	for( auto& session : trade_sessions_ )
	{
		session.open_time.time_with_day = offset_to_tp(session.open_time.day_offset) + session.open_time.time_of_day;
		session.close_time.time_with_day = offset_to_tp(session.close_time.day_offset) + session.close_time.time_of_day;
	}
}

bool TradeHour::IsTradingHour(const std::chrono::system_clock::time_point& time) const
{
	auto iter = std::lower_bound(std::begin(trade_sessions_), std::end(trade_sessions_), time
		, [this](const TradeSession& session, const std::chrono::system_clock::time_point& time)->bool
	{ return session.close_time.time_with_day < time; });
	
	if( iter == trade_sessions_.end() )
	{ return false; }
	else if( iter->close_time.time_with_day == time )
	{ return true;  }
	else 
	{ return iter->open_time.time_with_day <= time; }
}

int	TradeHour::ClearingDate(const std::chrono::system_clock::time_point& time) const
{
		return (time < trade_sessions_.front().open_time.time_with_day) 
					? exchange_->NextBusDay(trading_date_, -1)
					: trading_date_;
}