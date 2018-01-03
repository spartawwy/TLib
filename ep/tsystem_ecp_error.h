//by Xixin Silas Cheng

#ifndef TSYSTEM_ECP_ERROR_H_
#define TSYSTEM_ECP_ERROR_H_

#include <TLib/core/tsystem_error.h>

namespace TSystem
{
	class EcpErrorCategory: public TSystem::TErrorCategory
	{
	public:
		enum class ErrorCode : short
		{
			OK          = 0,
			BAD_REQUEST = 1, // error from client request
			BAD_LOGIN   = 2, // error from client login
			BAD_ORDER   = 3, // error from client order
			BAD_API     = 4, // error caused from exchange API
			BAD_SETTING = 5, // error caused by local ecp setting
			BAD_EXCHANGE = 6, // impossible error from exchange
			REQUEST_TIME_OUT = 7, // request time out
			EXCHANGE_FLOW_CONTROL = 8 // exchange flow control
		};

		// singleton accessor
		static EcpErrorCategory& GetCategory();
		
		virtual const std::string& name() const;

		virtual bool  CheckOK(short ec) const;

		virtual const std::string& Message(short ec ) const;

	private:
		EcpErrorCategory();
	};

	// make a TError based on CtpErrorCategory
	TError     MakeTError( EcpErrorCategory::ErrorCode ec );

	// make a TException based on CtpErrorCategory
	TException MakeTException( EcpErrorCategory::ErrorCode ec );
};


#endif // TSYSTEM_ECP_ERROR_H_
