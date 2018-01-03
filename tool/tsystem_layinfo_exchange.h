#ifndef TSYSTEM_LAYINFO_EXCHANGE_H_
#define TSYSTEM_LAYINFO_EXCHANGE_H_

#include <string>
#include <vector>

namespace TSystem
{
namespace layinfo
{
	class LayInfo;

	// saves exchange related layinfo, can only be 
	// loaded by class LayInfo
	class Exchange
	{
	public:

		Exchange();

		operator bool() const { return !name_.empty(); }

		// accessor
		const std::string& name() const { return name_; } 
		const std::string& time_zone() const { return time_zone_; }
		const std::string& city() const { return city_; }
		const std::vector<int>& AllExchCalDates() const { return exch_cal_dates_; }
		const std::vector<int>& AllExchBusDates() const { return exch_bus_dates_; }

		// find how many cal days between two dates, throw TException if bad date
		int DifCalDays( int date_1, int date_2 ) const;

		// find how many bus days between two dates, throw TException if bad date
		int DifBusDays( int date_1, int date_2 ) const;

		// find specific date before or after given
		// number of days for an initial date
		int NextBusDay( int date, int ndays ) const; // throw TException if bad date
		int NextCalDay( int date, int ndays ) const; // throw TException if bad date

		// check if date is bus day
		bool IsBusDay( int date ) const;

	private:

		// exchange info
		std::string name_;
		std::string time_zone_;
		std::string city_;

		// calendar
		std::vector<int> exch_cal_dates_;
		std::vector<int> exch_bus_dates_;

		friend class LayInfo;
	};
}
}

#endif 