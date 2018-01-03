#include "tsystem_contract.h"

#include <algorithm>
#include <sstream>

#include "TLib/core/tsystem_core_common.h"

using namespace TSystem;
using namespace TSystem::contract;

Contract::Contract()
	: contract_id(0)
	, contract_status(ContractStatus::NOTRADE)
	, exchange_sym()
	, trade_hour()
	, ticker_name()
	, tag()
	, contract_def_()
	, product_info_()
{}

const std::string& TSystem::contract::ContractStatusToString( Contract::ContractStatus t )
{
	static std::string notrade = "NOTRADE";
	static std::string active  = "ACTIVE";
	static std::string dead    = "DEAD";

	switch (t)
	{
	case TSystem::contract::Contract::ContractStatus::NOTRADE:
		return notrade;
	case TSystem::contract::Contract::ContractStatus::ACTIVE:
		return active;
	case TSystem::contract::Contract::ContractStatus::DEAD:
		return dead;
	}

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "ContractStatusToString"
		, utility::FormatStr("unknown ContractStatus: %d", static_cast<short>(t))); 
}

Contract::ContractStatus TSystem::contract::StringToContrctStatus(const std::string& s)
{
	static std::string notrade = "NOTRADE";
	static std::string active  = "ACTIVE";
	static std::string dead    = "DEAD";

	if( s == notrade )
		return Contract::ContractStatus::NOTRADE;
	else if( s == active )
		return Contract::ContractStatus::ACTIVE;
	else if( s == dead )
		return Contract::ContractStatus::DEAD;

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "StringToContrctStatus"
		, utility::FormatStr("unknown ContractStatus string: %s", s.c_str())); 
}

//---------------------
// ContractManager
//---------------------

ContractManager::ContractManager()
	: id_maps_(1024)
	, sym_maps_(1024)
	, group_maps_()
	, contract_holder_()
{
	contract_holder_.reserve(1024);
}

ContractManager::ContractManager( ContractManager&& in )
	: id_maps_(std::move(in.id_maps_))
	, sym_maps_(std::move(in.sym_maps_))
	, group_maps_(std::move(in.group_maps_))
	, contract_holder_(std::move(in.contract_holder_))
{}

ContractManager& ContractManager::operator=( ContractManager&& in)
{
	if( this != &in )
	{
		id_maps_ = std::move(in.id_maps_);
		sym_maps_ = std::move(in.sym_maps_);
		group_maps_ = std::move(in.group_maps_);
		contract_holder_ = std::move(in.contract_holder_);
	}

	return *this;
}

ContractManager& ContractManager::operator<<(ContractManager&& right)
{
	if( this != &right )
	{
		size_t size_1 = contract_holder_.size();
		size_t size_2 = right.contract_holder_.size();
		contract_holder_.resize(size_1+size_2);
		auto iter = std::begin(contract_holder_)+size_1;
		std::move(std::begin(right.contract_holder_), std::end(right.contract_holder_), iter);
		MapContracts();
	}

	return *this;
}

void ContractManager::MapContracts()
{
	// sort by conid and remove duplicated contract
	std::sort(std::begin(contract_holder_), std::end(contract_holder_), 
		[](const std::unique_ptr<Contract>& left, const std::unique_ptr<Contract>& right)->bool
	{ return left->contract_id < right->contract_id; });

	auto iter = std::unique(std::begin(contract_holder_), std::end(contract_holder_), 
		[](const std::unique_ptr<Contract>& left, const std::unique_ptr<Contract>& right)->bool
	{ return left->contract_id == right->contract_id; });
	contract_holder_.resize(std::distance(std::begin(contract_holder_), iter));

	// sort by contract operator<
	std::sort(std::begin(contract_holder_), std::end(contract_holder_), 
		[](const std::unique_ptr<Contract>& left, const std::unique_ptr<Contract>& right)->bool
	{ return *left < *right; });

	// map
	id_maps_.clear();
	sym_maps_.clear();
	group_maps_.clear();

	std::for_each(std::begin(contract_holder_), std::end(contract_holder_),[this](const std::unique_ptr<Contract>& p_con)
	{
		this->id_maps_.insert(std::make_pair(p_con->contract_id, p_con.get()));
		this->sym_maps_.insert(std::make_pair(p_con->exchange_sym, p_con.get()));
		auto iter = this->group_maps_.find(p_con->product_info().contract_type);
		if( iter == this->group_maps_.end() )
		{
			iter = group_maps_.insert(std::make_pair(p_con->product_info().contract_type, std::vector<Contract*>())).first;
			iter->second.reserve(1024);
		}
		iter->second.push_back(p_con.get());
	});
}

const Contract* ContractManager::FindContractID(int conid) const
{
	auto it = id_maps_.find(conid);
	return it != id_maps_.end() ? it->second : nullptr;
}

const Contract* ContractManager::FindExchangeSymbol(const std::string& sym) const
{
	auto it = sym_maps_.find(sym);
	return it != sym_maps_.end() ? it->second : nullptr;
}

std::set<int> TSystem::contract::FindMissingContractIDs(const ContractManager& mgt, const std::set<int>& conids)
{
	std::set<int> res;
	std::for_each(std::begin(conids), std::end(conids)
		, [&res, &mgt](int conid)
	{
		if( !mgt.FindContractID(conid))
			res.insert(conid);
	});
	return res;
}

void TSystem::contract::AssertMissingContractIDs(const ContractManager& mgt, const std::set<int>& conids)
{
	std::set<int> res = FindMissingContractIDs(mgt, conids);
	if( res.size() > 0 )
	{
		std::stringstream ss;
		auto iter = std::begin(res);
		ss << *iter++;
		while( iter != std::end(res) )
			ss << " " << *iter++;
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
			, "AssertMissingContractIDs"
			, "contract manager missing contract ids: " + ss.str());
	}
}

bool  TSystem::contract::operator<(const Contract& left, const Contract& right)
{
	if( left.product_info().name != right.product_info().name )
		return left.product_info().name < right.product_info().name;

	switch (left.product_info().contract_type)
	{
	case ProductInfo::ContractType::SPOT:
		return left.contract_def<SpotType>() < right.contract_def<SpotType>();
	case ProductInfo::ContractType::FUTURE:
		return left.contract_def<FutureType>() < right.contract_def<FutureType>(); 
	case ProductInfo::ContractType::OPTION:
		return left.contract_def<OptionType>() < right.contract_def<OptionType>(); 
	case ProductInfo::ContractType::BINARY_OPTION:
		return left.contract_def<BinaryOptionType>() < right.contract_def<BinaryOptionType>(); 
	case ProductInfo::ContractType::FUTURE_OPTION:
		return left.contract_def<FutureOptionType>() < right.contract_def<FutureOptionType>(); 
	}

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "operator<(const Contract& left, const Contract& right)"
		, utility::FormatStr("unknown ContractType: %d", static_cast<short>(left.product_info().contract_type))); 
}