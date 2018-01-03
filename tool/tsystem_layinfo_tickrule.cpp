#include "tsystem_layinfo_tickrule.h"

#include <algorithm>
#include <cmath>

#include <boost/math/common_factor.hpp>
#include <SQLite/sqlite_connection.h>

#include "TLib/core/tsystem_sqlite_functions.h"
#include "TLib/core/tsystem_utility_functions.h"

using namespace SQLite;
using namespace TSystem::layinfo;
using namespace TSystem;

//------------
// TickRule
//------------
TickRule::TickRule()
	: divisor_(1)
	, tic_ranges_()
{
	tic_ranges_.reserve(16);
}

int TickRule::CheckRanges() const
{
	//------------------
	// check tick rules
	//------------------

	// a) max_prc = min_prc + tick_sze * tick_num;
	static const double epsilon = 1E-12; // epsilon for compounded double operation
	if( std::find_if( std::begin(tic_ranges_), std::end(tic_ranges_)
		, [](const TickRange& range)->bool 
	{ return range.max_prc != range.min_prc + range.tic_sze*range.tic_num; }) != std::end(tic_ranges_) )
		return 1;

	// b) ranges are continues 
	if( tic_ranges_.size() > 1 )
	{
		auto iter1 = std::begin(tic_ranges_);
		auto iter2 = iter1;
		iter2++;
		while( iter2 != std::end(tic_ranges_))
		{
			if( iter1->max_prc != iter2->min_prc )
				return 2;
			iter1++;
			iter2++;
		}
	}

	return 0;
}

RationalPrice TickRule::TickPrice(double price, int tick_ofset, RoundType round_type) const
{
	price *= divisor_;

	// find tick range and tick pos in the range
	int     tick_pos;
	auto iter = std::upper_bound(std::begin(tic_ranges_), std::end(tic_ranges_), price
		,[](double price, const TickRange& range)->bool { return range.min_prc > price; });
	
	if( iter == std::begin(tic_ranges_) ) // if below lower bound of tick ranges
	{
		if( tick_ofset <= 0 )
			return RationalPrice(iter->min_prc, divisor_);

		tick_pos = 0;
	}else
	{
		--iter;
		double raw_tick_pos = (price - iter->min_prc)/iter->tic_sze;
		int floor_tick_pos = static_cast<int>(std::floor(raw_tick_pos));
		int ceil_tick_pos = static_cast<int>(std::ceil(raw_tick_pos));

		if( floor_tick_pos == ceil_tick_pos )
			tick_pos = floor_tick_pos;
		else switch (round_type)
		{
		case RoundType::CEIL:
			tick_pos =  ceil_tick_pos;
			break;
		case RoundType::FLOOR:
			tick_pos =  floor_tick_pos;
			break;
		default:
			assert(0);
			break;
		}
		assert( tick_pos >= 0 );
	}

	if( !tick_ofset )
	{
		return RationalPrice(iter->min_prc + tick_pos * iter->tic_sze, divisor_);
	}
	else if( tick_ofset > 0 )
	{
		tick_ofset += tick_pos;
		auto iter0 = std::find_if(iter, std::end(tic_ranges_), [&tick_ofset](const TickRange& range)->bool
		{
			if( tick_ofset > range.tic_num )
			{
				tick_ofset -= range.tic_num;
				return false;
			}
			else
			{
				return true;
			}
		});

		if( iter0 == std::end(tic_ranges_) )
		{
			return RationalPrice(tic_ranges_.back().max_prc, divisor_);
		}else
		{
			return RationalPrice(iter0->min_prc + tick_ofset * iter0->tic_sze, divisor_);
		}
	}else
	{
		tick_ofset = std::abs(tick_ofset) + iter->tic_num - tick_pos;
		auto iter0 = std::find_if( std::vector<TickRange>::const_reverse_iterator(++iter), tic_ranges_.rend(), [&tick_ofset](const TickRange& range)->bool
		{
			if( tick_ofset > range.tic_num )
			{
				tick_ofset -= range.tic_num;
				return false;
			}
			else
			{
				return true;
			}
		});

		if( iter0 == tic_ranges_.rend() )
		{
			return RationalPrice(tic_ranges_.front().min_prc, divisor_);
		}else
		{
			return RationalPrice(iter0->max_prc - tick_ofset * iter0->tic_sze, divisor_);
		}
	}	
}

bool TickRule::CheckPrice(const RationalPrice& price) const
{
	int multiple = divisor_ / price.d;
	if( multiple * price.d != divisor_ )
		return false;

	int64_t tot_tic = (static_cast<int64_t>(price.r) * price.d + price.n)*multiple;

	auto iter = std::upper_bound(std::begin(tic_ranges_), std::end(tic_ranges_), tot_tic
		,[](int64_t tot_tic, const TickRange& range)->bool { return range.min_prc > tot_tic; });

	if( iter == std::begin(tic_ranges_) ) // if below lower bound of tick ranges
	{
		return false;
	}else
	{
		--iter;
		if( (tot_tic - iter->min_prc) % iter->tic_sze )
			return false;
		return 
			true;
	}
}