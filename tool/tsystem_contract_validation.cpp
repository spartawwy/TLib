#include "tsystem_contract_validation.h"

#include <algorithm>

using namespace TSystem;

std::string TSystem::contract::ValidateUnderlying(const contract::ContractManager& mgt)
{
	std::string underlying;
	std::for_each( std::begin(mgt.AllContracts()), std::end(mgt.AllContracts())
		,[&underlying](const std::unique_ptr<contract::Contract>& p_con )
	{
		if( underlying.empty() )
			underlying = p_con->product_info().under;

		if( p_con->product_info().under != underlying )
			ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "ValidateUnderlying"
				, utility::FormatStr("contract has different underlying: conid: %d %s %s"
				, p_con->contract_id, p_con->product_info().under.c_str(), underlying.c_str()));
	});

	return underlying;
}

std::string TSystem::contract::ValidateCurrency(const contract::ContractManager& mgt)
{
	std::string currency;
	std::for_each( std::begin(mgt.AllContracts()), std::end(mgt.AllContracts())
		,[&currency](const std::unique_ptr<contract::Contract>& p_con )
	{
		if( currency.empty() )
			currency = p_con->product_info().currency;

		if( p_con->product_info().currency != currency )
			ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "ValidateCurrency"
				, utility::FormatStr("contract has different currency: conid: %d %s %s"
				, p_con->contract_id, p_con->product_info().currency.c_str(), currency.c_str()));
	});

	return currency;
}

std::string TSystem::contract::ValidateTicker(const contract::ContractManager& mgt)
{
	std::string ticker_name;
	std::for_each( std::begin(mgt.AllContracts()), std::end(mgt.AllContracts())
		,[&ticker_name](const std::unique_ptr<contract::Contract>& p_con )
	{
		if( ticker_name.empty() )
			ticker_name = p_con->ticker_name;

		if( p_con->ticker_name != ticker_name )
			ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "ValidateTicker"
				, utility::FormatStr("contract has different ticker: conid: %d %s %s"
				, p_con->contract_id, p_con->ticker_name.c_str(), ticker_name.c_str()));
	});

	return ticker_name;
}

TSystem::contract::ProductInfo::DeliveryType TSystem::contract::ValidateDelivery(const contract::ContractManager& mgt)
{
	TSystem::contract::ProductInfo::DeliveryType delivery = TSystem::contract::ProductInfo::DeliveryType::NONE;
	std::for_each( std::begin(mgt.AllContracts()), std::end(mgt.AllContracts())
		,[&delivery](const std::unique_ptr<contract::Contract>& p_con )
	{
		if( delivery == TSystem::contract::ProductInfo::DeliveryType::NONE  )
			delivery = p_con->product_info().delivery_type;

		if( p_con->product_info().delivery_type != delivery )
			ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "ValidateDelivery"
				, utility::FormatStr("contract has different delivery: conid: %d %s %s"
				, p_con->contract_id
				, TSystem::contract::DeliveryTypeToString(p_con->product_info().delivery_type).c_str()
				, TSystem::contract::DeliveryTypeToString(delivery)));
	});

	return delivery;
}

std::set<TSystem::contract::ProductInfo::ContractType> TSystem::contract::ValidateContractTypes(const contract::ContractManager& mgt
							, const std::set<TSystem::contract::ProductInfo::ContractType>& target_types)
{
	std::set<TSystem::contract::ProductInfo::ContractType> res;
	std::for_each(mgt.GroupedContracts().begin(), mgt.GroupedContracts().end()
		, [&res](const std::pair<const TSystem::contract::ProductInfo::ContractType, std::vector<contract::Contract*>>& group)
	{ res.insert(group.first); });

	if( res.size() != target_types.size() )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
			, "ValidateContractTypes"
			, "loaded contract has different types from target types");

	if( !std::equal(std::begin(target_types), std::end(target_types), std::begin(res)) )
	{
		// find different types
		std::for_each(std::begin(res), std::end(res), [&target_types](TSystem::contract::ProductInfo::ContractType type)
		{
			if( target_types.find(type) == target_types.end() )
				ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "ValidateTraderInfo"
				, "loaded contract has different types from target types: " 
					+ contract::ContractTypeToString(type) );
		});
	}

	return res;
}

std::string TSystem::contract::ValidateEcp(const contract::ContractManager& mgt
										   , const std::string& ecp_name
										   , const contract::ContractList& contract_list)
{
	const std::set<int>& ecp_conids = contract_list.GetContractList(ecp_name, ContractList::ResourceType::ECP);

	std::for_each( std::begin(mgt.AllContracts()), std::end(mgt.AllContracts())
		,[&ecp_conids, &ecp_name](const std::unique_ptr<contract::Contract>& p_con )
	{
		if( ecp_conids.find(p_con->contract_id) == ecp_conids.end() )
			ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "ValidateEcp"
				, utility::FormatStr("contract id is not assigned to ecp: %d, %s"
				, p_con->contract_id, ecp_name.c_str()));
	});

	return ecp_name;
}
