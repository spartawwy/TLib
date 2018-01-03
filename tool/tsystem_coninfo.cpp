#include "tsystem_coninfo.h"

#include "TLib/core/tsystem_sqlite_functions.h"

using namespace TSystem::contract;
using namespace TSystem;

ConInfo::ConInfo()
	: db_conn_(std::make_shared<SQLite::SQLiteConnection>())
	, products_()
	, contracts_()
{
	products_.reserve(64);
	contracts_.reserve(512);
}

ConInfo::ConInfo(const std::string& tag)
	: db_conn_(std::make_shared<SQLite::SQLiteConnection>())
	, products_()
	, contracts_()
{
	products_.reserve(64);
	contracts_.reserve(512);

	Open(tag);
}


bool ConInfo::Open(const std::string& tag, TError* te)
{
	std::string full_file_name = utility::ContractDir() + tag + ".condb";
	try
	{
		SQLite::SQLiteCode rc = db_conn_->Open(full_file_name.c_str());
		if( rc != SQLite::SQLiteCode::OK )
		{
			if( te )
				*te = MakeTError(CoreErrorCategory::ErrorCode::BAD_ARGUMENT
					, "ConInfo::Open"
					, FormatThirdPartyError("SQLite", static_cast<int>(rc), SQLite::SQLiteCodeStr(rc)));
			return false;
		}

		// open temp db for saving temp table
		SQLite::AttachDatabase("''", "temp_db", *db_conn_);

	}catch( const SQLite::SQLiteException& e)
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "ConInfo::Open"
			, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()));
	}

	LoadAllProducts();

	return true;
}

void ConInfo::Open(const std::string& tag )
{
	TError te;
	if( !Open(tag, &te) )
		throw TException(std::move(te));
}

void ConInfo::LoadAllProducts()
{
	try
	{
		if( !utility::ExistTable("Product", *db_conn_) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "ConInfo::LoadAllProducts"
				, "can't find table Prodcut");

		// load Exchange table
		std::string sql = "SELECT name, under, type, currency, exchange, exch_prod_sym, mkt_rule, delivery_type FROM Product";

		db_conn_->ExecuteSQL( sql.c_str(), [this](int cols, char** vals, char** names)->int
		{
			std::shared_ptr<ProductInfo> p = std::make_shared<ProductInfo>();
			p->name  = *vals;
			p->under = *(vals+1);
			p->contract_type = StringToContractType(*(vals+2));
			p->currency = *(vals+3);
			p->exchange = *(vals+4);
			p->exchange_prod_sym = *(vals+5);
			p->mkt_rule = *(vals+6);
			p->delivery_type = StringToDeliveryType(*(vals+7));

			products_.push_back(std::move(p));
			return 0;
		});

	}catch( const SQLite::SQLiteException& e )
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "ConInfo::LoadAllProducts"
			, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()));
	}
}

std::shared_ptr<ProductInfo> ConInfo::FindProduct(const std::string& prod_name) const
{
	auto iter = std::find_if(std::begin(products_), std::end(products_)
		, [&prod_name](const std::shared_ptr<ProductInfo>& p)->bool
	{ return p->name == prod_name; });

	return iter != products_.end() ? *iter : nullptr;
}

const ProductInfo& ConInfo::GetProduct(const std::string& prod_name) const
{
	std::shared_ptr<ProductInfo> p = FindProduct(prod_name);
	
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::GetExchange"
				, "can't find product: " + prod_name );

	return *p;
}

std::shared_ptr<Contract> ConInfo::FindContract(int contract_id, int longdate)
{
	auto iter = std::find_if(std::begin(contracts_), std::end(contracts_)
		, [contract_id](const std::shared_ptr<Contract>& p)->bool
	{ return p->contract_id == contract_id; });

	if( iter != std::end(contracts_) )
		return *iter;

	// call LoadContractHelper
	std::string conid_clause = utility::FormatStr("ContractIndex.conid = %d", contract_id);
	auto contract = LoadContractHelper(&conid_clause, longdate, ContractPredType());

	if( contract.size() )
	{
		std::shared_ptr<Contract> p_con(std::move(contract.begin()->second.begin()->second));
		contracts_.push_back(p_con);
		return p_con;
	}else
		return nullptr;
}

