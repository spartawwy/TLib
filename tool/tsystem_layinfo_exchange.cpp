#include "tsystem_layinfo_exchange.h"

#include <algorithm>
#include <iostream>

#include "TLib/core/tsystem_core_common.h"

using namespace TSystem;
using namespace TSystem::layinfo;

Exchange::Exchange()
	: name_(), time_zone_(), city_()
	, exch_cal_dates_(), exch_bus_dates_()
{
	exch_cal_dates_.reserve(2048);
	exch_bus_dates_.reserve(2048);
};

int Exchange::DifCalDays( int date_1, int date_2 ) const
{
	std::vector<int>::const_iterator iter1 = std::lower_bound( std::begin(exch_cal_dates_), std::end(exch_cal_dates_), date_1 );
	if( iter1 == exch_cal_dates_.end() || *iter1 != date_1 )
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
			"ExchangeCalendar::DifCalDays",
			utility::FormatStr("bad date_1: %d", date_1) );
	}

	std::vector<int>::const_iterator iter2 = std::lower_bound( std::begin(exch_cal_dates_), std::end(exch_cal_dates_), date_2 );
	if( iter2 == exch_cal_dates_.end() || *iter2 != date_2 )
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
			"ExchangeCalendar::DifCalDays",
			utility::FormatStr("bad date_2: %d", date_2) );
	}

	return static_cast<int>(std::distance( iter1, iter2 ));
}


int Exchange::DifBusDays( int date_1, int date_2 ) const
{
	std::vector<int>::const_iterator iter1 = std::lower_bound( std::begin(exch_bus_dates_), std::end(exch_bus_dates_), date_1 );
	if( iter1 == exch_bus_dates_.end() )
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
			"ExchangeCalendar::DifBusDays",
			utility::FormatStr("bad date_1: %d", date_1) );
	}
	if( *iter1 != date_1 ) iter1--;

	std::vector<int>::const_iterator iter2 = std::lower_bound( std::begin(exch_bus_dates_), std::end(exch_bus_dates_), date_2 );
	if( iter2 == exch_bus_dates_.end() )
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
			"ExchangeCalendar::DifBusDays",
			utility::FormatStr("bad date_2: %d", date_2) );
	}
	if( *iter2 != date_2 ) iter2--;

	return static_cast<int>(std::distance( iter1, iter2 ));
}

int Exchange::NextBusDay( int date, int ndays ) const
{
	std::vector<int>::const_iterator iter = std::lower_bound( exch_bus_dates_.begin(), exch_bus_dates_.end(), date );
	if( iter == exch_bus_dates_.end() )
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
						"ExchangeCalendar::NextBusDay",
						utility::FormatStr("out of date range, undefined date: %d", date) );

	if( (ndays >= 0 && ndays > std::distance(iter, std::end(exch_bus_dates_) - 1))
		|| ( ndays < 0 && abs(ndays) > std::distance(std::begin(exch_bus_dates_), iter)) )
			ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
									"ExchangeCalendar::NextBusDay",
									utility::FormatStr("beyond vector range given date: %d and distance %d", date, ndays) );

	return (*iter != date && ndays > 0) ? *(iter + ndays - 1) : *(iter + ndays);
}

int Exchange::NextCalDay( int date, int ndays ) const
{
	std::vector<int>::const_iterator iter = std::lower_bound( std::begin(exch_cal_dates_), std::end(exch_cal_dates_), date );
	if( iter == exch_cal_dates_.end() )
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
						"ExchangeCalendar::NextCalDay",
						utility::FormatStr("bad date: %d", date) );

	if( (ndays >= 0 && ndays > std::distance(iter, std::end(exch_cal_dates_) - 1 ))
		|| ( ndays < 0 && abs(ndays) > std::distance( std::begin(exch_cal_dates_), iter )) )
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
						"ExchangeCalendar::NextCalDay",
						utility::FormatStr("beyond vector range given date: %d and distance %d", date, ndays) );

	return *(iter + ndays);
}

bool Exchange::IsBusDay( int date ) const 
{ return std::binary_search( std::begin(exch_bus_dates_), std::end(exch_bus_dates_), date ); }

