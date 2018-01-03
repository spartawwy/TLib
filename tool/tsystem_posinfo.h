#ifndef TSYSTEM_POSINFO_H_
#define TSYSTEM_POSINFO_H_

#include <set>

#include <SQLite/SQLite.h>

#include "TLib/core/tsystem_core_paths.h"
#include "TLib/core/tsystem_time.h"

#include "tsystem_exchange_position.h"
#include "tsystem_fake_position.h"


namespace TSystem
{

namespace utility
{
	inline std::string PositionDir()
	{
		return DatabaseDir() + "position//";
	}
}

namespace position
{

	class PosInfo
	{
	public:

		PosInfo();
		explicit PosInfo(const std::string& tag);

		bool Open(const std::string& tag, TError* te);
		void Open(const std::string& tag); // version to throw TException

		// find/get position
		std::shared_ptr<ExchangePosition>	FindExchangePosition(int user_id, int conid, int longdate = Today()); // throw TException when fail
		const ExchangePosition&				GetExchangePosition(int user_id, int conid, int longdate = Today()); // throw TException when not found

		std::shared_ptr<FakePosition>		FindFakePosition(int user_id, int conid, int longdate = Today()); // throw TException when fail
		const FakePosition&					GetFakePosition(int user_id, int conid, int longdate = Today()); // throw TException when not found

		//-------------------------------
		// load position objects
		//-------------------------------

		// load exchange position with given user id and conids, throw TException when fail
		ExchangePositionManager LoadExchangePosition(int user_id, std::set<int> conids, int longdate = Today());

		// load fake position with given user id and conids, throw TException when fail
		FakePositionManager LoadFakePosition(int user_id, std::set<int> conids, int longdate = Today()); 

		
		// helper function, assume that position holder in manangement have been initiated by InitXXPosition
		// therefore no need to add conid condition to where clause, throw TException
		ExchangePositionManager& LoadExchangePositionHelper(ExchangePositionManager& mgt, int user_id, int longdate);
		FakePositionManager&     LoadFakePositionHelper(FakePositionManager& mgt, int user_id, int longdate);
		
	private:

		// sqlite database connection
		std::shared_ptr<SQLite::SQLiteConnection>	db_conn_;

		// lvector of positions
		std::vector<std::shared_ptr<ExchangePosition>>		exch_position_holder_;
		std::vector<std::shared_ptr<FakePosition>>			fake_position_holder_;
	};
}
}

#endif // TSYSTEM_POSINFO_H_