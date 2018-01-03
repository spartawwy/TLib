#ifndef TSYSTEM_CONTRACT_PRODUCT_H_
#define TSYSTEM_CONTRACT_PRODUCT_H_

#include <string>

namespace TSystem
{
namespace contract
{
	// struct to save product info
	struct ProductInfo
	{
		// contract type defines how underlying price turns into contract price.
		// combined with underlying type, we have things like STOCK-SPOT, INDICE-SPOT
		// underlying type defines how underlying market price is produced.
		enum class ContractType: short
		{
			SPOT	  = 0	,
			FUTURE			,
			OPTION			,
			BINARY_OPTION   ,
			FUTURE_OPTION	,
		};

		enum class DeliveryType: short
		{
			NONE        = 0	,
			CASH			,
			PHYSICAL		,
		};

		ProductInfo()
			: under(), contract_type(ContractType::SPOT)
			, exchange(), exchange_prod_sym()
			, currency(), mkt_rule(), delivery_type(DeliveryType::NONE)
		{}

		
		std::string name;            // name of this product = <under>.<type>
		std::string under;		     // underlying, can be queried from layinfo 
		ContractType contract_type;  // instrument type

		std::string exchange;          // exchange, can be queried from layinfo
		std::string exchange_prod_sym; // product symbol on exchange
		
		std::string currency;			// quoting currency, can be queried from layinfo
		std::string	mkt_rule;			// market rule, can be queried from layinfo
		DeliveryType delivery_type;		// delivery type
	};

	const std::string& ContractTypeToString( ProductInfo::ContractType t ); // throw TException if unkown contract type
	ProductInfo::ContractType StringToContractType(const std::string& s); // throw TException if unkown string of contract type

	const std::string& DeliveryTypeToString( ProductInfo::DeliveryType t ); // throw TException if unkown delivery type
	ProductInfo::DeliveryType StringToDeliveryType(const std::string& s); // throw TException if unknown string of delivery type
}
}

#endif // TSYSTEM_CONTRACT_PRODUCT_H_s