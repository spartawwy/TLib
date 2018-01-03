#ifndef TSYSTEM_MM_VALIDATION_H_
#define TSYSTEM_MM_VALIDATION_H_

#include <set>

#include "TLib/tool/tsystem_contract_list.h"
#include "TLib/tool/tsystem_contract_product.h"

namespace TSystem
{
namespace contract
{
	//------------------------------------------------------------
	// validate functions
	// check if contracts in ContractManager has the same property 
	// throw TException if not the same
	//-----------------------------------------------------------

	// check if has the same underlying, throw TException if not match
	std::string ValidateUnderlying(const contract::ContractManager& mgt);

	// check if has the same currency, throw TException if not match
	std::string ValidateCurrency(const contract::ContractManager& mgt); 

	// check if has the same ticker, throw TException if not match
	std::string ValidateTicker(const contract::ContractManager& mgt);

	// check if has the same delivery, throw TException if not match
	TSystem::contract::ProductInfo::DeliveryType ValidateDelivery(const contract::ContractManager& mgt);

	// check if contract types matches input, throw TException if not match
	std::set<TSystem::contract::ProductInfo::ContractType> ValidateContractTypes(const contract::ContractManager& mgt
		, const std::set<TSystem::contract::ProductInfo::ContractType>& target_types);

	// check if ecp matches input, because ecp group is based on exchange trading account, 
	// different strategies may trade the same contract in different ecps. Therefore, we have to 
	// input the target ecp name, throw TException if not match
	std::string ValidateEcp(const contract::ContractManager& mgt
		, const std::string& ecp_name, const contract::ContractList& contract_list);
}
}

#endif // TSYSTEM_MM_VALIDATION_H_