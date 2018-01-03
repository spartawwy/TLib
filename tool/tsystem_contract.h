#ifndef TSYSTEM_CONTRACT_H_
#define TSYSTEM_CONTRACT_H_

#include <cassert>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>

#include "tsystem_contract_product.h"
#include "tsystem_contract_type.h"

namespace TSystem
{
namespace contract
{

	class ConInfo;

	// struct to save contract info
	struct Contract
	{
		enum class ContractStatus: short
		{
			NOTRADE = 0, 
			ACTIVE  = 1,
			DEAD    = 2
		};

		Contract();

		operator bool() const { return contract_id != 0; }
		
		//-------------
		// get type def
		//-------------
		template<typename DefinitionType>
		const DefinitionType& contract_def() const 
		{
			assert( CheckBaseClass<DefinitionType>() );
			return *reinterpret_cast<DefinitionType*>(contract_def_.get()); 
		}

		const ContractType& contract_def() const { assert(contract_def_); return *contract_def_; }
		const ProductInfo& product_info() const { assert(product_info_); return *product_info_; }

		int					contract_id;	 // contract id
		ContractStatus      contract_status; // contract status
		std::string			exchange_sym;    // symbol on exchange
		std::string			trade_hour;      // trading hour, can be queried from layinfo
											 // if not longdate is not specified when loading
											 // use the default trade hour
		std::string         ticker_name;     // price source ticker name
		std::string         tag;             // tag of the contract

	private:
		std::shared_ptr<ContractType>	contract_def_;  // to make the object copyable, use shared_ptr
		std::shared_ptr<ProductInfo>    product_info_;  // product info

		friend class ConInfo;
	};

	// throw TException if unkown contract type
	bool operator<(const Contract& left, const Contract& right);

	const std::string& ContractStatusToString( Contract::ContractStatus t ); // throw TException if unkown contract status
	Contract::ContractStatus StringToContrctStatus(const std::string& s); // throw TException if unkown string of contract status

	//--------------------------------
	// manager of a group of contracts
	//--------------------------------
    class ContractManager
    {
	public:

		typedef std::map<ProductInfo::ContractType, std::vector<Contract*>> ContractGroupType;

		ContractManager();
		ContractManager( ContractManager&& in );
		ContractManager& operator=( ContractManager&& in);

		// merge other contract manager
		ContractManager& operator<<(ContractManager&& right);
	    
	    // query by contract id, O(1)
	    const Contract* FindContractID(int conid) const;

		// query by exchange symbol, O(1)
		const Contract* FindExchangeSymbol(const std::string& sym) const;

		// all contracts
		const std::vector<std::unique_ptr<Contract>>& AllContracts() const { return contract_holder_; }

		// all contracts by type group
		const ContractGroupType& GroupedContracts() const { return group_maps_; }

	private:

		// remove duplicaited contract_id, and map contract id and symbol
		// throw TException if unkown contract type of contracts in holder
		void MapContracts();
			
		// query by contract id
	    std::unordered_map<int,Contract*> id_maps_;

		// query by contract exchange symbol
		std::unordered_map<std::string, Contract*> sym_maps_;

		ContractGroupType						group_maps_;

		// contract holder
		std::vector<std::unique_ptr<Contract>>	contract_holder_;

		friend class ConInfo;
	};

	// find missing conids
	std::set<int> FindMissingContractIDs(const ContractManager& mgt, const std::set<int>& conids);

	// assert missing conids, throw TException if not match
	void AssertMissingContractIDs(const ContractManager& mgt, const std::set<int>& conids);
}
}

#endif // TSYSTEM_CONTRACT_H_