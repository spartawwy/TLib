#include "tsystem_layinfo.h"

#include <algorithm>
#include <boost/math/common_factor.hpp>

#include "Tlib/core/tsystem_time.h"
#include "TLib/core/tsystem_message_system.h"
#include "TLib/core/tsystem_sqlite_functions.h"


using namespace TSystem;
using namespace TSystem::layinfo;

LayInfo::LayInfo()
	: db_conn_(std::make_shared<SQLite::SQLiteConnection>() )
	, exchanges_holder_()
	, market_rules_holder_(), tick_rules_holder_()
	, trade_hours_holder_()
	, underlyings_holder_()
{
	exchanges_holder_.reserve(16);
	market_rules_holder_.reserve(16);
	tick_rules_holder_.reserve(16);
	trade_hours_holder_.reserve(16);
	underlyings_holder_.reserve(16);
}

LayInfo::LayInfo(const std::string& tag)
	: db_conn_(std::make_shared<SQLite::SQLiteConnection>() )
	, exchanges_holder_()
	, market_rules_holder_(), tick_rules_holder_()
	, trade_hours_holder_()
	, underlyings_holder_()
{
	exchanges_holder_.reserve(16);
	market_rules_holder_.reserve(16);
	tick_rules_holder_.reserve(16);
	trade_hours_holder_.reserve(16);
	underlyings_holder_.reserve(16);

	Open(tag);
}

bool LayInfo::Open(const std::string& tag, TError* te)
{
	std::string full_file_name = utility::LayoutDir() + tag + ".laydb";
	try
	{
		SQLite::SQLiteCode rc = db_conn_->Open(full_file_name.c_str(), SQLite::SQLiteConnection::OpenMode::READ);
		if( rc != SQLite::SQLiteCode::OK )
		{
			if( te )
				*te = MakeTError(CoreErrorCategory::ErrorCode::BAD_ARGUMENT
					, "LayInfo::Open"
					, FormatThirdPartyError("SQLite", static_cast<int>(rc), SQLite::SQLiteCodeStr(rc)));
			return false;
		}

	}catch( const SQLite::SQLiteException& e)
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "LayInfo::Open"
			, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()));
	}

	return true;
}

void LayInfo::Open(const std::string& tag )
{
	TError te;
	if( !Open(tag, &te) )
		throw TException(std::move(te));
}

std::shared_ptr<Exchange> LayInfo::FindExchange(const std::string& exchange_name)
{
	auto iter = std::find_if(std::begin(exchanges_holder_), std::end(exchanges_holder_)
		, [&exchange_name](const std::shared_ptr<Exchange>& p)
	{ return p->name() == exchange_name; });

	if( iter != std::end( exchanges_holder_) )
		return *iter;

	std::shared_ptr<Exchange> p;
	std::string sql;
	int res_count = 0;
	// database operation
	try
	{
		if( !(utility::ExistTable("Exchange", *db_conn_) && 
				utility::ExistTable("ExchangeDate", *db_conn_)) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::FindExchange"
				, "can't find table Exchange or ExchangeDate");

		std::shared_ptr<Exchange> p_exchange = std::make_shared<Exchange>();

		// load Exchange table
		sql = utility::FormatStr( "SELECT time_zone, city FROM Exchange WHERE name = '%s'"
			, exchange_name.c_str());
		res_count = 0;

		db_conn_->ExecuteSQL( sql.c_str(), [&p_exchange, &res_count](int cols, char** vals, char** names)->int
		{
			++res_count;
			p_exchange->time_zone_ = *vals;
			p_exchange->city_ = *(vals+1);
			return 0;
		});

		if( res_count > 1 )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::FindExchange"
				, "duplicate exchange in in Exchange table: " + exchange_name);

		if( !res_count )
			return p;

		// load exchange date
		sql = utility::FormatStr( "SELECT date, is_tradeday FROM ExchangeDate WHERE exch = '%s' Order By date "
			, exchange_name.c_str() );
		res_count = 0;

		db_conn_->ExecuteSQL( sql.c_str(), [&p_exchange, &res_count](int cols, char** vals, char** names)->int
		{
			++res_count;
			int longdate = atoi(*vals);
			int is_tradeday = atoi(*(vals+1));
			p_exchange->exch_cal_dates_.push_back( longdate );
			if( is_tradeday )
				p_exchange->exch_bus_dates_.push_back( longdate );
			return 0;
		});

		if( !res_count )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::FindExchange"
				, "no exchange date for exchange: " + exchange_name);

		p_exchange->name_ = exchange_name;
		p = p_exchange;
		exchanges_holder_.push_back(std::move(p_exchange));

	}catch( const SQLite::SQLiteException& e )
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
			, "LayInfo::FindExchange"
			, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()) );
	}
	
	return p;
}

