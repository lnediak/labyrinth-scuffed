#ifndef SRC_LABYRINTH_CORE_ABSTRACT_WORLD_HPP_
#define SRC_LABYRINTH_CORE_ABSTRACT_WORLD_HPP_

#include <string>

#include "labyrinth_core/color.hpp"

namespace labyrinth_core {

class AbstractWorld {

public:
	virtual ~AbstractWorld() {}

	virtual size_t getNumDims() const = 0;

	/**
	 * This makes a copy of the camera location pointer/array.
	 */
	virtual double* getCameraLocation() const = 0;

	/**
	 * This copies the input pointer/array to the actual.
	 */
	virtual void setCameraLocation(const double* newLocation) = 0;

	virtual double* getForwardDirection() const = 0;

	virtual void setForwardDirection(const double* newDirection) = 0;

	/**
	 * Returns the Color. If an error occurs, it is written into errMsg.
	 * If errMsg is nullptr, this function can throw a Segmentation Fault.
	 */
	virtual Color rayTrace(const double* direction, Color backgroundColor,
			std::string* errMsg) const = 0;

};

}

#endif /* SRC_LABYRINTH_CORE_ABSTRACT_WORLD_HPP_ */
