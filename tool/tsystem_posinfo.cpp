#include "tsystem_posinfo.h"

#include "TLib/core/tsystem_sqlite_functions.h"
#include "TLib/ep/tsystem_ecp_msg.h"

using namespace TSystem;
using namespace TSystem::position;

PosInfo::PosInfo()
	: db_conn_(std::make_shared<SQLite::SQLiteConnection>())
	, exch_position_holder_()
	, fake_position_holder_()
{
	exch_position_holder_.reserve(16);
	fake_position_holder_.reserve(16);
}

PosInfo::PosInfo(const std::string& tag)
	: db_conn_(std::make_shared<SQLite::SQLiteConnection>())
	, exch_position_holder_()
	, fake_position_holder_()
{
	exch_position_holder_.reserve(16);
	fake_position_holder_.reserve(16);

	Open(tag);
}

bool PosInfo::Open(const std::string& tag, TError* te)
{
	std::string full_file_name = utility::PositionDir() + tag + ".posdb";
	try
	{
		SQLite::SQLiteCode rc = db_conn_->Open(full_file_name.c_str(), SQLite::SQLiteConnection::OpenMode::READ);
		if( rc != SQLite::SQLiteCode::OK )
		{
			if( te )
				*te = MakeTError(CoreErrorCategory::ErrorCode::BAD_ARGUMENT
					, "PosInfo::Open"
					, FormatThirdPartyError("SQLite", static_cast<int>(rc), SQLite::SQLiteCodeStr(rc)));
			return false;
		}

		// check table
		if( !utility::ExistTable("FakePosition", *db_conn_) || !utility::ExistTable("ClearingPosition", *db_conn_)
			|| !utility::ExistTable("OrderFill", *db_conn_) )
			return false;

	}catch( const SQLite::SQLiteException& e)
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "PosInfo::Open"
			, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()));
	}

	return true;
}

void PosInfo::Open(const std::string& tag)
{
	TError te;
	if( !Open(tag, &te) )
		throw TException(std::move(te));
}

std::shared_ptr<ExchangePosition> PosInfo::FindExchangePosition(int user_id, int conid, int longdate)
{
	auto iter = std::find_if(std::begin(exch_position_holder_), std::end(exch_position_holder_),
		[conid, user_id, longdate](const std::shared_ptr<const ExchangePosition>& p)->bool
	{ return p->conid() == conid && p->user_id() == user_id && p->longdate() == longdate; });

	if( iter != std::end(exch_position_holder_) )
		return *iter;

	// if can't find, load from database
	ExchangePositionManager mgt;
	mgt.exchange_position_holder_.emplace_back(new ExchangePosition(conid, user_id, longdate));
	LoadExchangePositionHelper(mgt, user_id, longdate);
	if( mgt.AllExchangePositions().size() )
	{
		std::shared_ptr<ExchangePosition> p = std::move(mgt.AllExchangePositions().front());
		exch_position_holder_.push_back(p);
		return p;
	}

	return nullptr;
}

const ExchangePosition& PosInfo::GetExchangePosition(int user_id, int conid, int longdate)
{
	std::shared_ptr<ExchangePosition> p = FindExchangePosition(user_id, conid, longdate);
	if( !p )
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT
					, "PosInfo::GetExchangePosition"
					, utility::FormatStr("can't find exchange position: user_id %d, conid %d, longdate %d"
										, user_id, conid, longdate));
	return *p;
}

std::shared_ptr<FakePosition> PosInfo::FindFakePosition(int user_id, int conid, int longdate)
{
	auto iter = std::find_if( std::begin(fake_position_holder_), std::end(fake_position_holder_)
		, [user_id, conid, longdate](const std::shared_ptr<FakePosition>& p)->bool
	{ return p->conid == conid && p->longdate == longdate && p->user_id == user_id; });

	if( iter != std::end(fake_position_holder_) )
		return *iter;

	// if can't find, return nullptr
	FakePositionManager mgt;
	mgt.fake_position_holder_.emplace_back(new FakePosition(conid, user_id, longdate));
	LoadFakePositionHelper(mgt ,user_id, longdate);
	if( mgt.AllFakePositions().size() )
	{
		std::shared_ptr<FakePosition> p = std::move(mgt.AllFakePositions().front());
		fake_position_holder_.push_back(p);
		return p;
	}

	return nullptr;
}