const Contract& ConInfo::GetContract(int contract_id, int longdate)
{
	std::shared_ptr<Contract> p = FindContract(contract_id, longdate);
	
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "LayInfo::GetExchange"
				, utility::FormatStr("can't find contract: %d", contract_id) );

	return *p;
}

ContractManager ConInfo::LoadContract(const std::set<int>& contract_ids, int longdate, ContractPredType&& pred)
{
	ContractManager mgt;

	if( !contract_ids.empty() )
	{
		// generate conid in clause
		std::string conid_clause;
		auto iter = std::begin(contract_ids);
		conid_clause = utility::FormatStr("ContractIndex.conid in ( %d", *iter++);
		while( iter != std::end(contract_ids) )
			conid_clause += utility::FormatStr(", %d", *iter++);
		conid_clause += ")";
		
		// call LoadContractHelper
		FillContractManager( LoadContractHelper(&conid_clause, longdate, std::move(pred)), mgt);
	}

	return mgt;
}

ContractManager ConInfo::LoadContract(Contract::ContractStatus status, int longdate, ContractPredType&& pred )
{
	ContractManager mgt;

	// generate conid in clause
	std::string status_clause = utility::FormatStr("status = '%s'"
		, ContractStatusToString(status).c_str());
		
	// call LoadContractHelper
	FillContractManager( LoadContractHelper(&status_clause, longdate, std::move(pred)), mgt);
	
	return mgt;
}

ContractManager ConInfo::LoadTickerContract( const std::string& ticker, Contract::ContractStatus status, int longdate, ContractPredType&& pred )
{
	ContractManager mgt;

	// generate conid in clause
	std::string ticker_clause = utility::FormatStr("ticker_name = '%s' AND status = '%s'"
		, ticker.c_str(), ContractStatusToString(status).c_str());
		
	// call LoadContractHelper
	FillContractManager( LoadContractHelper(&ticker_clause, longdate, std::move(pred)), mgt);
	
	return mgt;
}

ContractManager ConInfo::LoadProductContract( const std::string& product, Contract::ContractStatus status, int longdate, ContractPredType&& pred )
{
	ContractManager mgt;

	// generate conid in clause
	std::string product_clause = utility::FormatStr("product = '%s' AND status = '%s'"
		, product.c_str(), ContractStatusToString(status).c_str());
		
	// call LoadContractHelper
	FillContractManager( LoadContractHelper(&product_clause, longdate, std::move(pred)), mgt);
	
	return mgt;
}

void ConInfo::FillContractManager(ContractMapType&& contracts, ContractManager& mgt)
{
	if( contracts.size() )
	{
		std::for_each(std::begin(contracts), std::end(contracts), [&mgt](ContractMapType::reference& entry)
		{
			std::for_each(std::begin(entry.second), std::end(entry.second), [&mgt](std::pair<const int, std::unique_ptr<Contract>>& a)
			{ mgt.contract_holder_.push_back(std::move(a.second));});
		});
	}
	mgt.MapContracts();
}

