#ifndef TSYSTEM_NETWORK_CMDLINE_H_
#define TSYSTEM_NETWORK_CMDLINE_H_

#include <string>
#include <tuple>
#include "TLib/tool/tsystem_generic_param.pb.h"

namespace TSystem
{
namespace utility
{
	// check parameter
	bool ValidateParameter(const GenericParam* param);

	// check parameter for specific type
	bool CheckScalarInt32(const GenericParam* param);
	bool CheckScalarUint(const GenericParam* param);
	bool CheckScalarDouble(const GenericParam* param);
	bool CheckScalarString(const GenericParam* param);
	bool CheckArrayInt32(const GenericParam* param);
	bool CheckArrayUint(const GenericParam* param);
	bool CheckArrayDouble(const GenericParam* param);
	bool CheckArrayString(const GenericParam* param);

	// get parameter size
	unsigned int GetParameterSize(const GenericParam& param);
}
}


#endif //TSYSTEM_NETWORK_CMDLINE_H_