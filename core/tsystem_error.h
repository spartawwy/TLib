// By Xixin Silas Cheng

#ifndef TSYSTEM_ERROR_H_
#define TSYSTEM_ERROR_H_

#include <cassert>
#include <exception>
#include <string>

namespace TSystem
{
	//-------------------------------------------------------
	// error category interface, get error message
	// library user should derive their error category from 
	// this interface class
	// see core error category for example
	//-------------------------------------------------------
	class TErrorCategory
	{
	public:

		// name of the error category
		virtual const std::string& name() const  = 0;

		// if OK
		virtual bool CheckOK( short ec ) const = 0;

		// error code's message
		virtual const std::string& Message( short ec ) const = 0;
	};

	// system wide error 
	class TError
	{
	public:
		TError( short code, TErrorCategory& err_cate );
		TError( TError&& in);
		TError();
	
		// = operator
		TError& operator=(TError&& in);
		TError& operator=(TError& in);

		// bool operator
		operator bool() const {assert(error_cate_); return !error_cate_->CheckOK(error_code_); }

		// accessors
		short code() const { return error_code_; }
		const TErrorCategory& category() const 
        {
            assert(error_cate_); 
            return *error_cate_; 
        }

		void func_name(const std::string& func) { func_name_ = func;}
		void func_name(std::string&& func ) { func_name_ = std::move(func); }
		const std::string& func_name() const { return func_name_; }

		void other_info(const std::string& info) { other_info_ = info; }
		void other_info(std::string&& info) {other_info_= std::move(info); }
		const std::string& other_info() const { return other_info_; }

	private:
		short					error_code_;
		TErrorCategory*	        error_cate_;

		std::string   func_name_;
		std::string   other_info_;
	};

	// system wide exception
	class TException: public std::exception
	{
	public:
		explicit TException( const TError& error  ) : error_(error) {}

		explicit TException( TError&& error ) : error_(std::move(error)) {}

		TException( TException&& in ) : error_(std::move(in.error_)) {}

		virtual const char* what() const { return error_.category().Message(error_.code()).c_str(); } 
		const TError& error() const { return error_;}

	private:
		TError      error_;
	};

	//---------------------
	// utility functions
	//---------------------

	// format error string based on TError
	std::string ErrorString( const TError& er);

	// format third party error
	std::string FormatThirdPartyError(const char* lib_name, int ec, const char* msg);

	// make error 
	template<typename ErrorCode>
	TError MakeTError(ErrorCode ec, std::string func_name, std::string other_info)
	{
		TError te = MakeTError(ec);
		te.func_name(std::move(func_name));
		te.other_info(std::move(other_info));
		return te;
	}

	// throw texception 
	template<typename ErrorCode>
	void ThrowTException( ErrorCode ec, std::string func_name, std::string other_info )
	{
		throw TException(MakeTError(ec, std::move(func_name), std::move(other_info)));
	};

	// throw texception from error
	inline void ThrowTException( TError er )
	{
		throw TException(std::move(er));
	}

	// replace function name and throw
	inline void ThrowTException(TError er, std::string func_name)
	{
		er.func_name(std::move(func_name));
		throw TException(std::move(er));
	}
}

#endif // TSYSTEM_ERROR_H_