const FakePosition& PosInfo::GetFakePosition(int user_id, int conid, int longdate)
{
	std::shared_ptr<FakePosition> p = FindFakePosition(user_id, conid, longdate);
	if( !p )
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT
				, "PosInfo::GetFakePosition"
				, utility::FormatStr("cant' find fake position: user_id %d, conid %d, longdate %d"
									, user_id, conid, longdate));
	return *p;
}

ExchangePositionManager PosInfo::LoadExchangePosition(int user_id, std::set<int> conids, int longdate)
{
	ExchangePositionManager mgt;

	if( !conids.empty() )
	{
		mgt.InitExchangePosition(user_id, longdate, std::begin(conids), std::end(conids), [](int conid){return conid;} );
		LoadExchangePositionHelper(mgt, user_id, longdate);
	}

	return mgt;
}

FakePositionManager PosInfo::LoadFakePosition(int user_id, std::set<int> conids, int longdate)
{
	FakePositionManager mgt;

	if( !conids.empty() )
	{
		mgt.InitExchangePosition(user_id, longdate, std::begin(conids), std::end(conids), [](int conid){return conid;});
		LoadFakePositionHelper(mgt, user_id, longdate);
	}

	return mgt;
}

ExchangePositionManager& PosInfo::LoadExchangePositionHelper(ExchangePositionManager& mgt, int user_id, int longdate)
{
	try
	{
		// lambda to convert char to OpenClose
		static auto convert_open_close = [](const std::string& open_close)->OpenClose
		{
			if( open_close == "O" )
				return OpenClose::OPEN;
			else if( open_close == "C" )
				return OpenClose::CLOSE;
			else if( open_close == "N" )
				return OpenClose::CLOSE_OVERNIGHT;
			else if( open_close == "I" )
				return OpenClose::CLOSE_INTRDAY;
			assert(0);
            return OpenClose::OPEN;
		};

		// lambda to convert char to BuySell
		static auto convert_buy_sell = [](const std::string& buy_sell)->BuySell
		{
			if( buy_sell == "B" )
				return BuySell::BUY;
			else if( buy_sell == "S" )
				return BuySell::SELL;
			assert(0);
            return BuySell::BUY;
		};

		// cat conid
		std::string conids;
		std::for_each(std::begin(mgt.exchange_position_holder_), std::end(mgt.exchange_position_holder_)
			, [&conids](const std::unique_ptr<ExchangePosition>& exch_pos)
		{
			if( conids.empty() )
				conids += utility::FormatStr("%d", exch_pos->conid());
			else
				conids += utility::FormatStr(", %d", exch_pos->conid());
		});
		std::string conid_in_clause = " AND conid in (" + conids + ")";


		// load open position
		std::string sql = utility::FormatStr("SELECT conid, overnight_long, overnight_short, intraday_long, intraday_short "
														"FROM ClearingPosition WHERE open_close='OPEN' "
														"AND longdate=%d AND user_id=%d ", longdate, user_id);
		sql += conid_in_clause;
		db_conn_->ExecuteSQL(sql.c_str(), [&mgt](int cols, char** vals, char** names)->int	
		{
			int conid			= atoi(*vals);
		
			// find conid
			auto iter = std::find_if(std::begin(mgt.exchange_position_holder_), std::end(mgt.exchange_position_holder_)
				, [conid](const std::unique_ptr<ExchangePosition>& p)->bool
			{ return p->conid() == conid; });
			assert(iter != mgt.exchange_position_holder_.end());

			int overnight_long	= atoi(*(vals+1));
			int overnight_short = atoi(*(vals+2));
			int intraday_long	= atoi(*(vals+3));
			int intraday_short	= atoi(*(vals+4));

			(*iter)->opening_position().overnight_long = overnight_long;
			(*iter)->opening_position().overnight_short = overnight_short;
			(*iter)->opening_position().intraday_long = intraday_long;
			(*iter)->opening_position().intraday_short = intraday_short;

			(*iter)->current_position().overnight_long = (*iter)->opening_position().overnight_long;
			(*iter)->current_position().overnight_short = (*iter)->opening_position().overnight_short;
			(*iter)->current_position().intraday_long = (*iter)->opening_position().intraday_long;
			(*iter)->current_position().intraday_short = (*iter)->opening_position().intraday_short;

			return 0;
		});

		// aggregate fill
		std::string fill_sql = utility::FormatStr("SELECT conid, open_close, buy_sell, quantity FROM OrderFill WHERE source != 'F' "
											"AND longdate =%d AND user_id =%d ", longdate, user_id);
		fill_sql += conid_in_clause;
		fill_sql += " ORDER BY conid, exec_id";					
		db_conn_->ExecuteSQL(fill_sql.c_str(), [&mgt](int cols, char** vals, char** names)->int
		{
			int conid = atoi(*vals);

			auto iter = std::find_if(std::begin(mgt.exchange_position_holder_), std::end(mgt.exchange_position_holder_)
				, [conid](const std::unique_ptr<ExchangePosition>& p)->bool
			{ return p->conid() == conid; });
			assert( iter != mgt.exchange_position_holder_.end() );

			std::string open_close = *(vals+1);
			std::string buy_sell = *(vals+2);
			int quantity = atoi(*(vals+3));

			(*iter)->UpdateExchangePosition( convert_open_close(open_close), convert_buy_sell(buy_sell), quantity );

			return 0;
		});
	}catch(const SQLite::SQLiteException& e)
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT
						, "PosInfo::LoadExchangePositionHelper"
						, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()));
	}

	mgt.MapExchangePositions();
	return mgt;
}