const Exchange& LayInfo::GetExchange(const std::string& exchange_name)
{
	std::shared_ptr<Exchange> p = FindExchange(exchange_name);
	
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::GetExchange"
				, "can't find exchange: " + exchange_name);

	return *p;
}

std::shared_ptr<TickRule> LayInfo::FindTickRule(const std::string& tick_rule_name)
{
	auto iter = std::find_if(std::begin(tick_rules_holder_), std::end(tick_rules_holder_)
		, [&tick_rule_name](const std::shared_ptr<TickRule>& p)
	{ return p->name() == tick_rule_name; });

	if( iter != std::end( tick_rules_holder_) )
		return *iter;

	std::shared_ptr<TickRule> p;
	std::string sql;
	int res_count = 0;
	// database operation
	try
	{
		if( !utility::ExistTable("TickSize", *db_conn_) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
					, "LayInfo::FindTickRule"
					, "can't find table TickSize");

		std::shared_ptr<TickRule> p_tickrule = std::make_shared<TickRule>();

		//---------------------
		// load TickSize table
		//---------------------

		// find divisor
		sql = utility::FormatStr("SELECT minprc_den, maxprc_den, ticsz_den FROM TickSize WHERE name = '%s' ORDER BY rule_id ASC"
			, tick_rule_name.c_str());
		res_count = 0;

		db_conn_->ExecuteSQL(sql.c_str(), [&p_tickrule, &res_count](int cols, char** vals, char** names)->int
		{
			++res_count;
			std::for_each( vals, vals+cols, [&p_tickrule](char* str)
			{
				p_tickrule->divisor_ = boost::math::lcm<int>(p_tickrule->divisor_, atoi(str));
			});
			return 0;
		});

		if( !res_count )
			return p;

		sql = utility::FormatStr("SELECT minprc_num, minprc_den, maxprc_num, maxprc_den, ticsz_num, ticsz_den FROM TickSize "
			"WHERE name = '%s' ORDER BY rule_id ASC", tick_rule_name.c_str());
		
		db_conn_->ExecuteSQL(sql.c_str(), [&p_tickrule, &res_count](int cols, char** vals, char** names)->int
		{
			TickRule::TickRange range;
			range.min_prc = atoi(*vals) * (p_tickrule->divisor_ / atoi(*(vals+1)));
			range.max_prc = atoi(*(vals+2)) * (p_tickrule->divisor_ / atoi(*(vals+3)));
			range.tic_sze = atoi(*(vals+4)) * (p_tickrule->divisor_ / atoi(*(vals+5)));
			range.tic_num = static_cast<int>((range.max_prc - range.min_prc)/range.tic_sze);
			p_tickrule->tic_ranges_.push_back(range);
			return 0;
		});

		// check tick rule
		switch (p_tickrule->CheckRanges())
		{
		case 1:
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::FindTickRule"
				, "max_prc != min_prc + tic_sze * tic_num: " + tick_rule_name);
			break;
		case 2:
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::FindTickRule"
				, "tick ranges are not continous: " + tick_rule_name);
			break;
		default:
			break;
		} 

		p_tickrule->name_ = tick_rule_name;
		p = p_tickrule;
		tick_rules_holder_.push_back(std::move(p_tickrule));

	}catch(const SQLite::SQLiteException& e )
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
			, "LayInfo::FindTickRule"
			, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()) );
	}

	return p;
}


const TickRule& LayInfo::GetTickRule(const std::string& tick_rule_name)
{
	std::shared_ptr<TickRule> p = FindTickRule(tick_rule_name);
	
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::GetTickRule"
				, "can't find tick rule: " + tick_rule_name);

	return *p;
}

