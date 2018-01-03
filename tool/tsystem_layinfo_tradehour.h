#ifndef TSYSTEM_LAYINFO_TRADEHOUR_H_
#define TSYSTEM_LAYINFO_TRADEHOUR_H_

#include <chrono>
#include <memory>
#include <vector>


namespace TSystem
{
namespace layinfo
{

	class Exchange;

	struct IntradayTimePoint
	{
		IntradayTimePoint();
		IntradayTimePoint(IntradayTimePoint&& in);

		int										day_offset;
		std::chrono::system_clock::duration		time_of_day;
		std::chrono::system_clock::time_point	time_with_day;
	};

	IntradayTimePoint MakeIntradayTimePoint(int day_offset, int hour, int min, int sec);

	struct TradeSession
	{
		enum class OpenCloseType: short
		{
			REGULAR_OPN = 0,
			REGULAR_CLS,
			BREAK_OPN,
			BREAK_CLS
		};

		TradeSession();
		TradeSession(TradeSession&& in);

		IntradayTimePoint	open_time;
		OpenCloseType		open_type;

		IntradayTimePoint	close_time;
		OpenCloseType		close_type;
	};

	const std::string&          OpenCloseTypeToString( TradeSession::OpenCloseType t ); // throw TException if unkown type
	TradeSession::OpenCloseType StringToOpenCloseType( const std::string& s ); // throw TException if unkown string of type

	class LayInfo;

	class TradeHour
	{
	public:

		// throw TException when exchange is nullptr
		explicit TradeHour(const std::shared_ptr<Exchange>& exchange);
		
		const std::string& name() const { return name_; }
		const Exchange&	exchange() const { return *exchange_; }

		// check if trading hour
		bool IsTradingHour(const std::chrono::system_clock::time_point& time) const;

		// get clearing date
		int	ClearingDate(const std::chrono::system_clock::time_point& time) const;

	private:

		// initiate trade_sessions_ given longdate
		void InitiateSessions(int longdate);

		std::string					name_;
		std::shared_ptr<Exchange>	exchange_;

		int							trading_date_;
		std::vector<TradeSession>   trade_sessions_;

		friend class LayInfo;
	};
}
}

#endif // TSYSTEM_LAYINFO_TRADEHOUR_H_