FakePositionManager& PosInfo::LoadFakePositionHelper(FakePositionManager& mgt, int user_id, int longdate)
{
	try
	{
		// cat conid
		std::string conids;
		std::for_each(std::begin(mgt.fake_position_holder_), std::end(mgt.fake_position_holder_)
			, [&conids](const std::unique_ptr<FakePosition>& fake_pos)
		{
			if( conids.empty() )
				conids += utility::FormatStr("%d", fake_pos->conid);
			else
				conids += utility::FormatStr(", %d", fake_pos->conid);
		});
		std::string conid_in_clause = " AND conid in (" + conids + ")";


		// load fake position
		std::string sql = utility::FormatStr("SELECT conid, mode, position FROM FakePosition "
												"WHERE longdate=%d AND user_id=%d ", longdate, user_id);
		sql += conid_in_clause;
		db_conn_->ExecuteSQL(sql.c_str(), [&mgt](int cols, char** vals, char** names)->int	
		{
			int conid			= atoi(*vals);
			auto iter = std::find_if(std::begin(mgt.fake_position_holder_), std::end(mgt.fake_position_holder_)
				, [conid](const std::unique_ptr<FakePosition>& p)->bool
			{ return p->conid == conid; });
			assert(iter != mgt.fake_position_holder_.end());

			(*iter)->UpdateFakePosition(atoi(*(vals+2)), CharMode(*(vals+1)));
			return 0;
		});

		// aggregate fill
		std::string fill_sql = utility::FormatStr("SELECT conid, mode, quantity FROM OrderFill WHERE source = 'F' "
													"AND longdate=%d AND user_id=%d ", longdate, user_id);
		fill_sql += conid_in_clause;
		fill_sql += " ORDER BY conid, exec_id";
		db_conn_->ExecuteSQL(fill_sql.c_str(), [&mgt](int cols, char** vals, char** names)->int
		{
			int conid = atoi(*vals);

			auto iter = std::find_if(std::begin(mgt.fake_position_holder_), std::end(mgt.fake_position_holder_)
				, [conid](const std::unique_ptr<FakePosition>& p)->bool
			{ return p->conid == conid; });
			assert(iter != mgt.fake_position_holder_.end());

			(*iter)->UpdateFakePosition(atoi(*(vals+2)), CharMode(*(vals+1)));
			return 0;
		});
	}catch(const SQLite::SQLiteException& e)
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT
						, "PosInfo::LoadFakePositionHelper"
						, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()));
	}


	mgt.MapFakePositions();
	return mgt;
}