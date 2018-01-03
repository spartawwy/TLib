#ifndef TSYSTEM_CORE_ERROR_H_
#define TSYSTEM_CORE_ERROR_H_

#include "tsystem_error.h"

namespace TSystem
{
	// core error category
	class CoreErrorCategory : public TErrorCategory
	{
	public:

		enum class ErrorCode : short 
		{
			OK           = 0 ,
			BAD_ARGUMENT = 1 , // bad runtime variable values
			BAD_CONTENT  = 2 , // bad external information
			BAD_SOCKET   = 3 , // bad system socket
			BAD_IO       = 4 , // bad system io
			BAD_NETWORK  = 5   // bad network
		};

		// singleton accessor
		static CoreErrorCategory& GetCategory();

		virtual const std::string& name() const;

		virtual bool  CheckOK( short ec ) const;

		virtual const std::string& Message( short ec ) const;

	private:
		CoreErrorCategory();
	};

	// make terror from error code of the category
	// this function will be used in utility templates defined in tsystem_error.h
	inline TError MakeTError( CoreErrorCategory::ErrorCode ec )
	{
		return TError(static_cast<short>(ec), CoreErrorCategory::GetCategory());
	}
}

#endif // TSYSTEM_CORE_ERROR_H_