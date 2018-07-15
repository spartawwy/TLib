#include "tsystem_contract_list.h"

#include "TLib/core/tsystem_sqlite_functions.h"

using namespace TSystem;
using namespace TSystem::contract;

ContractList::ContractList()
	: conid_list_holder_()
	, resource_query_map_(8)
{}

ContractList::ContractList(const std::string& tag)
	: conid_list_holder_()
	, resource_query_map_(8)
{
	Open(tag);
}

bool ContractList::Open(const std::string& tag, TError* te)
{
	std::shared_ptr<SQLite::SQLiteConnection> db_conn = std::make_shared<SQLite::SQLiteConnection>();
	std::string full_db_name = utility::ContractListDir() + tag + ".conlist";
	try
	{
		if( db_conn->Open(full_db_name.c_str(), SQLite::SQLiteConnection::OpenMode::READ) != SQLite::SQLiteCode::OK )
		{
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
				, "ContractList::Open"
				, "can't open contract list file: " + full_db_name);
		}


		if( !utility::ExistTable("ContractResource", *db_conn) )
		{
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "ContractList::Open"
				, "can't find table ContractResource");
		}

		// to do, load info into conid_list_holder_
		std::string sql = "SELECT resource_name, resource_type, contract_id FROM ContractResource "
				 " Order By resource_type, resource_name";

		std::tuple<std::string, ResourceType> current_res_name;
		std::set<int>* p_current_conids;
		db_conn->ExecuteSQL( sql.c_str(), [this, &current_res_name, &p_current_conids](int cols, char** vals, char** names)->int
		{
			std::tuple<std::string, ResourceType> res_tag = std::make_tuple( std::string(*vals), StringToResourceType(*(vals+1)));

			if( current_res_name != res_tag )
			{
				current_res_name = res_tag;
				p_current_conids = &conid_list_holder_[current_res_name];
			}

			p_current_conids->insert(atoi(*(vals+2)));
			return 0;
		});

		db_conn->Close();
	}catch( const SQLite::SQLiteException& e)
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "ContractList::Open"
			, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()));
	}

	return true;

}

void ContractList::Open(const std::string& tag)
{
	TError te;
	if( !Open(tag, &te) )
		throw TException(std::move(te));
}

const std::set<int>* ContractList::FindContractList(const std::string& res_name, ResourceType type) const
{
	auto iter = conid_list_holder_.find(std::make_pair(res_name, type));
	return iter != conid_list_holder_.end() ? &iter->second : nullptr;
}

const std::set<int>& ContractList::GetContractList(const std::string& res_name, ResourceType type) const
{
	const std::set<int>* p = FindContractList(res_name, type);
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "ContractList::GetContractList"
			, utility::FormatStr("can't find contract list for resource with type: %s, %d " 
				, res_name.c_str(), static_cast<short>(type)));

	return *p;
}

void ContractList::RegisterUserID(const std::string& res_name, int user_id)
{
	auto iter = std::find_if(std::begin(conid_list_holder_), std::end(conid_list_holder_)
		, [&res_name](const std::pair<const std::tuple<std::string, ResourceType>, std::set<int>>& res_entry)->bool
	{return std::get<0>(res_entry.first) == res_name;});

	if( iter != conid_list_holder_.end() )
	{
		const std::tuple<std::string, ResourceType>& res_info = iter->first;
		const std::set<int>& contract_ids = iter->second;

		auto user_iter = resource_query_map_.find(user_id);
		if( user_iter == resource_query_map_.end() )
			user_iter = resource_query_map_.insert(std::make_pair(user_id, ContractMap(256))).first;
		
		std::for_each(std::begin(contract_ids), std::end(contract_ids), [&res_info, user_iter](int contract_id)
		{
			auto contract_iter = user_iter->second.find(contract_id);
			if( contract_iter == user_iter->second.end() )
				contract_iter = user_iter->second.insert(std::make_pair(contract_id, TypeMap(4))).first;
			
			if( !contract_iter->second.insert(std::make_pair(std::get<1>(res_info), &std::get<0>(res_info))).second )
			{
				ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
					, "ContractList::RegisterUserID"
					, utility::FormatStr("contract resource already set: conid: %d, user id: %d, type: %s"
						, contract_id
						, user_iter->first
						, ResourceTypeToString(std::get<1>(res_info)).c_str()));
			}
		});
	}
}

const std::string* ContractList::FindResourceName(int contract_id, int user_id, ResourceType type) const
{
	auto user_iter = resource_query_map_.find(user_id);
	if( user_iter == resource_query_map_.end() )
		return nullptr;

	auto contract_iter = user_iter->second.find(contract_id);
	if( contract_iter == user_iter->second.end() )
		return nullptr;
	
	auto type_iter = contract_iter->second.find(type);
	if( type_iter == contract_iter->second.end() )
		return nullptr;

	return type_iter->second;
}
	
const std::string& ContractList::GetResourceName(int contract_id, int user_id, ResourceType type) const
{
	const std::string* p_name = FindResourceName(contract_id, user_id, type);
	if( !p_name )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "ContractList::GetResourceName"
			, utility::FormatStr("can't find resource name for: conid: %d, user_id: %d, type: %s"
				, contract_id
				, user_id
				, ResourceTypeToString(type).c_str()));
	return *p_name;
}


ContractList::ResourceType TSystem::contract::StringToResourceType( const std::string& type_str)
{
	static std::string pricer = "pricer";
	static std::string ecp    = "ecp";

	if( type_str == pricer )
		return TSystem::contract::ContractList::ResourceType::PRICER;
	else if( type_str == ecp )
		return TSystem::contract::ContractList::ResourceType::ECP;

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "TSystem::contract::StringToResourceType"
		, "unknown ResourceType string: " + type_str);
    return TSystem::contract::ContractList::ResourceType::PRICER;
}

const std::string& TSystem::contract::ResourceTypeToString( ContractList::ResourceType type )
{
	static std::string pricer = "pricer";
	static std::string ecp    = "ecp";

	switch (type)
	{
	case TSystem::contract::ContractList::ResourceType::PRICER:
		return pricer;
	case TSystem::contract::ContractList::ResourceType::ECP:
		return ecp;
	}

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "TSystem::contract::ResourceTypeToString"
		, utility::FormatStr("unknown ResourceType: %d", static_cast<short>(type)));
    return pricer;
}