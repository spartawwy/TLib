#ifndef TSYSTEM_CONTRACT_FUTURE_TYPE_H_
#define TSYSTEM_CONTRACT_FUTURE_TYPE_H_

#include "tsystem_contract_base_type.h"

namespace TSystem
{
namespace contract
{
	struct FutureType: ContractType {};

	inline bool operator<(const FutureType& left, const FutureType& right)
	{ 
		if( left.expiration != right.expiration )
			return left.expiration < right.expiration;
		else
			return left.multiplier < right.multiplier;
	}
}
}

#endif // TSYSTEM_CONTRACT_FUTURE_TYPE_H_