std::shared_ptr<TradeCostRule> LayInfo::FindTradeCostRule(const std::string& cost_rule_name)
{
	auto iter = std::find_if(std::begin(cost_rules_holder_), std::end(cost_rules_holder_)
		, [&cost_rule_name](const std::shared_ptr<TradeCostRule>& p)
	{ return p->name() == cost_rule_name; });

	if( iter != std::end(cost_rules_holder_) )
		return *iter;

	std::shared_ptr<TradeCostRule> p;
	std::string sql;
	int res_count = 0;
	// database operation
	try
	{
		if( !utility::ExistTable("TradeCost", *db_conn_) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
					, "LayInfo::FindTradeCostRule"
					, "can't find table TradeCost");

		std::shared_ptr<TradeCostRule> p_cost_rule = std::make_shared<TradeCostRule>();
		

		//---------------------
		// load TradeCost table
		//---------------------

		// find and initiate trade cost rule
		sql = utility::FormatStr("SELECT buy_sell, open_close, category, min_cost, per_capital, per_contract, per_trade, n_decimal "
									"FROM TradeCost "
									"WHERE name = '%s' "
									"ORDER BY name, id"
									, cost_rule_name.c_str());

		db_conn_->ExecuteSQL(sql.c_str(), [&p_cost_rule, &res_count](int cols, char** vals, char** names)->int
		{
			++res_count;
			TradeCost* p_cost = nullptr;
			
			std::string buy_sell		= *vals;
			std::string open_close		= *(vals+1);
			if( buy_sell == "B" )
			{
				if( open_close == "O") p_cost = &p_cost_rule->buy_open_;
				else if( open_close == "C") p_cost = &p_cost_rule->buy_close_;
				else if( open_close == "CI") p_cost = &p_cost_rule->buy_close_intraday_;
			}else if( buy_sell == "S" )
			{
				if( open_close == "O") p_cost = &p_cost_rule->sell_open_;
				else if( open_close == "C") p_cost = &p_cost_rule->sell_close_;
				else if( open_close == "CI") p_cost = &p_cost_rule->sell_close_intraday_;
			}

			if( !p_cost )
			{
				ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
								"LayInfo::FindTradeCostRule",
                                utility::FormatStr("Can't find buy_sell type %s or open_close type %s", buy_sell.c_str(), open_close.c_str()));
			}

			TradeCost::FeeStruct* p_fee = nullptr;

			std::string category		= *(vals+2);
			if( category == "stamp_duty" ) p_fee = &p_cost->stamp_duty_;
			else if( category == "commission" ) p_fee = &p_cost->commission_;
			else if( category == "other" ) p_fee = &p_cost->other_;

			if( !p_fee )
			{
				ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
								"LayInfo::FindTradeCostRule",
                                utility::FormatStr("Can't find category: %s", category.c_str()));
			}

			p_fee->min_charge   = atof(*(vals+3));
			p_fee->per_capital  = atof(*(vals+4));
			p_fee->per_contract = atof(*(vals+5));
			p_fee->per_trade    = atof(*(vals+6));

            p_cost_rule->n_decimal_ = atoi(*(vals+7));

			return 0;
		});

		if( !res_count )
			return p;

		p_cost_rule->GenerateDerivedFee();
		p_cost_rule->name_ = cost_rule_name;
		p = p_cost_rule;
		cost_rules_holder_.push_back( std::move(p_cost_rule) );

	}catch( const SQLite::SQLiteException& e )
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
			, "LayInfo::FindCostRule"
			, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()) );
	}

	return p;
}

const TradeCostRule& LayInfo::GetTradeCostRule(const std::string& cost_rule_name)
{
	std::shared_ptr<TradeCostRule> p = FindTradeCostRule(cost_rule_name);

	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::GetTradeCostRule"
				, "can't find trade cost rule: " + cost_rule_name);

	return *p;
}


