#ifndef TSYSTEM_LAYINFO_H_
#define TSYSTEM_LAYINFO_H_

#include <memory>
#include <string>
#include <vector>

#include <SQLite/SQLite.h>

#include "TLib/core/tsystem_error.h"
#include "TLib/core/tsystem_core_paths.h"
#include "TLib/tool/tsystem_layinfo_exchange.h"
#include "TLib/tool/tsystem_layinfo_marketrule.h"
#include "TLib/tool/tsystem_layinfo_resource.h"
#include "TLib/tool/tsystem_layinfo_tradehour.h"
#include "TLib/tool/tsystem_layinfo_underlying.h"


namespace TSystem
{
	class MessageSystem;

namespace utility
{
	inline std::string LayoutDir()
	{
		return DataDir() + "layout//";
	}
}

namespace layinfo
{
	// server as a common place holder and generator for all layinfo object
	// nearly all operation may throw TException if there is database IO error
	// the comments only means TException not caused by database IO operation 
	// add try-catch block to handle possible database IO error is necessary 
	class LayInfo
	{
	public:
		
		// underlying predictor
		typedef std::function<bool(const Underlying&)>	UnderlyingPredType;

		LayInfo();
		explicit LayInfo(const std::string& tag);

		bool Open(const std::string& tag, TError* te);
		void Open(const std::string& tag); // version to throw TException

		// return shared_ptr because in case user uses Getxxxx interface, copy the object
		// and destroy the LayInfo obejct

		// find/get layinfo object
		std::shared_ptr<Exchange> FindExchange(const std::string& exchange_name); // throw TException when fail to load from db
		const Exchange& GetExchange(const std::string& exchange_name); // throw TException when not found

		// find/get market rule object
		std::shared_ptr<MarketRule> FindMarketRule(const std::string& rule_name); // throw TException when fail to load from db
		const MarketRule& GetMarketRule(const std::string& rule_name); // throw TException when not found

		// return shared_ptr<TickRule> because MarketRule object holds a shared_ptr to TickRule.
		// this is because in case user uses GetMarketRule, copy the object and destroy the LayInfo obejct
		// the copied MarketRule object should still hold a valid TickRule object pointer
		std::shared_ptr<TickRule> FindTickRule(const std::string& tick_rule_name); // throw TException when fail to load from db
		const TickRule& GetTickRule(const std::string& tick_rule_name); // throw TException when not found

		// find/get trade cost rule
		std::shared_ptr<TradeCostRule> FindTradeCostRule(const std::string& cost_rule_name); // throw TException when fail to load from db
		const TradeCostRule& GetTradeCostRule(const std::string& cost_rule_name); // throw TException when not found

		// find/get trade hour object
		std::shared_ptr<TradeHour> FindTradeHour(const std::string& hour_name, const std::string& exchange); // throw TException when fail to load from db
		const TradeHour& GetTradeHour(const std::string& hour_name, const std::string& exchange); // throw TException when not found

		// find/get/load underlying object
		std::shared_ptr<Underlying> FinderUnderlying(const std::string& under_name); // throw TException when fail to load from db
		const Underlying& GetUnderlying(const std::string& under_name); // throw TException when not found
		UnderlyingManager LoadUnderlying(UnderlyingPredType&& pred = UnderlyingPredType() ); // throw TException when fail to load from db

		// load message system
		// throw TException when fail to load from db
		MessageSystem LoadClassID() const;

		// load resource manager
		// throw TException when fail to load from db
		AppResourceManager LoadAppResourceManager() const;

	private:
	
		// the database connection
		std::shared_ptr<SQLite::SQLiteConnection>	db_conn_;

		// exchange objects holder
		std::vector<std::shared_ptr<Exchange>>	exchanges_holder_;

		// market rule object holder
		std::vector<std::shared_ptr<MarketRule>>	market_rules_holder_;
		std::vector<std::shared_ptr<TickRule>>		tick_rules_holder_; 
		std::vector<std::shared_ptr<TradeCostRule>> cost_rules_holder_;

		// trade hour object holder
		std::vector<std::shared_ptr<TradeHour>> trade_hours_holder_;

		// underlying object holder
		std::vector<std::shared_ptr<Underlying>> underlyings_holder_;
	};
}
}

#endif // TSYSTEM_LAYINFO_H_