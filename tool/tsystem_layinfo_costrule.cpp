#include "tsystem_layinfo_costrule.h"

#include <cstdint>

TSystem::layinfo::TradeCost::TradeCost()
	: stamp_duty_()
	, commission_()
	, other_()
{}

TSystem::layinfo::TradeCostRule::TradeCostRule()
	: buy_open_(), buy_close_(), buy_close_intraday_()
	, sell_open_(), sell_close_(), sell_close_intraday_()
	, buy_avg_cost_(), sell_avg_cost_(), avg_cost_()
	, name_(), n_decimal_(0)
{}

void TSystem::layinfo::TradeCostRule::GenerateDerivedFee()
{
	buy_avg_cost_ = buy_open_*0.5 + buy_close_*0.25 + buy_close_intraday_*0.25;
	sell_avg_cost_ = sell_open_*0.5 + sell_close_*0.25 + sell_close_intraday_*0.25;
	avg_cost_ = buy_avg_cost_*0.5 + sell_avg_cost_*0.5;
}

double TSystem::layinfo::TradeCost::TradeFee::round_all(int n_decimal, RoundType type) const
{
    double num = 1.0;
    double den = std::pow(10, n_decimal);

    double ret = stamp_duty + commission + other;
    int64_t i_ret = static_cast<int64_t>(ret / num * den);
    double f_ret = i_ret / den * num;

    if( ret > f_ret && type == RoundType::CEIL )
        f_ret += num / den;

    return f_ret;
}