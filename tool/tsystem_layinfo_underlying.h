#ifndef TSYSTEM_LAYINFO_UNDERLYING_H_
#define TSYSTEM_LAYINFO_UNDERLYING_H_

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "TLib/tool/tsystem_layinfo_exchange.h"

namespace TSystem
{
namespace layinfo
{
	class LayInfo;

	// struct to load underlying information
	struct Underlying
	{
		enum class UnderlyingType: short 
		{
			EQUITY	  = 0,
			COMMODITY 
		};

		Underlying(): name(), type(UnderlyingType::EQUITY), exchange_(), is_index(false) {}

		operator bool() const {return !name.empty();}

		const Exchange& exchange() const { assert(exchange_); return *exchange_; }

		std::string     name;		// name of the underlying	
		UnderlyingType  type;		// type of the underlying
		bool            is_index;   // if an index
	
	private:

		std::shared_ptr<Exchange>	exchange_;

		friend class LayInfo;
	};


	const std::string&		   UnderlyingTypeToString(Underlying::UnderlyingType t); // throw TException if unkown type
	Underlying::UnderlyingType StringToUnderlyingType(const std::string& s);  // throw TException if unkown type of string

	// manager of a group of underlying
	// loaded by LayInfo
	class UnderlyingManager
	{
	public:

		UnderlyingManager();

		// find underlying by name, O(1)
		const Underlying* FindName(const std::string& name) const;
		const Underlying& GetName(const std::string& name) const; // throw TException when not found

		// all underlying
		const std::vector<std::shared_ptr<Underlying>>& AllUnderlying() const { return underlying_holder_; }

	private:

		// query by name
		std::unordered_map<std::string, Underlying*>  name_maps_;

		// underlying holder
		std::vector<std::shared_ptr<Underlying>>	underlying_holder_;

		friend class LayInfo;
	};
}
}

#endif // TSYSTEM_LAYINFO_UNDERLYING_H_