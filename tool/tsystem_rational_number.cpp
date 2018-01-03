#include "tsystem_rational_number.h"

#include <boost/math/common_factor.hpp>
#include <boost/lexical_cast.hpp>

#include "TLib/core/tsystem_core_common.h"

void TSystem::FillRational( int round, RationalNumber& n)
{
	n.set_r(round);
	n.set_n(0);
	n.set_d(1);
}

void TSystem::FillRational( const std::string& value, RationalNumber& n)
{
	size_t pos = value.find('.');
	try
	{
		if( pos == std::string::npos )
		{
			int round = boost::lexical_cast<int>(value);
			FillRational(round, n);
		}else
		{
			int round = boost::lexical_cast<int>(value.c_str(), pos);
			unsigned int decimal =  boost::lexical_cast<unsigned int>(value.c_str()+pos+1);
			
			unsigned int d = static_cast<unsigned int>(pow(10, value.length()-pos-1));
			unsigned int gcd = boost::math::gcd<unsigned int>(decimal, d);
			d /= gcd;
			decimal /= gcd;

			n.set_r(round);
			n.set_n(decimal*(round<0?-1:1));
			n.set_d(d);
		}
	}catch(const boost::bad_lexical_cast&)
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT, "TSystem::FillRational",
				"Bad value_str: " + value);
	}
}

std::string TSystem::RationalToString(const RationalNumber& num)
{
	if( !num.n() )
		return utility::FormatStr("%d", num.r());
	else
		return utility::FormatStr("%d|%d|%d", num.r(), num.n(), num.d());
}

void TSystem::StringToRational(const std::string& str, RationalNumber& num)
{
	try
	{
		auto vals = utility::split(str, "|");

		if( vals.size() == 1 )
		{
			int round = boost::lexical_cast<int>(vals[0]);
			FillRational(round, num);
		}else if(vals.size() == 3)
		{
			int r = boost::lexical_cast<int>(vals[0]);
			int n = boost::lexical_cast<int>(vals[1]);
			int d = boost::lexical_cast<int>(vals[2]);
			
			if( d <= 0 )
				ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT, "TSystem::StringToRational",
				"Bad value string: " + str);

			num.set_r(r);
			num.set_n(n);
			num.set_d(d);
		}else
			ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT, "TSystem::StringToRational",
				"Bad value string: " + str);
	}catch(const boost::bad_lexical_cast&)
	{
		ThrowTException(CoreErrorCategory::ErrorCode::BAD_ARGUMENT, "TSystem::StringToRational",
				"Bad value string: " + str);
	}

}