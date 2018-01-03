#ifndef TSYSTEM_LAYINFO_POSITION_H_
#define TSYSTEM_LAYINFO_POSITION_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "TLib/core/tsystem_core_paths.h"
#include "TLib/ep/tsystem_ecp_msg.pb.h"

namespace TSystem
{

namespace utility
{
	inline std::string PositionDir()
	{
		return DatabaseDir() + "position//";
	}
}

namespace layinfo
{
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

	// struct to load exchange position information
	class ExchangePosition
	{
	public:

		explicit ExchangePosition(int conid);

		operator bool() const;

		// update exchange position, throw TException when qty does not match
		void UpdateExchangePosition(OpenClose oc, BuySell bs, unsigned int qty);

		const PositionDetail& current_position() const { return current_position_; }
		const PositionDetail& opening_position() const { return opening_position_; }

		int   conid() const { return conid_; }

	private:

		int conid_;
		
		PositionDetail current_position_;
		PositionDetail opening_position_;

		friend class UserExchangePosition;
	};

	class UserExchangePosition
	{
	public:

		UserExchangePosition();

		// init exchange positions
		template<typename IteratorType, typename ParserType>
		void InitExchangePosition(IteratorType first, IteratorType last, ParserType&& parser)
		{
			while( first != last )
				exchange_position_holder_.emplace_back(parser(*(first++)));

			// create unordered_map mapping
			std::for_each(std::begin(exchange_position_holder_), std::end(exchange_position_holder_)
			, [this](ExchangePosition& position)
			{
				if( !id_maps_.insert(std::make_pair(position.conid(), &position)).second )
				{
					TSystem::ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT, 
										"UserExchangePosition::InitExchangePosition",
										utility::FormatStr("duplicated conid: %d", position.conid())
										);
				}
			});
		}

		// load exchange position from pos condb
	    bool LoadExchangePosition(int user_id, int longdate, const std::string& dbname);

		// query by conid O(1)
		const ExchangePosition& FindPosition(int conid) const;
		ExchangePosition&       FindPosition(int conid);

		// all exchange position
		const std::vector<ExchangePosition>& AllExchangePosition() const { return exchange_position_holder_; }

	private:

		std::unordered_map<int, ExchangePosition*>  id_maps_;

		// exchange position holder
		std::vector<ExchangePosition>	exchange_position_holder_;

		// the default bad exchange position
		static ExchangePosition		bad_exchange_position;
	};

	// load dev_tag() laydb, throw TException
	void LoadAppExchangePosition(UserExchangePosition& mgt, int longdate, int user_id = 0 );
}
}
#endif // TSYSTEM_LAYINFO_POSITION_H_