std::shared_ptr<MarketRule> LayInfo::FindMarketRule(const std::string& rule_name)
{
	auto iter = std::find_if(std::begin(market_rules_holder_), std::end(market_rules_holder_)
		, [&rule_name](const std::shared_ptr<MarketRule>& p)
	{ return p->name() == rule_name; });

	if( iter != std::end( market_rules_holder_) )
		return *iter;

	std::shared_ptr<MarketRule> p;
	std::string sql;
	int res_count = 0;
	// database operation
	try
	{
		if( !utility::ExistTable("MarketRule", *db_conn_) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
					, "LayInfo::FindMarketRule"
					, "can't find table MarketRule");

		std::shared_ptr<MarketRule> p_marketrule = std::make_shared<MarketRule>();

		// select MarketRule table
		sql = utility::FormatStr("SELECT lot_sz, tick_sz, trade_cost FROM MarketRule WHERE name = '%s'"
			, rule_name.c_str());
		res_count = 0;
		std::string tick_rule;
		std::string cost_rule;

		db_conn_->ExecuteSQL(sql.c_str(), [&p_marketrule, &tick_rule, &cost_rule, &res_count](int cols, char** vals, char** names)->int
		{
			++res_count;
			p_marketrule->lot_size_ = atoi(*vals);
			tick_rule = *(vals+1);
			cost_rule = *(vals+2);
			return 0;
		});

		if( res_count > 1 )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::FindMarketRule"
				, "duplicate market rule in MarketRule table: " + rule_name);

		if( !res_count )
			return p;

		p_marketrule->tick_rule_ = FindTickRule(tick_rule);

		if( !p_marketrule->tick_rule_ )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::FindMarketRule"
				, utility::FormatStr("can't find tick rule for market rule: %s, %s"
				, tick_rule.c_str(), rule_name.c_str()));
		
		p_marketrule->cost_rule_ = FindTradeCostRule(cost_rule);
		if( !p_marketrule->cost_rule_ )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::FindMarketRule"
				, utility::FormatStr("can't find trade cost rule for market rule: %s, %s"
				, cost_rule.c_str(), rule_name.c_str()));


		p_marketrule->name_ = rule_name;
		p = p_marketrule;
		market_rules_holder_.push_back(std::move(p_marketrule));
	
	}catch(const SQLite::SQLiteException& e)
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT,
								"LayInfo::FindMarketRule",
								FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what())
								);	
	}

	return p;
}

const MarketRule& LayInfo::GetMarketRule(const std::string& rule_name)
{
	std::shared_ptr<MarketRule> p = FindMarketRule(rule_name);
	
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::GetMarketRule"
				, "can't find market rule: " + rule_name);

	return *p;
}

