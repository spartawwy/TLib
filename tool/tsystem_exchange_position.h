#ifndef TSYSTEM_EXCHANGE_POSITION_H_
#define TSYSTEM_EXCHANGE_POSITION_H_

#include <memory>
#include <unordered_map>

#include "TLib/core/tsystem_core_common.h"
#include "TLib/ep/tsystem_ecp_msg.pb.h"

namespace TSystem
{
namespace position
{
	class PosInfo;

	struct PositionDetail
	{
		PositionDetail()
		{
			overnight_long = overnight_short = intraday_long = intraday_short = 0;
		}

		int NetPosition() const
		{
			return static_cast<int>(overnight_long)+intraday_long-overnight_short-intraday_short;
		};

		unsigned int ShortPosition() const 
		{
			return overnight_short+intraday_short;
		}

		unsigned int LongPosition() const 
		{
			return overnight_long+intraday_long;
		}

		unsigned int overnight_long;
		unsigned int overnight_short;
		unsigned int intraday_long;
		unsigned int intraday_short;
	};

	// class to store exchange position information
	class ExchangePosition
	{
	public:

		explicit ExchangePosition(int conid, int user_id, int longdate)
			: conid_(conid), user_id_(user_id), longdate_(longdate)
			, current_position_(), opening_position_()
		{}

		operator bool() const;

		// update exchange position, throw TException when qty does not match
		void UpdateExchangePosition(OpenClose oc, BuySell bs, unsigned int qty);

		const PositionDetail& current_position() const { return current_position_; }
		const PositionDetail& opening_position() const { return opening_position_; }
		PositionDetail& current_position() { return current_position_; }
		PositionDetail& opening_position() { return opening_position_; }


		int	conid()		const	{ return conid_; }
		int	user_id()	const	{ return user_id_; }
		int	longdate()	const	{ return longdate_; }	

	private:

		int conid_;
		int user_id_;
		int longdate_;
		
		PositionDetail current_position_;
		PositionDetail opening_position_;
	};
	
	//--------------------------------
	// manager of a group of  
	//--------------------------------
	class ExchangePositionManager
	{
	public:
		// constructor
		explicit ExchangePositionManager();
		ExchangePositionManager(ExchangePositionManager&& in);
		
		// operators
		ExchangePositionManager& operator=(ExchangePositionManager&& in);

		// find position, query by conid O(1)
		const ExchangePosition* FindPosition(int conid) const;
		ExchangePosition* FindPosition(int conid);

		// init position holder
		template<typename IteratorType, typename ParserType>
		void InitExchangePosition(int user_id, int longdate, IteratorType first, IteratorType last, ParserType&& parser )
		{
			while( first != last )
				exchange_position_holder_.emplace_back(new ExchangePosition(parser(*first++), user_id, longdate));
		}

		const std::vector<std::unique_ptr<ExchangePosition>>& AllExchangePositions() const { return exchange_position_holder_; }
		std::vector<std::unique_ptr<ExchangePosition>>& AllExchangePositions() { return exchange_position_holder_; }

	private:

		void MapExchangePositions();

		std::unordered_map<int, ExchangePosition*>		id_map_;
		std::vector<std::unique_ptr<ExchangePosition>>	exchange_position_holder_;

		friend class PosInfo;
	};

	// load dev_tag() laydb, throw TException
	void LoadAppExchangePosition(PosInfo& pos_info, ExchangePositionManager& mgt, int longdate, int user_id = 0 );
}
}

#endif // TSYSTEM_EXCHANGE_POSITION_H_