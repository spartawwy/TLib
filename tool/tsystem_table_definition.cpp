#include "tsystem_table_definition.h"
#include <boost/algorithm/string/join.hpp>

using namespace TSystem::utility;

DBTableDef::DBTableDef(const std::string& name)
	: name_(name)
	, fields_()
	, key_()
{}

void DBTableDef::AddField(const std::string& name, DBDataType type)
{
	fields_.emplace_back(name, type);
}

void DBTableDef::AddKeyField(const std::string& name)
{
	key_.emplace_back(name);
}

std::string TSystem::utility::CreateTableSQL(const DBTableDef& table)
{
	std::string sql;
	sql.append("CREATE TABLE IF NOT EXISTS [").append(table.name()).append("]");

	// generate table column defination
	std::string cols;
	for( const auto& col : table.fields())
	{
		if( !cols.empty() )
			cols.append(", ");
		cols.append(col.first);
		switch (col.second)
		{
		case DBTableDef::DBDataType::Integer:
			cols.append(" INTEGER");
			break;
		case DBTableDef::DBDataType::Text:
			cols.append(" TEXT");
			break;
		case DBTableDef::DBDataType::Double:
			cols.append(" DOUBLE");
			break;
		default:
			break;
		}
	}

	sql.append("(").append(cols);

	if( table.key().size() > 0 )
		sql.append(", PRIMARY KEY (").append(boost::algorithm::join(table.key(), ", ")).append(")");

	sql.append(")");
	return sql;
}

std::string TSystem::utility::DropTableSQL(const DBTableDef& table)
{
	std::string sql("DROP TABLE IF EXISTS ");
	sql.append(table.name());
	return sql;
}

std::string TSystem::utility::SelectAllSQL(const DBTableDef& table)
{
	std::string cols;

	for( const auto& col : table.fields())
	{
		if( !cols.empty() )
			cols.append(", ");
		cols.append(col.first);
	}

	std::string sql;
	sql.append("SELECT ").append(cols).append(" FROM ").append(table.name());
	return sql;
}

std::string TSystem::utility::InsertOrAbortSQL(const DBTableDef& table, const std::vector<std::string>& data)
{
	std::string vals;
	for( size_t i = 0; i < data.size(); ++i)
	{
		if( !vals.empty() )
			vals.append(", ");

		if(table.fields().at(i).second == DBTableDef::DBDataType::Text)
			vals.append("'").append(data[i]).append("'");
		else
			vals.append(data[i]);
	}

	// create sql
	std::string sql("INSERT OR ABORT INTO ");
	sql.append(table.name()).append(" VALUES(").append(vals).append(")");
	return sql;
}
