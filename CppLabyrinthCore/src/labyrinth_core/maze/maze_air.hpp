#ifndef SRC_LABYRINTH_CORE_MAZE_MAZE_AIR_HPP_
#define SRC_LABYRINTH_CORE_MAZE_MAZE_AIR_HPP_

#include "labyrinth_core/maze/maze_block.hpp"

namespace labyrinth_core {

namespace maze {

class MazeAir: public MazeBlock {

public:
	MazeAir(size_t dims, const std::uint64_t* location_in,
			const double* camera_in, const double* forward_in):
				MazeBlock(dims, location_in,
						camera_in, forward_in) {}

	bool isVoid() const override {
		return true;
	}

};

}

}

#endif /* SRC_LABYRINTH_CORE_MAZE_MAZE_AIR_HPP_ */
