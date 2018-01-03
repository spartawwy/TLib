// By Xixin Silas Cheng
// class for database table defination

#ifndef TSYSTEM_TABLE_DEFINITION_H_
#define TSYSTEM_TABLE_DEFINITION_H_

#include <string>
#include <vector>

namespace TSystem
{
namespace utility
{
	class DBTableDef
	{
	public:
		enum class DBDataType
		{
			Integer,
			Text,
			Double
		};
		typedef std::pair<std::string, DBDataType>	FieldInfo;

		explicit DBTableDef(const std::string& name);

		// add fieled info
		void AddField(const std::string& name, DBDataType type);	

		// add field into primary key
		void AddKeyField(const std::string& name);

		// name
		const std::string&				name()	 const {return name_;}		

		// get fields info
		const std::vector<FieldInfo>&	fields() const {return fields_;}      

		const std::vector<std::string>& key() const    {return key_;}

	private:
		std::string							name_;		  // the table name
		std::vector<FieldInfo>              fields_;	  // fields info
		std::vector<std::string>            key_;         // primary key
	};

	// sql for create table
	std::string CreateTableSQL(const DBTableDef& table);

	// sql for clear table
	std::string DropTableSQL(const DBTableDef& table);

	// sql for select all column's in table
	std::string SelectAllSQL(const DBTableDef& table);

	// sql for insert into database
	std::string InsertOrAbortSQL(const DBTableDef& table, const std::vector<std::string>& data);
}
}

#endif // TSYSTEM_TABLE_DEFINITION_H_