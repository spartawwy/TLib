#include "tsystem_listing.h"

#include <algorithm>
#include <fstream>

#include "TLib/core/tsystem_core_error.h"
#include "TLib/tool/tsystem_dump_reader.h"

using namespace TSystem;
using namespace TSystem::utility;

// load listing from file
bool TSystem::utility::LoadListing(const std::string& file, const TSystem::MessageSystem& msg_system, Listing& listing, TError* p_error)
{
	std::ifstream input_data_file(file, std::ifstream::binary);
	if( !input_data_file.is_open() )
	{
		if( p_error )
			*p_error = MakeTError(CoreErrorCategory::ErrorCode::BAD_ARGUMENT,
				"LoadListing",
				"can't open file: " + file);
		return false;
	}

	utility::DumpReader reader(input_data_file);
	SerialData data = reader.ParseSerialData();
	if( !Decode(data, listing, msg_system) )
	{
		if( p_error )
			*p_error = MakeTError(CoreErrorCategory::ErrorCode::BAD_CONTENT,
				"LoadListing",
				"corrupted message: " + file);
		return false;
	}
	input_data_file.close();

	return true;
}

// dump listing to file
bool TSystem::utility::DumpListing(const std::string& file, const TSystem::MessageSystem& msg_system, const Listing& listing, TError* p_error)
{
	std::ofstream data_file(file, std::ofstream::binary|std::ofstream::trunc);
	if( !data_file.is_open() )
	{
		if( p_error )
			*p_error = MakeTError(CoreErrorCategory::ErrorCode::BAD_ARGUMENT
				, "DumpListing"
				, "can't open file to write: " + file);
		return false;
	}
	
	communication::CodedDataPtr tmp_ptr = Encode(listing, msg_system);
	data_file.write(tmp_ptr->data(), tmp_ptr->size());
	data_file.close();

	return true;
}

ListingMap::ListingMap()
	: vars_holder_()
	, vars_map_(512)
{
	MapListingVars();
}

ListingMap::ListingMap(Listing&& listing)
	: vars_holder_()
	, vars_map_(512)
{
	vars_holder_.Swap(&listing);
	MapListingVars();
}


ListingMap::ListingMap(ListingMap&& in)
	: vars_holder_()
	, vars_map_(512)
{
	vars_holder_.Swap(&in.vars_holder_);
	MapListingVars();
}

ListingMap::ListingMap(const std::string& file, const MessageSystem& msg_system)
	: vars_holder_()
	, vars_map_(512)
{
	TError te;
	if( !LoadFromFile(file, msg_system, &te ) )
		ThrowTException(std::move(te), "ListingMap::ListingMap");
}

ListingMap& ListingMap::operator=(ListingMap&& in)
{
	if( this == &in )
		return *this;

	vars_holder_.Swap(&in.vars_holder_);
	MapListingVars();

	return *this;
}

ListingMap& ListingMap::operator<<(ListingMap&& in)
{
	if( this == &in )
		return *this;

	vars_holder_.mutable_vars()->MergeFrom(in.vars_holder_.vars());
	MapListingVars();

	return *this;
}

bool ListingMap::LoadFromFile(const std::string& file, const TSystem::MessageSystem& msg_system, TError* p_te)
{ 
	vars_holder_.Clear();
	vars_map_.clear();

	if( LoadListing(file, msg_system, vars_holder_, p_te) )
	{
		MapListingVars();
		return true;
	}
	else
		return false;
}

void ListingMap::DumpToFile(const std::string& file, const TSystem::MessageSystem& msg_system)
{
	TError te;
	if( !DumpListing(file, msg_system, vars_holder_, &te) )
		ThrowTException(std::move(te), "ListingMap::DumpToFile");
}

const std::string& ListingMap::name() const
{
	return vars_holder_.name();
}

void ListingMap::name(const std::string& n)
{
	vars_holder_.set_name(n);
}

