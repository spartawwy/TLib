#ifndef TSYSTEM_CONTRACT_OPTION_TYPE_H_
#define TSYSTEM_CONTRACT_OPTION_TYPE_H_

#include <string>
#include <numeric>

#include "tsystem_contract_base_type.h"

namespace TSystem
{
namespace contract
{
	struct OptionType: ContractType
	{
		enum class ExpireStyle: short
		{
			EUROPEAN  = 0,
			AMERICAN   
		};

		enum class CallPut: short
		{
			CALL   = 0,
			PUT    
		};

		OptionType()
			: ContractType()
			, strike(0), expire_style(ExpireStyle::EUROPEAN), call_put(CallPut::CALL)
		{}

		double	    strike;
		ExpireStyle	expire_style;
		CallPut		call_put;
	};

	inline bool operator<(const OptionType& left, const OptionType& right)
	{ 
		if( left.expire_style != right.expire_style )
			return left.expire_style == OptionType::ExpireStyle::EUROPEAN;
		else if( left.expiration != right.expiration )
			return left.expiration < right.expiration;
		else if( std::abs(left.strike-right.strike) > std::numeric_limits<double>::epsilon() )
			return left.strike < right.strike;
		else if( left.call_put != right.call_put )
			return left.call_put == OptionType::CallPut::CALL;
		else
			return left.multiplier < right.multiplier;
	}

	const std::string& ExpireStyleToString( OptionType::ExpireStyle t); // throw TException if unkown expiry style 
	OptionType::ExpireStyle StringToExpireStyle( const std::string& s); // throw TException if unkown string of expiry

	const std::string& CallPutToString( OptionType::CallPut t ); // throw TEception if unkown call put type
	OptionType::CallPut StringToCallPut( const std::string& s ); // throw TEception if unkown string of call put type
}
}

#endif // TSYSTEM_CONTRACT_OPTION_TYPE_H_