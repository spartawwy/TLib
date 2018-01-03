// by Xixin Silas Cheng

#ifndef TSYSTEM_RETURN_CODE_H_
#define TSYSTEM_RETURN_CODE_H_

#include "TLib/core/tsystem_error.h"
#include "TLib/core/tsystem_return_code.pb.h"

namespace TSystem
{
	// fill network return code
	void FillReturnCode(int code, const std::string& str, ReturnCode& msg);
	void FillReturnCode(int code, const char* str, ReturnCode& msg);

	// convert a return code to a TError
	template<typename ErrorCode>
	TError MakeTError(const ReturnCode& rc, std::string func_name)
	{
		TError te = MakeTError(static_cast<ErrorCode>(rc.code()));
		te.func_name(std::move(func_name));
		te.other_info(rc.error_msg());
		return te;
	}

	//------------------
	// fill reqeust ack with request id 
	//------------------

	// request ok(code 0)
	void FillRequestAck(int request_id, RequestAck& msg);

	// request code with message
	void FillRequestAck(int request_id, int code, const std::string& str, RequestAck& msg);
	void FillRequestAck(int request_id, int code, const char* str, RequestAck& msg);

	//------------------------------
	// fill request without request id
	// a default constructed request ack means request ok without request id
	//------------------------------

	// request code with message
	void FillRequestAck(int code, const std::string& str, RequestAck& msg);
	void FillRequestAck(int code, const char* str, RequestAck& msg);

	// to string
	std::string ToString( const ReturnCode& code);
}

#endif // TSYSTEM_RETURN_CODE_H_