void ListingMap::MapListingVars()
{
	vars_map_.clear();

	for( int i = 0; i < vars_holder_.vars().size(); ++i)
	{
		ListingVariable* var = vars_holder_.mutable_vars()->Mutable(i);
		vars_map_.insert(std::make_pair(var->name(), var->mutable_value()));
	}
}

GenericParam* ListingMap::FindVariable(const std::string& name)
{
	auto iter = vars_map_.find(name);
	return iter != vars_map_.end() ? iter->second : nullptr;
}

const GenericParam* ListingMap::FindVariable(const std::string& name) const
{
	auto iter = vars_map_.find(name);
	return iter != vars_map_.end() ? iter->second : nullptr;
}

// get variable, throw exception if not found
GenericParam& ListingMap::GetVariable(const std::string& name)
{
	GenericParam* p = FindVariable(name);
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "ListingMap::GetVariable"
		, "Can't find variable: " + name);

	return *p;
}

const GenericParam& ListingMap::GetVariable(const std::string& name) const
{
	const GenericParam* p = FindVariable(name);
	if( !p )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
		, "ListingMap::GetVariable"
		, "Can't find variable: " + name);

	return *p;
}

GenericParam& ListingMap::operator[](const std::string& name)
{
	auto iter = vars_map_.find(name);
	if( iter == vars_map_.end() )
	{
		ListingVariable* var = vars_holder_.add_vars();
		var->set_name(name);
		iter = vars_map_.insert(std::make_pair(name, var->mutable_value())).first;
	}
	return *iter->second;
}

GenericParam& ListingMapWriter::AddListingVariable(const std::string& var_name, GenericParam::DataType data_type, GenericParam::SizeType size_type)
{
	if( map_.FindVariable(var_name) )
		ThrowTException( CoreErrorCategory::ErrorCode::BAD_ARGUMENT
			, "ListingMapWriter::AddListingVariable"
			, "variable already exists: " + var_name);

	GenericParam& param = map_[var_name];
	param.set_data_type(data_type);
	param.set_size_type(size_type);
	return param;
}

void ListingMapWriter::AddArrayInt32(const std::string& var_name, int value)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckArrayInt32(&param) );
	param.add_array_int32(value);
}

void ListingMapWriter::AddArrayUint(const std::string& var_name, unsigned int value)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckArrayUint(&param) );
	param.add_array_uint32(value);
}

void ListingMapWriter::AddArrayDouble(const std::string& var_name, double value)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckArrayDouble(&param) );
	param.add_array_double(value);
}

void ListingMapWriter::AddArrayString(const std::string& var_name, const std::string& value)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckArrayString(&param) );
	param.add_array_string(value);
}

void ListingMapWriter::SetScalarInt32(const std::string& var_name, int value)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckScalarInt32(&param) );
	param.set_val_int32(value);
}

void ListingMapWriter::SetScalarUint(const std::string& var_name, unsigned int value)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckScalarUint(&param) );
	param.set_val_uint32(value);
}

void ListingMapWriter::SetScalarDouble(const std::string& var_name, double value)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckScalarDouble(&param) );
	param.set_val_double(value);
}

void ListingMapWriter::SetScalarString(const std::string& var_name, const std::string& value)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckScalarString(&param) );
	param.set_val_string(value);
}

void ListingMapWriter::SetArrayInt32(const std::string& var_name, int value, int pos)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckArrayInt32(&param) );
	param.set_array_int32(pos, value);
}

void ListingMapWriter::SetArrayUint(const std::string& var_name, unsigned int value, int pos)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckArrayUint(&param) );
	param.set_array_uint32(pos, value);
}

void ListingMapWriter::SetArrayDouble(const std::string& var_name, double value, int pos)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckArrayDouble(&param) );
	param.set_array_double(pos, value);
}

void ListingMapWriter::SetArrayString(const std::string& var_name, const std::string& value, int pos)
{
	GenericParam& param = map_.GetVariable(var_name);
	assert( utility::CheckArrayString(&param) );
	param.set_array_string(pos, value);
}
