#include "tsystem_generic_param.h"

using namespace TSystem;

bool TSystem::utility::ValidateParameter(const GenericParam* param)
{
	if( !param )
		return false;

	switch( param->size_type() )
	{
	case GenericParam::SCALAR:
		switch (param->data_type())	
		{
		case GenericParam::UINT:
			return param->has_val_uint32();
		case GenericParam::INT:
			return param->has_val_int32();
		case GenericParam::DOUBLE:
			return param->has_val_double();
		case GenericParam::STRING:
			return param->has_val_string();
		default:
			return false;
		}

	case GenericParam::ARRAY:
		switch (param->data_type())	
		{
		case GenericParam::UINT:
			return param->array_uint32().size()>0;
		case GenericParam::INT:
			return param->array_int32().size()>0;
		case GenericParam::DOUBLE:
			return param->array_double().size()>0;
		case GenericParam::STRING:
			return param->array_string().size()>0;
		default:
			return false;
		}
	}
	return false;
}

bool TSystem::utility::CheckScalarInt32(const GenericParam* param)
{
	if( !param )
		return false;

	return param->size_type() == GenericParam::SCALAR 
		&& param->data_type() == GenericParam::INT;
}

bool TSystem::utility::CheckScalarUint(const GenericParam* param)
{
	if( !param )
		return false;

	return param->size_type() == GenericParam::SCALAR 
		&& param->data_type() == GenericParam::UINT;

}
bool TSystem::utility::CheckScalarDouble(const GenericParam* param)
{
	if( !param )
		return false;

	return param->size_type() == GenericParam::SCALAR 
		&& param->data_type() == GenericParam::DOUBLE;
}

bool TSystem::utility::CheckScalarString(const GenericParam* param)
{
	if( !param )
		return false;

	return param->size_type() == GenericParam::SCALAR 
		&& param->data_type() == GenericParam::STRING;

}

bool TSystem::utility::CheckArrayInt32(const GenericParam* param)
{
	if( !param )
		return false;

	return param->size_type() == GenericParam::ARRAY 
		&& param->data_type() == GenericParam::INT;
}

bool TSystem::utility::CheckArrayUint(const GenericParam* param)
{
	if( !param )
		return false;

	return param->size_type() == GenericParam::ARRAY 
		&& param->data_type() == GenericParam::UINT;
}

bool TSystem::utility::CheckArrayDouble(const GenericParam* param)
{
	if( !param )
		return false;

	return param->size_type() == GenericParam::ARRAY 
		&& param->data_type() == GenericParam::DOUBLE;
}

bool TSystem::utility::CheckArrayString(const GenericParam* param)
{
	if( !param )
		return false;

	return param->size_type() == GenericParam::ARRAY 
		&& param->data_type() == GenericParam::STRING;
}

unsigned int TSystem::utility::GetParameterSize(const GenericParam& param)
{
	switch( param.size_type() )
	{
	case GenericParam::SCALAR:
		return 1;

	case GenericParam::ARRAY:
		switch (param.data_type())	
		{
		case GenericParam::UINT:
			return param.array_uint32().size();
		case GenericParam::INT:
			return param.array_int32().size();
		case GenericParam::DOUBLE:
			return param.array_double().size();
		case GenericParam::STRING:
			return param.array_string().size();
		default:
			assert(0);
		}
	}
	assert(0);
}

