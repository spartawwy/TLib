// by Xixin Silas Cheng

#ifndef TSYSTEM_ECP_MSG_H_
#define TSYSTEM_ECP_MSG_H_

#include "TLib/ep/tsystem_ecp_msg.pb.h"

namespace TSystem
{
	char BuySellChar( BuySell bs);
	BuySell CharBuySell(char bs);
	BuySell CharBuySell(const std::string& bs);

	char OpenCloseChar( OpenClose oc);
	OpenClose CharOpenClose(char oc);
	OpenClose CharOpenClose(const std::string& oc);

	// b = none ioc 
	// K = ioc fok 
	// k = ioc fak
	char IOCChar( IOC ioc );
	IOC CharIOC(char ioc);
	IOC CharIOC(const std::string& ioc);

	char OrderStatusChar( OrderHeader::OrderStatus status);

	// EXCHANGE : exchange trade, any trade from exchange public trade
	// COUNTER  : counter trade, any trade from non-exchange public trade
	// FAKE     : fake trade
	char SourceChar( Source source);

	// throw TException if convert fail
	Source CharSource(char source);
	Source CharSource(const std::string& source);

	// NORMAL   = 0 normal trade
	// INJECT   = 1 injection trade, things that we miss from normal trade
	// EXPIRE   = 2 expiration adjustment
	// ADJUST   = 3 any adjust
	// POSITION = 4 for fake trade only, this trade only count for position distortion
	// does not count for any other risk exposure
	// CASH     = 5 ajust to reflect cash change, for example: dividend, interest
	char ModeChar( Mode mode);

	// throw TException if convert fail
	Mode CharMode(char mode);
	Mode CharMode(const std::string& mode);

	// check if source and mode is allowed
	// only the follow source, mode is allowed
	// EXCHANGE: NORMAL, INJECT
	// COUNTER : NORMAL, INJECT, EXPIRE, ADJUST, CASH
	// FAKE    : ADJUST, POSITION
	bool CheckSourceMode(Source source, Mode mode);

    char CombUnCombChar(CombUnComb cu);
    CombUnComb CharCombUnComb(char cu);
    CombUnComb CharCombUnComb(const std::string& cu);
}

#endif // TSYSTEM_ECP_MSG_H_