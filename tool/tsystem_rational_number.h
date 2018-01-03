#ifndef TSYSTEM_RATIONAL_NUMBER_H_
#define TSYSTEM_RATIONAL_NUMBER_H_

#include <string>
#include "TLib/tool/tsystem_rational_number.pb.h"

namespace TSystem
{
	bool operator==(const RationalNumber& left, const RationalNumber& right);

	double RationalDouble( const RationalNumber& n);
	
	void   FillRational( int round, RationalNumber& n);
	void   FillRational( const std::string& value, RationalNumber& n); // throw TException if bad value string

	std::string   RationalToString(const RationalNumber& n);
	void		  StringToRational(const std::string& str, RationalNumber& n);
}

inline double TSystem::RationalDouble( const RationalNumber& n)
{
	return n.r() + static_cast<double>(n.n()) / n.d();
}

inline bool TSystem::operator==(const RationalNumber& left, const RationalNumber& right)
{
	return left.r() == right.r() && left.n() == right.n() && left.d() == right.d();
}


#endif // TSYSTEM_RATIONAL_NUMBER_H_


