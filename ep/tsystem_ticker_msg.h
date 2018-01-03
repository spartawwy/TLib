// by Xixin Silas Cheng

#ifndef TSYSTEM_TICKER_MSG_H_
#define TSYSTEM_TICKER_MSG_H_

#include "tsystem_ticker_msg.pb.h"

namespace TSystem
{
	void FillQuoteLevel(double bid_prc, unsigned int bid_sz, double ask_prc, unsigned int ask_sz, BookData::BidAskLevel& level);

	char QuoteStatusChar( QuoteRequestData::QuoteStatus status);
}

#endif // TSYSTEM_TICKER_MSG_H_