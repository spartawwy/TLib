#include "tsystem_layinfo_underlying.h"

#include "TLib/core/tsystem_core_common.h"

using namespace TSystem::layinfo;
using namespace TSystem;

const std::string& TSystem::layinfo::UnderlyingTypeToString(Underlying::UnderlyingType t)
{
	static std::string equity = "equity";
	static std::string commodity = "commodity";

	switch (t)
	{
	case TSystem::layinfo::Underlying::UnderlyingType::EQUITY:
		return equity;
	case TSystem::layinfo::Underlying::UnderlyingType::COMMODITY:
		return commodity;
	}

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "UnderlyingTypeToString"
		, utility::FormatStr("unknown UnderlyingType: %d", static_cast<short>(t))); 

	// should never come to this line
	assert(0);
	return equity;
}

Underlying::UnderlyingType TSystem::layinfo::StringToUnderlyingType(const std::string& s)
{
	static std::string equity = "equity";
	static std::string commodity = "commodity";

	 if( s == equity )
		return Underlying::UnderlyingType::EQUITY;
	else if( s == commodity )
		return Underlying::UnderlyingType::COMMODITY;

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "StringToUnderlyingType"
		, utility::FormatStr("unknown UnderlyingType string: %s", s.c_str())); 

	// should never come to this line
	assert(0);
	return Underlying::UnderlyingType::EQUITY;
};

//--------------------
// UnderlyingManager
//--------------------
UnderlyingManager::UnderlyingManager()
	: name_maps_(1024)
	, underlying_holder_()
{
	underlying_holder_.reserve(1024);
}

const Underlying* UnderlyingManager::FindName(const std::string& name) const
{
	auto iter = name_maps_.find(name);
	return iter != name_maps_.end() ? iter->second : nullptr;
}

const Underlying& UnderlyingManager::GetName(const std::string& name) const
{
	const Underlying* p = FindName(name);
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "UnderlyingManager::GetName"
			, "can't find underlying: " + name);
	return *p;
}