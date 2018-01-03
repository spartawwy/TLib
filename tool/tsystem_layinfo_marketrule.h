#ifndef TSYSTEM_LAYINFO_MARKET_RULE_H_
#define TSYSTEM_LAYINFO_MARKET_RULE_H_

#include "tsystem_layinfo_tickrule.h"
#include "tsystem_layinfo_costrule.h"

namespace TSystem
{
namespace layinfo
{

	class LayInfo;

	class MarketRule
	{
	public:

		MarketRule(): name_(), lot_size_(0), tick_rule_(), cost_rule_() {}

		operator bool() const { return !name_.empty(); }

		const std::string& name() const { return name_; }
		unsigned int    lot_size() const { return lot_size_;   }
		const TickRule& tick_rule() const { return *tick_rule_; }
		const TradeCostRule& trade_cost_rule() const { return *cost_rule_; }

	private:

		std::string						name_;
		unsigned int					lot_size_;
		std::shared_ptr<TickRule>		tick_rule_;
		std::shared_ptr<TradeCostRule>	cost_rule_;

		friend class LayInfo;
	};
}
}

#endif //TSYSTEM_LAYINFO_MARKET_RULE_H_