#ifndef TSYSTEM_CONINFO_H_
#define TSYSTEM_CONINFO_H_

#include <set>
#include <string>
#include <memory>
#include <vector>

#include <SQLite/SQLite.h>

#include "TLib/core/tsystem_core_common.h"
#include "TLib/core/tsystem_core_paths.h"
#include "TLib/core/tsystem_time.h"
#include "TLib/tool/tsystem_contract.h"

namespace TSystem
{

namespace utility
{
	// contract dir
	inline std::string ContractDir()
	{
		return DataDir() + "contract//";	
	}
}

namespace contract
{
	// server as a common generator for all contract object, and also the 
	// place holder for all product info object
	// nearly all operation may throw TException if there is database IO error
	// the comments only means TException not caused by database IO operation 
	// add try-catch block to handle possible database IO error is necessary 
	class ConInfo
	{
	public:

		// contract predictor type
		typedef std::function<bool(const Contract&)>	ContractPredType;

		ConInfo();
		explicit ConInfo(const std::string& tag);

		bool Open(const std::string& tag, TError* te);
		void Open(const std::string& tag); // version to throw TException

		// find/get product info object
		std::shared_ptr<ProductInfo> FindProduct(const std::string& prod_name) const;
		const ProductInfo& GetProduct(const std::string& prod_name) const; // throw TException when not found

		// find / get / contract object
		std::shared_ptr<Contract> FindContract(int contract_id, int longdate = Today() );
		const Contract& GetContract(int contract_id, int longdate = Today() ); // throw TException when not found

		//----------------------------------------
		// load contract objects with longdate 
		// and pred(contract) == true if set pred
		//----------------------------------------

		// load contracts with given conids
		ContractManager LoadContract(const std::set<int>& contract_ids
			, int longdate = Today(), ContractPredType&& pred = ContractPredType());

		// load contracts with given contract status
		ContractManager LoadContract(Contract::ContractStatus status = Contract::ContractStatus::ACTIVE
			, int longdate = Today(), ContractPredType&& pred = ContractPredType());

		// load contracts with given ticker name
		ContractManager LoadTickerContract( const std::string& ticker, Contract::ContractStatus status = Contract::ContractStatus::ACTIVE
			, int longdate = Today(), ContractPredType&& pred = ContractPredType());

		// load contracts with given product name, won't check if a valid product
		ContractManager LoadProductContract( const std::string& product, Contract::ContractStatus status = Contract::ContractStatus::ACTIVE
			, int longdate = Today(), ContractPredType&& pred = ContractPredType());

	private:

		// load all products in the database, will be called from Open
		void LoadAllProducts();

		// core helper to load contracts
		typedef std::map<ProductInfo::ContractType, std::map<int, std::unique_ptr<Contract>>> ContractMapType;
		ContractMapType LoadContractHelper(const std::string* where_clause, int longdate , ContractPredType&& pred );

		static void FillContractManager(ContractMapType&& contracts, ContractManager& mgt);

		// the database connection
		std::shared_ptr<SQLite::SQLiteConnection>	db_conn_;
		
		std::vector<std::shared_ptr<ProductInfo>>	products_;
		std::vector<std::shared_ptr<Contract>>		contracts_;
	};
}
}

#endif // TSYSTEM_CONINFO_H_