std::shared_ptr<TradeHour> LayInfo::FindTradeHour(const std::string& hour_name, const std::string& exchange_name )
{
	auto iter = std::find_if(std::begin(trade_hours_holder_), std::end(trade_hours_holder_)
		, [&hour_name, &exchange_name](const std::shared_ptr<TradeHour>& p)
	{ return p->name() == hour_name && p->exchange().name() == exchange_name; });

	if( iter != std::end(trade_hours_holder_) )
		return *iter;

	std::shared_ptr<TradeHour> p;
	std::string sql;
	int res_count = 0;
	// database operation
	try
	{
		if( !utility::ExistTable("TradingHour", *db_conn_) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
					, "LayInfo::FindTradeHour"
					, "can't find table TradingHour");

		std::shared_ptr<TradeHour> p_tradehour = std::make_shared<TradeHour>( FindExchange(exchange_name) );

		// read all sessions
		sql = utility::FormatStr("SELECT opn_type, opn_d, opn_h, opn_m, opn_s, cls_type, cls_d, cls_h, cls_m, cls_s FROM TradingHour "
			"WHERE name = '%s' ORDER BY ses_id ASC", hour_name.c_str());
		res_count = 0;
		db_conn_->ExecuteSQL(sql.c_str(), [&p_tradehour, &res_count](int cols, char** vals, char** names)->int
		{
			++res_count;
			int cal_offset = 0;

			TradeSession session;
			session.open_type	= StringToOpenCloseType( *vals );
			session.open_time	= MakeIntradayTimePoint( atoi(*(vals+1)), atoi(*(vals+2)), atoi(*(vals+3)), atoi(*(vals+4)) );
			

			session.close_type = StringToOpenCloseType( *(vals+5) );
			session.close_time = MakeIntradayTimePoint( atoi(*(vals+6)), atoi(*(vals+7)), atoi(*(vals+8)),  atoi(*(vals+9)) );
			
			p_tradehour->trade_sessions_.push_back(std::move(session));
			return 0;
		});

		if( !res_count )
			return p;

		//-----------------------------
		// find and set trading date
		// trading day is defined as close-to-close
		// that means after close, the trading date is the second day
		//-----------------------------
		TimePoint now(GetClock().Now());
		int longdate = ToLongdate(now.year(), now.month(), now.day());
		std::chrono::system_clock::duration time_of_day = std::chrono::hours(now.hour()) 
															+ std::chrono::minutes(now.min()) 
															+ std::chrono::seconds(now.sec());

		int trading_date = 0;
		std::chrono::system_clock::duration open_time = p_tradehour->trade_sessions_.front().open_time.time_of_day;
		std::chrono::system_clock::duration close_time = p_tradehour->trade_sessions_.back().close_time.time_of_day;

		if( open_time < close_time ) // 00:00:00-----------open---------close--------------23:59:59
		{
			trading_date = (time_of_day < close_time) ? longdate : p_tradehour->exchange_->NextBusDay(longdate, 1);
		}else  // 00:00:00---close-----open-------23:59:59
		{
			int open_offset  = p_tradehour->trade_sessions_.front().open_time.day_offset;
			int close_offset = p_tradehour->trade_sessions_.back().close_time.day_offset;
			if( time_of_day >= open_time )
			{
				trading_date = p_tradehour->exchange_->NextBusDay(longdate, -1*open_offset);
			}else if( time_of_day >= close_time && time_of_day < open_time )
			{
				trading_date = p_tradehour->exchange_->NextBusDay(longdate, 1-close_offset);	
			}else
			{
				trading_date = p_tradehour->exchange_->NextBusDay(longdate, -1*close_offset);
			}
		}

		if( !p_tradehour->exchange_->IsBusDay(trading_date) ) // round to next busday if not a busday
			trading_date = p_tradehour->exchange_->NextBusDay(longdate, 0);
		
		p_tradehour->InitiateSessions(trading_date);
		p_tradehour->name_ = hour_name;
		p = p_tradehour;
		trade_hours_holder_.push_back(std::move(p_tradehour));

	}catch(const SQLite::SQLiteException& e)
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT,
								"LayInfo::FindTradeHour",
								FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what())
								);	
	}

	return p;
}

const TradeHour& LayInfo::GetTradeHour(const std::string& hour_name, const std::string& exchange)
{
	std::shared_ptr<TradeHour> p = FindTradeHour(hour_name, exchange);
	
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::GetTradeHour"
				, "can't find trade hour: " + hour_name);

	return *p;
}

std::shared_ptr<Underlying> LayInfo::FinderUnderlying(const std::string& under_name)
{
	auto iter = std::find_if(std::begin(underlyings_holder_), std::end(underlyings_holder_)
		, [&under_name](const std::shared_ptr<Underlying>& p)
	{ return p->name == under_name; });

	if( iter != std::end(underlyings_holder_) )
		return *iter;

	std::shared_ptr<Underlying> p;
	std::string sql;
	int res_count = 0;
	// database operation
	try
	{
		if( !utility::ExistTable("Underlying", *db_conn_) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
					, "LayInfo::FinderUnderlying"
					, "can't find table Underlying");

		std::shared_ptr<Underlying> p_under = std::make_shared<Underlying>();

		sql = utility::FormatStr("SELECT type, is_index, exch FROM Underlying "
				"WHERE name = '%s'", under_name.c_str());
		res_count = 0;
		std::string exchange;

		db_conn_->ExecuteSQL(sql.c_str(), [&p_under, &res_count, &exchange](int cols, char** vals, char** names)->int
		{
			++res_count;
			p_under->type     = StringToUnderlyingType(*vals);
			p_under->is_index = atoi(*(vals+1)) != 0;
			exchange		  = *(vals+2);

			return 0;
		});

		if( !res_count )
			return p;

		p_under->exchange_ = FindExchange(exchange);

		if( !p_under->exchange_ )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::FinderUnderlying"
				, utility::FormatStr("can't find exchange for underlying: %s, %s"
				, exchange.c_str(), under_name.c_str()));

		p_under->name = under_name;
		p = p_under;
		underlyings_holder_.push_back(std::move(p_under));

	}catch(const SQLite::SQLiteException& e)
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT,
								"LayInfo::FinderUnderlying",
								FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what())
								);	
	}

	return p;
}

