/*
 * ParalutionInitializer.h
 *
 *  Created on: Mar 5, 2013
 *      Author: steverma
 */

#ifndef PARALUTIONINITIALIZER_H_
#define PARALUTIONINITIALIZER_H_

#include <paralution.hpp>

namespace sofa {
namespace helper {

using namespace paralution;

class ParalutionInitializer {
	public:
	ParalutionInitializer() {
		std::cout<<"Trying to init paralution\n";
		if ((refs++) == 0) {
			init_paralution();
			info_paralution();
			std::cout<<"Paralution is initialized\n";
		}
	}
	virtual ~ParalutionInitializer() {
		if ((--refs) == 0) stop_paralution();
	}
	private:
	static uint refs;
};

} /* namespace helper */
} /* namespace sofa */
#endif /* PARALUTIONINITIALIZER_H_ */
