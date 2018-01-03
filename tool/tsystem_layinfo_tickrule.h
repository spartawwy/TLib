#ifndef TSYSTEM_LAYINFO_TICK_RULE_H_
#define TSYSTEM_LAYINFO_TICK_RULE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "TLib/tool/tsystem_rational_number.pb.h"

namespace TSystem
{
namespace layinfo
{
	// struct for rational price, no operator is defined
	// only used for representing a precise number
	struct RationalPrice
	{
		RationalPrice()
		{ Clear(); }

		RationalPrice(int64_t v, int divisor)
		{
			r = static_cast<int>(v/divisor);
			n = v%divisor;
			d = divisor;
		}

		explicit RationalPrice(const RationalNumber& num)
		{
			r = num.r();
			n = num.n();
			d = num.d();
		};

		double ToDouble() const 
		{
			return r+static_cast<double>(n)/d;
		}

		void Clear() 
		{ r = n = 0; d = 1; }

		int r;
		int n;
		int d;
	};

	class LayInfo;

	//---------------
	// tick rule
	//---------------
	class TickRule
	{
	public:

		enum class RoundType: char
		{
			CEIL  = 0,
			FLOOR = 1
		};

		TickRule();

		operator bool() const { return !name_.empty(); } 

		const std::string& name() const { return name_; }

		// round price to tick price, and plus num_tick adjustment(signed)
		// this function is supposed to be called only when filling the order price
		// quite expensive for high frequent calling
		RationalPrice TickPrice(double price, int tick_ofset, RoundType round_type ) const;

		// check if RationalPrice fit the tick rule
		bool CheckPrice(const RationalPrice& price) const;

	private:
		
		struct TickRange
		{
			TickRange()
			{ 
				min_prc = max_prc = 0;
				tic_sze = tic_num = 0; 
			}

			int64_t min_prc;
			int64_t max_prc;
			int tic_sze;
			int tic_num;
		};

		// 0: ok; 1: max_prc != min_prc + tic_sze * tic_num; 2: range not continoues
		int CheckRanges() const;

		std::string             name_;
		int                     divisor_;
		std::vector<TickRange>	tic_ranges_;
		
		friend class LayInfo;
	};

	inline void FillRational(const layinfo::RationalPrice& prc, RationalNumber& num)
	{
		assert(prc.d>0);
		num.set_d(prc.d);
		num.set_r(prc.r);
		num.set_n(prc.n);
	}
}
}

#endif // TSYSTEM_LAYINFO_TICK_RULE_H_