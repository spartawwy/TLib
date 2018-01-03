// By Xixin Silas Cheng
// misc utility functions for reading info from sqlitedb

#ifndef TSYSTEM_SQLITE_FUNCS_H_
#define TSYSTEM_SQLITE_FUNCS_H_

#include <set>
#include <string>
 
#include <SQLite/SQLite.h>

// forward declaration
namespace SQLite 
{
	class SQLiteConnection;
}


namespace TSystem
{
namespace utility
{
	// load all table names from sqlite database, throw TException when sqlite api error
	std::set<std::string> LoadTableNames(SQLite::SQLiteConnection& sqlite_db, const std::string& db_name = "");

	// check if exist table,  throw TException when sqlite api error
	bool				  ExistTable(const std::string& name, SQLite::SQLiteConnection& sqlite_db, const std::string& db_name = "");
}
}

#endif// TSYSTEM_SQLITE_FUNCS_H_