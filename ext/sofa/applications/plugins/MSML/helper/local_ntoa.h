/*
 * local_ntoa.h
 *
 *  Created on: Feb 25, 2013
 *      Author: steverma
 */

#ifndef LOCAL_NTOA_H_
#define LOCAL_NTOA_H_

#include <boost/lexical_cast.hpp>

/**
 * local_n[umber]toa[scii] is a helper function that encapsulates
 * boost::lexical_cast<std::string, numType> with numType usually [u]int or
 * double. This avoids explicitly constructing a stringstream for the official
 * C++ way of converting between numbers and strings
 */
template<class numType> static inline std::string local_ntoa(numType value) {
	return boost::lexical_cast<std::string, numType>(value);
}

#endif /* LOCAL_NTOA_H_ */
