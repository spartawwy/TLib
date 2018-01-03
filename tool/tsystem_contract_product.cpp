#include "tsystem_contract_product.h"

#include "TLib/core/tsystem_core_common.h"

using namespace TSystem;
using namespace TSystem::contract;

const std::string& TSystem::contract::ContractTypeToString( ProductInfo::ContractType t )
{
	static std::string spot  = "spot";
	static std::string future = "future";
	static std::string option = "option";
	static std::string future_option = "future_option";
	static std::string binary_option = "binary_option";

	switch (t)
	{
	case TSystem::contract::ProductInfo::ContractType::SPOT:
		return spot;
	case TSystem::contract::ProductInfo::ContractType::FUTURE:
		return future;
	case TSystem::contract::ProductInfo::ContractType::OPTION:
		return option;
	case TSystem::contract::ProductInfo::ContractType::FUTURE_OPTION:
		return future_option;
	case TSystem::contract::ProductInfo::ContractType::BINARY_OPTION:
		return binary_option;
	}

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "ContractTypeToString"
		, utility::FormatStr("unknown ContractType: %d", static_cast<short>(t))); 
}

ProductInfo::ContractType TSystem::contract::StringToContractType(const std::string& s)
{
	static std::string spot  = "spot";
	static std::string future = "future";
	static std::string option = "option";
	static std::string future_option = "future_option";
	static std::string binary_option = "binary_option";

	if( s == spot )
		return ProductInfo::ContractType::SPOT;
	else if( s == future )
		return ProductInfo::ContractType::FUTURE;
	else if( s == option )
		return ProductInfo::ContractType::OPTION;
	else if( s == future_option )
		return ProductInfo::ContractType::FUTURE_OPTION;
	else if( s == binary_option )
		return ProductInfo::ContractType::BINARY_OPTION;

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "StringToContractType"
		, utility::FormatStr("unknown contract type string: %s", s.c_str())); 
}

const std::string& TSystem::contract::DeliveryTypeToString( ProductInfo::DeliveryType t )
{
	static std::string none = "NONE";
	static std::string cash = "CASH";
	static std::string physical = "PHYSICAL";

	switch (t)
	{
	case TSystem::contract::ProductInfo::DeliveryType::NONE:
		return none;
	case TSystem::contract::ProductInfo::DeliveryType::CASH:
		return cash;
	case TSystem::contract::ProductInfo::DeliveryType::PHYSICAL:
		return physical;
	}

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "DeliveryTypeToString"
		, utility::FormatStr("unknown DeliveryType: %d", static_cast<short>(t))); 
}

ProductInfo::DeliveryType TSystem::contract::StringToDeliveryType(const std::string& s)
{
	static std::string none = "NONE";
	static std::string cash = "CASH";
	static std::string physical = "PHYSICAL";

	if( s == none )
		return ProductInfo::DeliveryType::NONE;
	else if( s == cash )
		return ProductInfo::DeliveryType::CASH;
	else if( s == physical )
		return ProductInfo::DeliveryType::PHYSICAL;

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "StringToDeliveryType"
		, utility::FormatStr("unknown DeliveryType string: %s", s.c_str())); 
}