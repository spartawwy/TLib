#include "tsystem_layinfo_position.h"

#include <SQLite/sqlite_connection.h>

#include "TLib/core/tsystem_core_common.h"
#include "TLib/core/tsystem_time.h"
#include "TLib/core/tsystem_sqlite_functions.h"

using namespace SQLite;
using namespace TSystem;
using namespace TSystem::layinfo;

ExchangePosition::ExchangePosition(int id)
	: conid_(id)
	, current_position_()
	, opening_position_()
{}

ExchangePosition::operator bool() const
{
	return conid_ != 0;
}

void ExchangePosition::UpdateExchangePosition(OpenClose oc, BuySell bs, unsigned int qty)
{
	// lambda to check if enought position to close
	static auto update_close_position = []( unsigned int& a, unsigned int& b, unsigned int& c, unsigned int qty)
	{
		if( a >= qty )
		{
			a -= qty;
		}else
		{
			qty -= a; a = 0; 
			if( b >= qty )
				b -= qty;
			else
			{
				qty -= b; b = 0;
				c += qty;
			}
		}
	};

	static auto update_close_intraday = []( unsigned int& a, unsigned int& b, unsigned int qty)
	{
		if( a >= qty )
		{ a -= qty; }
		else
		{ qty -= a; a = 0; b+=qty;}
	};

	switch (oc)
	{
	case OPEN:
		{
			switch (bs)
			{
			case BUY:
				current_position_.intraday_long += qty;
				break;
			case SELL:
				current_position_.intraday_short += qty;
				break;
			}
			break;
		}
	case CLOSE:
		{
			switch (bs)
			{
			case BUY:
				update_close_position( current_position_.overnight_short, current_position_.intraday_short, current_position_.intraday_long, qty);
				break;
			case SELL:
				update_close_position( current_position_.overnight_long, current_position_.intraday_long, current_position_.intraday_short, qty);
				break;
			}
			break;
		}
	case CLOSE_OVERNIGHT:
		{
			switch (bs)
			{
			case BUY:
				update_close_position( current_position_.overnight_short, current_position_.intraday_short, current_position_.intraday_long, qty);
				break;
			case SELL:
				update_close_position( current_position_.overnight_long, current_position_.intraday_long, current_position_.intraday_short, qty);
				break;
			}
			break;
		}
	case CLOSE_INTRDAY:
		{
			switch (bs)
			{
			case BUY:
				update_close_intraday( current_position_.intraday_short, current_position_.intraday_long, qty);
				break;
			case SELL:
				update_close_intraday( current_position_.intraday_long, current_position_.intraday_short, qty);
				break;
			}
			break;
		}
	default:
		break;
	}
}

//-------------------------
// UserExchangePosition
//-------------------------
ExchangePosition UserExchangePosition::bad_exchange_position = ExchangePosition(0);

UserExchangePosition::UserExchangePosition()
	: id_maps_(1024)
	, exchange_position_holder_()
{
	exchange_position_holder_.reserve(1024);
}


