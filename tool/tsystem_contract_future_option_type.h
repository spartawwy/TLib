#ifndef TSYSTEM_CONTRACT_FUTURE_OPTION_TYPE_H_
#define TSYSTEM_CONTRACT_FUTURE_OPTION_TYPE_H_

#include "tsystem_contract_option_type.h"

namespace TSystem
{
namespace contract
{
	struct FutureOptionType: OptionType
	{
		FutureOptionType()
			: OptionType()
			, future_expiration(0)
		{}

		int future_expiration;
	};

	inline bool operator<(const FutureOptionType& left, const FutureOptionType& right)
	{ 
		if( left.expire_style != right.expire_style )
			return left.expire_style == OptionType::ExpireStyle::EUROPEAN;
		else if( left.future_expiration != right.future_expiration )
			return left.future_expiration < right.future_expiration;
		else if( left.expiration != right.expiration )
			return left.expiration < right.expiration;
		else if( std::abs(left.strike-right.strike) > std::numeric_limits<double>::epsilon() )
			return left.strike < right.strike;
		else if( left.call_put != right.call_put )
			return left.call_put == OptionType::CallPut::CALL;
		else 
			return left.multiplier < right.multiplier;
	}
}
}

#endif // TSYSTEM_CONTRACT_FUTURE_OPTION_TYPE_H_