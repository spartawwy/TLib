#ifndef TSYSTEM_ECP_CLIENT_SOCKET_H_
#define TSYSTEM_ECP_CLIENT_SOCKET_H_

#include <memory>
#include <string>

#include <TLib/tool/tsystem_rational_number.h>
#include "TLib/ep/tsystem_ecp_msg.pb.h"

namespace TSystem
{
namespace communication
{
	class Connection;
}
	class ServerClientAppBase;

	class ECPClientSocket
	{
	public:

		// constructor
		ECPClientSocket(ServerClientAppBase& app);

		// moving constructor
		ECPClientSocket(ECPClientSocket&& in);

		// destructor
		~ECPClientSocket();

		// user id
		void user_id(unsigned int user_id);

		// connection
		void Connection(const std::shared_ptr<communication::Connection>& pconn);
		const std::shared_ptr<communication::Connection>& Connection() const { return pconn_;}

		// send user request <LOGIN or LOGOUT>
		void SendUserRequest(unsigned int request_id, EcpUserRequest::Type type);

		// ------------------
		// submit/cancel order request
		// ------------------
		void SubmitSimpleOrder(unsigned int user_ref, int con_id
								, OpenClose oc, BuySell bs, unsigned int size, const RationalNumber& price
								, IOC ioc = IOC::NONE, int time_out = -1, const std::string* remark = nullptr);

		void SubmitTwoSideOrder(unsigned int user_ref, int con_id
								, OpenClose bid_oc, unsigned int bid_size, const RationalNumber& bid_prc
								, OpenClose ask_oc, unsigned int ask_size, const RationalNumber& ask_prc
								, int time_out = -1
								, const std::string* remark = nullptr
								, const std::string* quote_ref = nullptr);

		void CancelOrder(unsigned int ecp_oid);

		//-------------------------------------------------------------------------
		// the following Replace interfaces SHOULD NOT BE USED given no permission
		// since the Replaces are ECP defined composite operations, it is difficult to 
		// define operation status if atom operation is failed
		//-------------------------------------------------------------------------
		void ReplaceSimpleOrder(unsigned int ecp_oid, unsigned int user_ref, int con_id
								, OpenClose oc, BuySell bs, unsigned int size, const RationalNumber& price
								, IOC ioc = IOC::NONE, int time_out = -1, const std::string* remark = nullptr);

		void ReplaceTwoSideOrder(unsigned int ecp_oid, unsigned int user_ref, int con_id
								, OpenClose bid_oc, unsigned int bid_size, const RationalNumber& bid_prc
								, OpenClose ask_oc, unsigned int ask_size, const RationalNumber& ask_prc
								, int time_out = -1
								, const std::string* remark = nullptr
								, const std::string* quote_ref = nullptr);

	private:

		// ------------------
		// fill order request
		// ------------------
		void FillOrderHeader(OrderRequest& req, unsigned int user_ref, int con_id, unsigned int ecp_oid = 0
			, const std::string* remark = nullptr, const std::string* quote_ref = nullptr);

		void FillCancelOrderHeader(OrderRequest& req, unsigned int ecp_oid);

		void FillSimpleOrder(OrderRequest& req, OpenClose oc, BuySell bs, unsigned int size, const RationalNumber& price, IOC ioc);

		void FillTwoSideOrder(OrderRequest& req, OpenClose bid_oc, unsigned int bid_size, const RationalNumber& bid_prc
			                                          , OpenClose ask_oc, unsigned int ask_size, const RationalNumber& ask_prc);

	private:

		ServerClientAppBase&                       app_;
		unsigned int                               user_id_;
		std::shared_ptr<communication::Connection> pconn_;
	};
}

#endif // TSYSTEM_ECP_CLIENT_SOCKET_H_