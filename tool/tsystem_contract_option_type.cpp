#include "tsystem_contract_option_type.h"

#include "TLib/core/tsystem_core_common.h"

using namespace TSystem;
using namespace TSystem::contract;

const std::string& TSystem::contract::ExpireStyleToString( OptionType::ExpireStyle t)
{
	static std::string european = "E";
	static std::string american = "A";

	switch (t)
	{
	case TSystem::contract::OptionType::ExpireStyle::EUROPEAN:
		return european;
	case TSystem::contract::OptionType::ExpireStyle::AMERICAN:
		return american;
	}

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "ExpireStyleToString"
		, utility::FormatStr("unknown ExpireStyle: %d", static_cast<short>(t))); 
}

OptionType::ExpireStyle TSystem::contract::StringToExpireStyle( const std::string& s)
{
	static std::string european = "E";
	static std::string american = "A";

	if( s == european )
		return OptionType::ExpireStyle::EUROPEAN;
	else if( s == american )
		return OptionType::ExpireStyle::AMERICAN;

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "StringToExpireStyle"
		, utility::FormatStr("unknown ExpireStyle string: %s", s.c_str())); 
}

const std::string& TSystem::contract::CallPutToString( OptionType::CallPut t )
{
	static std::string call = "C";
	static std::string put = "P";

	switch (t)
	{
	case TSystem::contract::OptionType::CallPut::CALL:
		return call;
	case TSystem::contract::OptionType::CallPut::PUT:
		return put;
	}

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "CallPutToString"
		, utility::FormatStr("unknown CallPut: %d", static_cast<short>(t))); 
}

OptionType::CallPut TSystem::contract::StringToCallPut( const std::string& s )
{
	static std::string call = "C";
	static std::string put = "P";

	if( s == call )
		return OptionType::CallPut::CALL;
	else if( s == put )
		return OptionType::CallPut::PUT;

	ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "StringToCallPut"
		, utility::FormatStr("unknown CallPut string: %s", s.c_str())); 
}