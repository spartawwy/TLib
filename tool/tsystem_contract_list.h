#ifndef TSYSTEM_CONTRACT_LIST_H_
#define TSYSTEM_CONTRACT_LIST_H_

#include <tuple>

#include "TLib/tool/tsystem_coninfo.h"

namespace TSystem
{
namespace utility
{
	inline std::string ContractListDir()
	{
		return ContractDir() + "contract_list//";
	}
}

namespace contract
{
	// used to read contract list for given resource.
	// not all resource has a contract list, but some have like ecp / pricer
	// throw TException if no contract found
	class ContractList
	{
	public:

		enum class ResourceType:short
		{
			PRICER = 0,
			ECP
		};

		ContractList();
		explicit ContractList(const std::string& tag);

		bool Open(const std::string& tag, TError* te);
		void Open(const std::string& tag); // version to throw TException

		// get contract id list for given resource
		const std::set<int>* FindContractList(const std::string& res_name, ResourceType type) const;
		const std::set<int>& GetContractList(const std::string& res_name, ResourceType type) const; // throw TException when not found

		// register user id, update resource_query_map_, throw exception if contract resource already set
		void RegisterUserID(const std::string& res_name, int user_id);

		// get resource name based on contract id and user id
		const std::string* FindResourceName(int contract_id, int user_id, ResourceType type) const;
		const std::string& GetResourceName(int contract_id, int user_id, ResourceType type) const; // throw TException when not found

	private:

		std::map<std::tuple<std::string, ResourceType>, std::set<int>>  conid_list_holder_; 

		typedef std::unordered_map<ResourceType, const std::string*>	TypeMap;					// (ResourceType -> name )
		typedef std::unordered_map<int, TypeMap>						ContractMap;			   // (contract_id -> TypeToResourceMap)
		std::unordered_map<int, ContractMap>							resource_query_map_;	   // (user_id -> ContractToResourceMap)
	};

	ContractList::ResourceType StringToResourceType( const std::string& type_str); // throw TException if unkown string of resource type
	const std::string& ResourceTypeToString( ContractList::ResourceType type ); // throw TException if unkown resource type

}
}

#endif // TSYSTEM_CONTRACT_LIST_H_