bool UserExchangePosition::LoadExchangePosition(int user_id, int longdate, const std::string& dbname )
{
	// open database
	std::string database_file = utility::PositionDir() + dbname + ".posdb";
	std::shared_ptr<SQLiteConnection> posdb = std::make_shared<SQLiteConnection>();
	if( posdb->Open(database_file.c_str(), SQLiteConnection::OpenMode::READ) != SQLiteCode::OK )
		return false;

	// database operation
	try
	{
		if( !utility::ExistTable("OrderFill", *posdb) || !utility::ExistTable("ClearingPosition", *posdb) )
			return false;

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
		};

		// lambda to convert char to BuySell
		static auto convert_buy_sell = [](const std::string& buy_sell)->BuySell
		{
			if( buy_sell == "B" )
				return BuySell::BUY;
			else if( buy_sell == "S" )
				return BuySell::SELL;
			assert(0);
		};

		// cat conid
		std::string conids;
		std::for_each(std::begin(exchange_position_holder_), std::end(exchange_position_holder_)
			, [&conids](const ExchangePosition& exch_pos)
		{
			if( conids.empty() )
				conids += utility::FormatStr("%d", exch_pos.conid());
			else
				conids += utility::FormatStr(", %d", exch_pos.conid());
		});
		std::string conid_in_clause = " AND conid in (" + conids + ")";

		// load longdate open position
		std::string open_pos_sql = utility::FormatStr("SELECT conid, overnight_long, overnight_short, intraday_long, intraday_short "
														"FROM ClearingPosition "
														"WHERE longdate=%d AND user_id=%d AND open_close='OPEN'"
														, longdate, user_id);
		open_pos_sql += conid_in_clause;

		posdb->ExecuteSQL(open_pos_sql.c_str(), [this](int cols, char** vals, char** names)->int
		{
			int conid = atoi(*vals);
			auto iter = std::find_if(std::begin(exchange_position_holder_), std::end(exchange_position_holder_)
				, [conid](const ExchangePosition& pos)->bool { return pos.conid() == conid; });
			assert( iter != std::end(exchange_position_holder_));

			iter->opening_position_.overnight_long  = atoi(*(vals+1));
			iter->opening_position_.overnight_short = atoi(*(vals+2));
			iter->opening_position_.intraday_long   = atoi(*(vals+3));
			iter->opening_position_.intraday_short  = atoi(*(vals+4));

			iter->current_position_.overnight_long  =	iter->opening_position_.overnight_long;
			iter->current_position_.overnight_short =	iter->opening_position_.overnight_short;
			iter->current_position_.intraday_long   =	iter->opening_position_.intraday_long;
			iter->current_position_.intraday_short  =	iter->opening_position_.intraday_short;
			return 0;
		});

		// aggregate fill
		std::string fill_sql = utility::FormatStr("SELECT conid, open_close, buy_sell, quantity "
											"FROM OrderFill "
											"WHERE longdate =%d AND user_id =%d AND source != 'F' "
											, longdate, user_id);
		fill_sql += conid_in_clause;
		fill_sql += " ORDER BY conid, exec_id";
		
		ExchangePosition* p_pos(nullptr);
		posdb->ExecuteSQL(fill_sql.c_str(), [&p_pos, this](int cols, char** vals, char** names)->int
		{
			int conid = atoi(*vals);
			if( !p_pos || p_pos->conid() != conid )
			{
				auto iter = std::find_if(std::begin(exchange_position_holder_), std::end(exchange_position_holder_)
				, [conid](const ExchangePosition& pos)->bool { return pos.conid() == conid; });
				assert( iter != std::end(exchange_position_holder_));

				p_pos = std::addressof(*iter);
			}

			std::string open_close  = *(vals+1);
			std::string buy_sell    = *(vals+2);
			int quantity            = atoi(*(vals+3));
			p_pos->UpdateExchangePosition( convert_open_close(open_close), convert_buy_sell(buy_sell), quantity );
			return 0;
		});
	}
	catch( const SQLiteException& e )
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_CONTENT,
							"UserExchangePosition::LoadExchangePosition",
							FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what())
							);
	}
	posdb->Close();
	return true;
}

const ExchangePosition& UserExchangePosition::FindPosition(int conid) const
{
	auto iter = id_maps_.find(conid);
	return iter != id_maps_.end() ? *iter->second : bad_exchange_position;
}

ExchangePosition& UserExchangePosition::FindPosition(int conid)
{
	auto iter = id_maps_.find(conid);
	return iter != id_maps_.end() ? *iter->second : bad_exchange_position;
}

void TSystem::layinfo::LoadAppExchangePosition(UserExchangePosition& mgt, int longdate, int user_id)
{
	if( ! mgt.LoadExchangePosition(user_id, longdate, utility::DevTag()) )
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT, 
							"LoadAppExchangePosition",
							"Can't find posdb or ClearingPosition or OrderFill table: " + utility::DevTag()
							);
	}
}