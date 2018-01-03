// a generic listing file

#ifndef TSYSTEM_LISTING_H_
#define TSYSTEM_LISTING_H_

#include <unordered_map>
#include <vector>

#include "TLib/core/tsystem_message_system.h"

#include "TLib/tool/tsystem_generic_param.h"
#include "TLib/tool/tsystem_listing.pb.h"

namespace TSystem
{
namespace utility
{
	// load listing from file
	bool LoadListing(const std::string& file, const TSystem::MessageSystem& msg_system, Listing& listing, TError* p_error = nullptr);

	// dump listing to file
	bool DumpListing(const std::string& file, const TSystem::MessageSystem& msg_system, const Listing& listing, TError* p_error = nullptr);
	
	// listing reader, will map the variables
	class ListingMap
	{
	public:
		
		ListingMap();

		// create from existing listing
		explicit ListingMap(Listing&& listing);

		ListingMap(ListingMap&& in);

		// create from loading
		ListingMap(const std::string& file, const TSystem::MessageSystem& msg_system);

		// move assign
		ListingMap& operator=(ListingMap&& in);

		// move merage
		ListingMap& operator<<(ListingMap&& in);

		// load listing file
		bool LoadFromFile(const std::string& file, const TSystem::MessageSystem& msg_system, TError* p_te = nullptr);

		// dump listing to file, throw TException when failed
		void DumpToFile(const std::string& file, const TSystem::MessageSystem& msg_system);

		// read / set listing name
		const std::string& name() const;
		void name(const std::string& n);
		
		// find variable
		GenericParam* FindVariable(const std::string& name);
		const GenericParam* FindVariable(const std::string& name) const;

		// get variable, throw TException if not found
		GenericParam& GetVariable(const std::string& name);
		const GenericParam& GetVariable(const std::string& name) const;

		// all variable names
		Listing& AllVariables() { return vars_holder_; }
		const Listing& AllVariables() const { return vars_holder_; }

		// create if not find
		GenericParam& operator[](const std::string& name);

	private:

		void MapListingVars();

		std::unordered_map<std::string, GenericParam*> vars_map_;

		Listing   vars_holder_;	
	};

	// check list map template interface
	template<class DerivedListingMapType>
	TError CheckListingMap(const ListingMap& list_map);

	// format list map template interface
	template<class DerivedListingMapType>
	void FormatListingMap(ListingMap& list_map);
	

	// consolidated helper class to modify listing map
	class ListingMapWriter
	{
	public:

		explicit ListingMapWriter( ListingMap& map): map_(map) {}

		// add listing variable, throw TExcpetion when already added
		GenericParam& AddListingVariable(const std::string& var_name, GenericParam::DataType data_type, GenericParam::SizeType size_type);

		// add array value, throw TException if var not found
		void AddArrayInt32(const std::string& var_name, int value);
		void AddArrayUint(const std::string& var_name, unsigned int value);
		void AddArrayDouble(const std::string& var_name, double value);
		void AddArrayString(const std::string& var_name, const std::string& value);

		// set scalar value, throw TException if var not found
		void SetScalarInt32(const std::string& var_name, int value);
		void SetScalarUint(const std::string& var_name, unsigned int value);
		void SetScalarDouble(const std::string& var_name, double value);
		void SetScalarString(const std::string& var_name, const std::string& value);

		// set array value, throw TException if var not found
		void SetArrayInt32(const std::string& var_name, int value, int pos);
		void SetArrayUint(const std::string& var_name, unsigned int value, int pos);
		void SetArrayDouble(const std::string& var_name, double value, int pos);
		void SetArrayString(const std::string& var_name, const std::string& value, int pos);

	private:

		ListingMap& map_; 
	};
}
}

#endif 