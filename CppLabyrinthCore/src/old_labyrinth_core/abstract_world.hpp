#ifndef SRC_OLD_LABYRINTH_CORE_ABSTRACT_WORLD_HPP_
#define SRC_OLD_LABYRINTH_CORE_ABSTRACT_WORLD_HPP_

#include <old_labyrinth_core/color.hpp>
#include <string>


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

#endif /* SRC_OLD_LABYRINTH_CORE_ABSTRACT_WORLD_HPP_ */
