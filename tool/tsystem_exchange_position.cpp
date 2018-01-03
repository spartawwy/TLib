#include "tsystem_exchange_position.h"
#include "tsystem_posinfo.h"

using namespace TSystem;
using namespace TSystem::position;


ExchangePosition::operator bool() const
{
	return conid_ != 0 && longdate_ > 0;
}

void ExchangePosition::UpdateExchangePosition(OpenClose oc, BuySell bs, unsigned int qty)
{
	// lambda to check if enought position to close
	static auto update_close_position = []( unsigned int& a, unsigned int& b, unsigned int& c, unsigned int qty)
	{
		if( a >= qty )
		{
			a -= qty;
		}else
		{
			qty -= a; a = 0; 
			if( b >= qty )
				b -= qty;
			else
			{
				qty -= b; b = 0;
				c += qty;
			}
		}
	};

	static auto update_close_intraday = []( unsigned int& a, unsigned int& b, unsigned int qty)
	{
		if( a >= qty )
		{ a -= qty; }
		else
		{ qty -= a; a = 0; b+=qty;}
	};

	switch (oc)
	{
	case OPEN:
		{
			switch (bs)
			{
			case BUY:
				current_position_.intraday_long += qty;
				break;
			case SELL:
				current_position_.intraday_short += qty;
				break;
			}
			break;
		}
	case CLOSE:
		{
			switch (bs)
			{
			case BUY:
				update_close_position( current_position_.overnight_short, current_position_.intraday_short, current_position_.intraday_long, qty);
				break;
			case SELL:
				update_close_position( current_position_.overnight_long, current_position_.intraday_long, current_position_.intraday_short, qty);
				break;
			}
			break;
		}
	case CLOSE_OVERNIGHT:
		{
			switch (bs)
			{
			case BUY:
				update_close_position( current_position_.overnight_short, current_position_.intraday_short, current_position_.intraday_long, qty);
				break;
			case SELL:
				update_close_position( current_position_.overnight_long, current_position_.intraday_long, current_position_.intraday_short, qty);
				break;
			}
			break;
		}
	case CLOSE_INTRDAY:
		{
			switch (bs)
			{
			case BUY:
				update_close_intraday( current_position_.intraday_short, current_position_.intraday_long, qty);
				break;
			case SELL:
				update_close_intraday( current_position_.intraday_long, current_position_.intraday_short, qty);
				break;
			}
			break;
		}
	default:
		break;
	}
}

ExchangePositionManager::ExchangePositionManager()
	: id_map_(1024)
	, exchange_position_holder_()
{
	exchange_position_holder_.reserve(1024);
}

ExchangePositionManager::ExchangePositionManager(ExchangePositionManager&& in)
	: id_map_(std::move(in.id_map_))
	, exchange_position_holder_(std::move(in.exchange_position_holder_))
{
}

ExchangePositionManager& ExchangePositionManager::operator=(ExchangePositionManager&& in)
{
	if( this != &in )
	{
		id_map_ = std::move(in.id_map_);
		exchange_position_holder_ = std::move(in.exchange_position_holder_);
	}

	return *this;
}

const ExchangePosition* ExchangePositionManager::FindPosition(int conid) const
{
	auto iter = id_map_.find(conid);
	return iter != id_map_.end() ? iter->second : nullptr;
}

ExchangePosition* ExchangePositionManager::FindPosition(int conid)
{
	auto iter = id_map_.find(conid);
	return iter != id_map_.end() ? iter->second : nullptr;
}

void ExchangePositionManager::MapExchangePositions()
{
	// sort by conid
	std::sort(std::begin(exchange_position_holder_), std::end(exchange_position_holder_), 
		[](const std::unique_ptr<ExchangePosition>& left, const std::unique_ptr<ExchangePosition>& right)->bool
	{ return left->conid() < right->conid(); });

	// remove duplicated
	auto iter = std::unique(std::begin(exchange_position_holder_), std::end(exchange_position_holder_),
		[](const std::unique_ptr<ExchangePosition>& left, const std::unique_ptr<ExchangePosition>& right)->bool
	{ return left->conid() == right->conid();});

	exchange_position_holder_.resize(std::distance(std::begin(exchange_position_holder_), iter));

	// clear and map
	id_map_.clear();
	std::for_each(std::begin(exchange_position_holder_), std::end(exchange_position_holder_), 
		[this](const std::unique_ptr<ExchangePosition>& entry)
	{
		id_map_.insert( std::make_pair(entry->conid(), entry.get()) );
	});
}

void position::LoadAppExchangePosition(PosInfo& pos_info, ExchangePositionManager& mgt, int longdate, int user_id)
{
	pos_info.LoadExchangePositionHelper(mgt, user_id, longdate);
}
