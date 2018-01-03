#ifndef TSYSTEM_LAYINFO_COST_RULE_H_
#define TSYSTEM_LAYINFO_COST_RULE_H_

#include <string>
#include <algorithm>

#include "TLib/core/tsystem_core_common.h"


namespace TSystem
{
namespace layinfo
{
	class LayInfo;

	class TradeCost
	{
	public:

		struct TradeFee
		{
            enum class RoundType: char
            {
                CEIL  = 0,
                FLOOR = 1
            };

			TradeFee()
			{ stamp_duty = commission = other = 0;}

			TradeFee(double s, double c, double o)
				: stamp_duty(s), commission(c), other(o)
			{}

			double all() const { return stamp_duty + commission + other; }

            double round_all(int n_decimal, RoundType type) const;

			double stamp_duty;
			double commission;
			double other;
		};

		TradeCost();

		TradeCost operator*(double a) const
		{
			TradeCost res;

			res.stamp_duty_ = stamp_duty_*a;	
			res.commission_ = commission_*a;
			res.other_ = other_*a;
			return res;
		}

		TradeCost operator/(double a) const
		{
			return *this * (1/a);
		}

		TradeFee CalculateTradeFee(double size, double dollar) const
		{
			return size <= 0 ? TradeFee() : TradeFee(stamp_duty_.CalcaulteFee( size, dollar ) 
					, commission_.CalcaulteFee( size, dollar )
					, other_.CalcaulteFee(size, dollar ));
		}

	private:
		//------------------
		// struct FeeStruct
		//------------------
		struct FeeStruct
		{
			FeeStruct()
			{
				per_trade = per_contract = per_capital = min_charge = 0;	
			}

			FeeStruct operator*(double a) const
			{
				FeeStruct res;
				res.per_trade = per_trade*a;
				res.per_contract = per_contract*a;
				res.per_capital = per_capital*a;
				res.min_charge = min_charge*a;
				return res;
			}

			FeeStruct operator/(double a) const
			{
				return *this * (1/a);
			}

			double CalcaulteFee(double size, double dollar) const
			{ return std::max(per_trade + per_contract * size + per_capital * dollar, min_charge); }

			double	per_trade;
			double  per_contract;
			double  per_capital;
			double  min_charge;
		};

		FeeStruct	stamp_duty_;
		FeeStruct   commission_;
		FeeStruct   other_;

		friend class LayInfo;
		friend FeeStruct operator+(const FeeStruct& left, const FeeStruct& right);
		friend TradeCost operator+(const TradeCost& left, const TradeCost& right);
	};

	inline TradeCost::FeeStruct operator+(const TradeCost::FeeStruct& left, const TradeCost::FeeStruct& right)
	{
		TradeCost::FeeStruct res;
		res.per_trade = left.per_trade + right.per_trade;
		res.per_contract = left.per_contract + right.per_contract;
		res.per_capital = left.per_capital + right.per_capital;
		res.min_charge = left.min_charge + right.min_charge;
		return res;
	}

	inline TradeCost operator+(const TradeCost& left, const TradeCost& right)
	{
		TradeCost res;
		res.stamp_duty_ = left.stamp_duty_ + right.stamp_duty_;
		res.commission_ = left.commission_ + right.commission_;
		res.other_ = left.other_ + right.other_;
		return res;
	}

	//---------------
	// TradeCostRule
	//---------------
	class TradeCostRule
	{
	public:
		
		TradeCostRule();

		const std::string& name() const { return name_; }

        int n_decimal() const { return n_decimal_; }

		// calculate trade cost in terms of
		// fill price, fill siz, buy sell direction, open close/close_intraday
		TradeCost::TradeFee CalculateTradeCost( double size, double dollar ) const
		{
			return avg_cost_.CalculateTradeFee(size, dollar);
		}

		TradeCost::TradeFee CalculateTradeCost( double size, double dollar, bool buy ) const
		{
			return buy ? buy_avg_cost_.CalculateTradeFee(size, dollar) : sell_avg_cost_.CalculateTradeFee(size, dollar);
		}

		// throw TException if wrong open close type
		TradeCost::TradeFee CalculateTradeCost( double size, double dollar, bool buy, int oc ) const
		{
			if( buy )
				switch (oc)
				{
					case 0: 
						return buy_open_.CalculateTradeFee(size, dollar);
					case 1: 
						return buy_close_.CalculateTradeFee(size, dollar);
					case 2: 
						return buy_close_intraday_.CalculateTradeFee(size, dollar);
					default:
						ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
										"TSystem::layinfo::TradeCostRule::CalculateTradeCost",
										utility::FormatStr( "wrong open close type: %d", oc ) ); 
				}
			else
				switch (oc)
				{
					case 0: 
						return sell_open_.CalculateTradeFee(size, dollar);
					case 1: 
						return sell_close_.CalculateTradeFee(size, dollar);
					case 2: 
						return sell_close_intraday_.CalculateTradeFee(size, dollar);
					default:
						ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
										"TSystem::layinfo::TradeCostRule::CalculateTradeCost",
										utility::FormatStr( "wrong open close type: %d", oc ) ); 
				}
		}

	private:

		// derived fee struct from basic structs
		void GenerateDerivedFee();

		TradeCost	buy_open_;
		TradeCost   buy_close_;
		TradeCost	buy_close_intraday_;

		TradeCost   sell_open_;
		TradeCost   sell_close_;
		TradeCost   sell_close_intraday_;
		
		// derived fee struct
		TradeCost   buy_avg_cost_;
		TradeCost	sell_avg_cost_;
		TradeCost   avg_cost_;

		std::string			name_;

        int                 n_decimal_;

		friend class		LayInfo;
	};

}
}






#endif //TSYSTEM_LAYINFO_COST_RULE_H_