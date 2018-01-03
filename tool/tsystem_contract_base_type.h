#ifndef TSYSTEM_CONTRACT_BASE_TYPE_H_
#define TSYSTEM_CONTRACT_BASE_TYPE_H_

namespace TSystem
{
namespace contract
{
	// empty base class for contract type def
	// no virtual destructor for performance consideration
	// and make sure the derived structs do not need destructor
	struct ContractType 
	{
		ContractType(): expiration(0), multiplier(1) {}

		int expiration;
		double multiplier;
	};

	// utility, static check if derived from ContractTypeDef
	template<typename DefinitionType>
	bool CheckBaseClass()
	{
		static_assert( std::is_base_of<ContractType,DefinitionType>::value
			, "Not derived from struct ContractType" );
		return true;
	};
}
}

#endif // TSYSTEM_CONTRACT_BASE_TYPE_H_
