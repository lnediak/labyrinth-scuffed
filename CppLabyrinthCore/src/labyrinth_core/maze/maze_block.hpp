#ifndef SRC_LABYRINTH_CORE_MAZE_MAZE_BLOCK_HPP_
#define SRC_LABYRINTH_CORE_MAZE_MAZE_BLOCK_HPP_

#include <algorithm>

#include "labyrinth_core/abstract_block.hpp"

namespace labyrinth_core {

namespace maze {

class MazeBlock: public AbstractBlock {

	size_t numDims;
	std::uint64_t* location;
	double* camera;
	double* forward;

protected:
	// this is for raytrace calculations
	double* numerators;

	MazeBlock(size_t dims, const std::uint64_t* location_in,
			const double* camera_in, const double* forward_in): numDims(dims) {
		location = new std::uint64_t[numDims];
		camera = new double[numDims];
		forward = new double[numDims];
		std::copy(location_in, location_in + numDims, location);
		std::copy(camera_in, camera_in + numDims, camera);
		std::copy(forward_in, forward_in + numDims, forward);

		// here we calculate stuff.
		numerators = new double[numDims * 2];
		for (size_t i = 0; i < numDims; i++) {
			// the numerator is threshold - <camera, normal>
			// threshold = <normal, modified_location>
			// modified_location is location but
			// modified_location[i] = location[i] + offset / 2.0
			numerators[i] = location[i] + 0.5 - camera[i];
			numerators[i + numDims] = -location[i] + 0.5 + camera[i];
		}
	}

public:
	~MazeBlock() {
		delete[] location;
		delete[] camera;
		delete[] forward;
		delete[] numerators;
	}

	double* getCameraLocation() const override {
		double* toreturn = new double[numDims];
		std::copy(camera, camera + numDims, toreturn);
		return toreturn;
	}

	void setCameraLocation(const double* newLocation) override {
		std::copy(newLocation, newLocation + numDims, camera);
	}

	double* getForwardDirection() const override {
		double* toreturn = new double[numDims];
		std::copy(forward, forward + numDims, toreturn);
		return toreturn;
	}

	void setForwardDirection(const double* newLocation) override {
		std::copy(newLocation, newLocation + numDims, forward);
	}

	void rayTrace(const double* direction, std::string* errMsg,
			std::vector<Color>* colors,
			std::vector<double>* t_values) const override {
		colors->clear();
		t_values->clear();
	}

	std::uint64_t* getLocation() const override {
		std::uint64_t* toreturn = new std::uint64_t[numDims];
		std::copy(location, location + numDims, toreturn);
		return toreturn;
	}

};

}

}

#endif /* SRC_LABYRINTH_CORE_MAZE_MAZE_BLOCK_HPP_ */
