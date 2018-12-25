#ifndef SRC_LABYRINTH_CORE_ABSTRACT_THING_HPP_
#define SRC_LABYRINTH_CORE_ABSTRACT_THING_HPP_

#include <string>
#include <vector>

#include "labyrinth_core/color.hpp"

namespace labyrinth_core {

class AbstractThing {

public:
	virtual ~AbstractThing() {}

	virtual size_t getNumDims() const = 0;

	virtual double* getCameraLocation() const = 0;

	virtual void setCameraLocation(const double* newLocation) = 0;

	virtual double* getForwardDirection() const = 0;

	virtual void setForwardDirection(const double* newDirection) = 0;

	/**
	 * errMsg is written to if an error occurs.
	 * colors and t_values are output parameters.
	 * The result must be sorted in increasing order of t_values.
	 */
	virtual void rayTrace(const double* direction, std::string* errMsg,
			std::vector<Color>* colors, std::vector<double>* t_values) const = 0;

};

}

#endif /* SRC_LABYRINTH_CORE_ABSTRACT_THING_HPP_ */
