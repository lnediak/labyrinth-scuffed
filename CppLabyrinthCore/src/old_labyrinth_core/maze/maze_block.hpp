#ifndef SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_BLOCK_HPP_
#define SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_BLOCK_HPP_

#include <old_labyrinth_core/abstract_block.hpp>
#include <algorithm>


namespace labyrinth_core {

namespace maze {

class MazeBlock: public AbstractBlock {

	size_t numDims;
	std::uint64_t* location;

protected:
	MazeBlock(size_t dims, const std::uint64_t* location_in): numDims(dims) {
		location = new std::uint64_t[numDims];
		if (location_in != nullptr) {
			std::copy(location_in, location_in + numDims, location);
		} else {
			location = nullptr;
		}
	}

public:
	~MazeBlock() {
		delete[] location;
	}

	size_t getNumDims() const override {
		return numDims;
	}

	std::uint64_t* getLocation() const override {
		std::uint64_t* toreturn = new std::uint64_t[numDims];
		std::copy(location, location + numDims, toreturn);
		return toreturn;
	}

	void rayTrace(const double* point, const double* direction,
			std::string* errMsg,
			std::vector<Color>* colors,
			std::vector<double>* t_values) const override {
		colors->clear();
		t_values->clear();
	}

};

}

}

#endif /* SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_BLOCK_HPP_ */