ConInfo::ContractMapType ConInfo::LoadContractHelper(const std::string* where_clause, int longdate, ContractPredType&& pred) 
{
	ContractMapType all_contracts;
	try
	{
		// check database necessary table
		if( !utility::ExistTable("ContractIndex", *db_conn_) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "ConInfo::LoadContractHelper"
				, "can't find table ContractIndex");

		if( !utility::ExistTable("ContractTradeHour", *db_conn_) )
			ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
				, "ConInfo::LoadContractHelper"
				, "can't find table ContractTradeHour");
		
		// generate select clause
		std::string select_clause = "SELECT type, ContractIndex.conid as conid, symbol, status, ticker_name, trade_hour"
			" FROM ContractIndex LEFT JOIN ContractTradeHour ON ContractIndex.conid = ContractTradeHour.conid" 
			" WHERE longdate = 0 ";
		if( where_clause )
			select_clause += "AND " + *where_clause;

		// attach temp database and create temp table
		static int temp_count = 0;
		++temp_count;
		std::string full_temp_tb_name = utility::FormatStr("temp_db.temp_tb_%d", temp_count);

		// create temp table
		std::string sql = "CREATE TABLE " + full_temp_tb_name + " AS " + select_clause;

		db_conn_->ExecuteSQL(sql.c_str());

		// get all types
		sql = "SELECT DISTINCT type FROM " + full_temp_tb_name;
		db_conn_->ExecuteSQL(sql.c_str(), [&all_contracts](int cols, char** vals, char** names)->int
		{ 
			all_contracts.insert(std::make_pair(StringToContractType(*vals), ContractMapType::mapped_type()));
			return 0;
		});

		// get all special trade hour
		std::string special_td_clause = utility::FormatStr("SELECT conid, trade_hour "
			"FROM ContractTradeHour "
			"WHERE longdate = %d", longdate);
		std::map<int, std::string> special_trade_hour;
		db_conn_->ExecuteSQL(special_td_clause.c_str(), [&special_trade_hour](int cols, char** vals, char** names)->int
		{ 
			special_trade_hour.insert(std::make_pair(atoi(*vals), *(vals+1)));
			return 0;
		});

		// lambda to parse general info
		auto parse_general_info = [this, &special_trade_hour](char** vals)->std::unique_ptr<Contract>
		{
			std::unique_ptr<Contract> p_con(new Contract);
			p_con->contract_id = atoi(*vals);
			p_con->exchange_sym = *(vals+1);
			p_con->contract_status = StringToContrctStatus(*(vals+2));
			p_con->ticker_name = *(vals+3);
			p_con->trade_hour = *(vals+4);
			
			if( p_con->trade_hour.empty() )
				ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
					, "ConInfo::LoadContractHelper"
					, utility::FormatStr("no regular trade hour set for contract: %d", p_con->contract_id));

			auto trade_hour_iter = special_trade_hour.find(p_con->contract_id);
			if( trade_hour_iter != special_trade_hour.end() )
				p_con->trade_hour = trade_hour_iter->second;

			p_con->product_info_ = this->FindProduct(*(vals+5)); 

			if( !p_con->product_info_ )
				ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
					, "ConInfo::LoadContractHelper"
					, utility::FormatStr("can't find product info for contract: %d", p_con->contract_id));

			return p_con;
		};

		//-------------------------
		// load type specific info
		//-------------------------
		std::for_each(std::begin(all_contracts), std::end(all_contracts)
			, [this, &full_temp_tb_name, &parse_general_info, &pred](ContractMapType::value_type& type_contracts)
		{
			switch (type_contracts.first)
			{
			case ProductInfo::ContractType::SPOT:
				{
					if( !utility::ExistTable("spot", *this->db_conn_) )
						ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
							, "ConInfo::LoadContractHelper"
							, "can't find table spot");

					std::string sql = utility::FormatStr("SELECT spot.conid, symbol, status, ticker_name, trade_hour, "
						"spot.product "
						"FROM %s LEFT JOIN spot ON spot.conid = %s.conid "
						"WHERE %s.type = '%s'"
						, full_temp_tb_name.c_str(), full_temp_tb_name.c_str(), full_temp_tb_name.c_str()
						, ContractTypeToString(type_contracts.first).c_str());

					db_conn_->ExecuteSQL(sql.c_str(), [&type_contracts, &parse_general_info, &pred](int cols, char** vals, char** names)->int
					{ 
						std::unique_ptr<Contract> p_con = parse_general_info(vals);

						p_con->contract_def_ = std::make_shared<SpotType>();

						p_con->tag = p_con->product_info().name;

						if(!pred || pred(*p_con) )
							type_contracts.second.insert(std::make_pair(p_con->contract_id, std::move(p_con)));
						return 0;
					});
					break;
				}
			case ProductInfo::ContractType::FUTURE:
				{
					if( !utility::ExistTable("future", *db_conn_) )
						ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
							, "ConInfo::LoadContractHelper"
							, "can't find table future");

					std::string sql = utility::FormatStr("SELECT future.conid, symbol, status, ticker_name, trade_hour, "
						"future.product, future.expiry, future.multiplier "
						"FROM %s LEFT JOIN future ON future.conid = %s.conid "
						"WHERE %s.type = '%s'"
						, full_temp_tb_name.c_str(), full_temp_tb_name.c_str(), full_temp_tb_name.c_str()
						, ContractTypeToString(type_contracts.first).c_str());

					db_conn_->ExecuteSQL(sql.c_str(), [&type_contracts, &parse_general_info, &pred](int cols, char** vals, char** names)->int
					{ 
						std::unique_ptr<Contract> p_con = parse_general_info(vals);

						p_con->contract_def_ = std::make_shared<FutureType>();
						p_con->contract_def_->expiration = atoi(*(vals+6));
						p_con->contract_def_->multiplier = atof(*(vals+7));

						p_con->tag = utility::FormatStr("%s %d", p_con->product_info().name.c_str(), p_con->contract_def_->expiration);

						if(!pred || pred(*p_con) )
							type_contracts.second.insert(std::make_pair(p_con->contract_id, std::move(p_con)));
						return 0;
					});
					break;
				}
			case ProductInfo::ContractType::OPTION:
				{
					if( !utility::ExistTable("option", *db_conn_) )
						ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
							, "ConInfo::LoadContractHelper"
							, "can't find table option");

					std::string sql = utility::FormatStr("SELECT option.conid, symbol, status, ticker_name, trade_hour, "
						"option.product, option.style, option.expiry, option.multiplier, option.cp, option.strike "
						"FROM %s LEFT JOIN option ON option.conid = %s.conid "
						"WHERE %s.type = '%s'"
						, full_temp_tb_name.c_str(), full_temp_tb_name.c_str(), full_temp_tb_name.c_str()
						, ContractTypeToString(type_contracts.first).c_str());

					db_conn_->ExecuteSQL(sql.c_str(), [&type_contracts, &parse_general_info, &pred](int cols, char** vals, char** names)->int
					{ 
						std::unique_ptr<Contract> p_con = parse_general_info(vals);

						p_con->contract_def_ = std::make_shared<OptionType>();
						OptionType& def = *reinterpret_cast<OptionType*>(p_con->contract_def_.get());
						def.expire_style = StringToExpireStyle(*(vals+6));
						def.expiration = atoi(*(vals+7));
						def.multiplier = atof(*(vals+8));
						def.call_put = StringToCallPut(*(vals+9));
						def.strike   = atof(*(vals+10));

						p_con->tag = utility::FormatStr("%s %d %.2f %c%c"
							, p_con->product_info().name.c_str()
							, def.expiration
							, def.strike
							, (def.expire_style == contract::OptionType::ExpireStyle::AMERICAN ? 'A' : 'E')
							, (def.call_put == contract::OptionType::CallPut::CALL ? 'C' : 'P' ));

						if(!pred || pred(*p_con) )
							type_contracts.second.insert(std::make_pair(p_con->contract_id, std::move(p_con)));
						return 0;
					});
					break;
				}
			case ProductInfo::ContractType::BINARY_OPTION:
				{
					if( !utility::ExistTable("binary_option", *db_conn_) )
						ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
							, "ConInfo::LoadContractHelper"
							, "can't find table binary_option");

					std::string sql = utility::FormatStr("SELECT binary_option.conid, symbol, status, ticker_name, trade_hour, "
						"binary_option.product, binary_option.style, binary_option.expiry, binary_option.multiplier, binary_option.cp, binary_option.strike "
						"FROM %s LEFT JOIN binary_option ON binary_option.conid = %s.conid "
						"WHERE %s.type = '%s'"
						, full_temp_tb_name.c_str(), full_temp_tb_name.c_str(), full_temp_tb_name.c_str()
						, ContractTypeToString(type_contracts.first).c_str());

					db_conn_->ExecuteSQL(sql.c_str(), [&type_contracts, &parse_general_info, &pred](int cols, char** vals, char** names)->int
					{ 
						std::unique_ptr<Contract> p_con = parse_general_info(vals);

						p_con->contract_def_ = std::make_shared<BinaryOptionType>();
						BinaryOptionType& def = *reinterpret_cast<BinaryOptionType*>(p_con->contract_def_.get());
						def.expire_style = StringToExpireStyle(*(vals+6));
						def.expiration = atoi(*(vals+7));
						def.multiplier = atof(*(vals+8));
						def.call_put = StringToCallPut(*(vals+9));
						def.strike   = atof(*(vals+10));

						p_con->tag = utility::FormatStr("%s %d %.2f %c%c"
							, p_con->product_info().name.c_str()
							, def.expiration
							, def.strike
							, (def.expire_style == contract::OptionType::ExpireStyle::AMERICAN ? 'A' : 'E')
							, (def.call_put == contract::OptionType::CallPut::CALL ? 'C' : 'P' ));

						if(!pred || pred(*p_con) )
							type_contracts.second.insert(std::make_pair(p_con->contract_id, std::move(p_con)));
						return 0;
					});
					break;
				}
			case ProductInfo::ContractType::FUTURE_OPTION:
				{
					if( !utility::ExistTable("future_option", *db_conn_) )
						ThrowTException( CoreErrorCategory::ErrorCode::BAD_CONTENT
							, "ConInfo::LoadContractHelper"
							, "can't find table future_option");

					std::string sql = utility::FormatStr("SELECT future_option.conid, symbol, status, ticker_name, trade_hour, "
						"future_option.product, future_option.future_expiry, future_option.style, future_option.expiry, "
						"future_option.multiplier, future_option.cp, future_option.strike "
						"FROM %s LEFT JOIN future_option ON future_option.conid = %s.conid "
						"WHERE %s.type = '%s'"
						, full_temp_tb_name.c_str(), full_temp_tb_name.c_str(), full_temp_tb_name.c_str()
						, ContractTypeToString(type_contracts.first).c_str());

					db_conn_->ExecuteSQL(sql.c_str(), [&type_contracts, &parse_general_info, &pred](int cols, char** vals, char** names)->int
					{ 
						std::unique_ptr<Contract> p_con = parse_general_info(vals);
						
						p_con->contract_def_ = std::make_shared<FutureOptionType>();
						FutureOptionType& def = *reinterpret_cast<FutureOptionType*>(p_con->contract_def_.get());
						def.future_expiration = atoi(*(vals+6));
						def.expire_style = StringToExpireStyle(*(vals+7));
						def.expiration = atoi(*(vals+8));
						def.multiplier = atof(*(vals+9));
						def.call_put = StringToCallPut(*(vals+10));
						def.strike   = atof(*(vals+11));

						p_con->tag = utility::FormatStr("%s %d %d %.2f %c%c"
							, p_con->product_info().name.c_str()
							, def.future_expiration
							, def.expiration
							, def.strike
							, (def.expire_style == contract::OptionType::ExpireStyle::AMERICAN ? 'A' : 'E')
							, (def.call_put == contract::OptionType::CallPut::CALL ? 'C' : 'P' ));

						if(!pred || pred(*p_con) )
							type_contracts.second.insert(std::make_pair(p_con->contract_id, std::move(p_con)));
						return 0;
					});
					break;
				}
			}
		});

		// drop temp table
		std::string drop_table_clause = "DROP TABLE " + full_temp_tb_name;
		db_conn_->ExecuteSQL(drop_table_clause.c_str());

	}catch( const SQLite::SQLiteException& e )
	{
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "ConInfo::FindContract"
			, FormatThirdPartyError("SQLite", static_cast<int>(e.code()), e.what()));
	}

	return all_contracts;
}