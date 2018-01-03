#ifndef TSYSTEM_CONTRACT_STOCK_TYPE_H_
#define TSYSTEM_CONTRACT_STOCK_TYPE_H_

#include "tsystem_contract_base_type.h"

namespace TSystem
{
namespace contract
{
	struct SpotType: ContractType{};

	inline bool operator<(const SpotType& left, const SpotType& right)
	{ return false; }
}
}

#endif // TSYSTEM_CONTRACT_STOCK_TYPE_H_