const Underlying& LayInfo::GetUnderlying(const std::string& under_name)
{
	std::shared_ptr<Underlying> p = FinderUnderlying(under_name);
	
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::GetUnderlying"
				, "can't find underlying: " + under_name);

	return *p;
}

UnderlyingManager LayInfo::LoadUnderlying(UnderlyingPredType&& pred)
{
	UnderlyingManager mgt;
	std::string sql = "Select name, type, is_index, exch from Underlying";
	try
	{
		if( !utility::ExistTable("Underlying", *db_conn_) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
					, "LayInfo::LoadUnderlying"
					, "can't find table Underlying");

		db_conn_->ExecuteSQL(sql.c_str(), [&mgt, &pred, this](int cols, char** vals, char** names)->int
		{
			std::shared_ptr<Underlying> a = std::make_shared<Underlying>();
			a->name			= *vals;
			a->type			= StringToUnderlyingType(*(vals+1));
			a->is_index		= atoi(*(vals+2)) != 0;
			
			std::string exch = *(vals+3);
			a->exchange_    = FindExchange(exch);
			if( !a->exchange_ )
				ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
					, "LayInfo::FinderUnderlying"
					, utility::FormatStr("can't find exchange for underlying: %s, %s"
					, exch.c_str(), a->name.c_str()));

			if( (pred && pred(*a)) || (!pred) )	
				mgt.underlying_holder_.push_back(std::move(a));
			return 0;
		});
	}catch( const SQLite::SQLiteException& e )
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT,
							"LayInfo::LoadUnderlying",
							FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what())
							);
	}

	// map manager
	mgt.name_maps_.clear();
	std::for_each(std::begin(mgt.underlying_holder_), std::end(mgt.underlying_holder_)
		, [&mgt](std::shared_ptr<Underlying>& underlying)
	{		
		if( !mgt.name_maps_.insert(std::make_pair(underlying->name, underlying.get())).second )
		{
			ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT, 
								"LayInfo::LoadUnderlying",
								"duplicaited name: " + underlying->name
								);
		}
	});

	return mgt;
}

MessageSystem LayInfo::LoadClassID() const
{
	MessageSystem res;
	try
	{
		if( !utility::ExistTable("ClassID", *db_conn_) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
						, "LayInfo::LoadClassID"
						, "can't find table ClassID");

		std::string sql = "Select name, class_id from ClassID";
	
	
		db_conn_->ExecuteSQL( sql.c_str(), [&res](int cols, char** vals, char** names)->int
		{
			std::string name = *vals;
			int         class_id = atoi(*(vals+1));
			res.RegisterClassID( name, class_id);
			return 0;
		});
	}catch( const SQLite::SQLiteException& e )
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
			, "LayInfo::LoadClassID"
			, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()) );
	}
	
	return res;
}

layinfo::AppResourceManager LayInfo::LoadAppResourceManager() const
{
	layinfo::AppResourceManager res;

	try
	{
		if( !(utility::ExistTable("Resource", *db_conn_) && utility::ExistTable("ResourceRaid", *db_conn_)) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
						, "LayInfo::LoadAppResourceManager"
						, "can't find table Resource or ResourceRaid ");

		std::string sql = "SELECT name, type, dev_tag, node, domain, host, port, Resource.pid, raid_id "
			"FROM Resource LEFT JOIN ResourceRaid ON Resource.pid = ResourceRaid.pid";
	
		db_conn_->ExecuteSQL(sql.c_str(), [&res](int cols, char** vals, char** names)->int
		{
			AppResource a;
			a.name    = *vals;
			a.type    = *(vals+1);
			a.dev_tag = *(vals+2);
			a.node	  = *(vals+3);
			a.domain  = *(vals+4);
			a.host    = *(vals+5);
			a.port    = static_cast<unsigned short>(atoi(*(vals+6)));
			a.pid     = static_cast<unsigned short>(atoi(*(vals+7)));
			if( *(vals+8) )
				a.raid_id = static_cast<unsigned short>(atoi(*(vals+8)));
			res.RegisterResource(a);
			return 0;
		});
	}catch( const SQLite::SQLiteException& e )
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT,
							"LayInfo::LoadAppResourceManager",
							FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what())
							);
	}

	res.MapResource();
	
	return res;
}