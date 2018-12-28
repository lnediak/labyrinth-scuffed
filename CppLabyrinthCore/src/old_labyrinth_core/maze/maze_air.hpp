#ifndef SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_AIR_HPP_
#define SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_AIR_HPP_

#include <old_labyrinth_core/maze/maze_block.hpp>

namespace labyrinth_core {

namespace maze {

class MazeAir: public MazeBlock {

public:
	MazeAir(size_t dims, const std::uint64_t* location_in):
		MazeBlock(dims, location_in) {}

	bool isVoid() const override {
		return true;
	}

};

}

}

#endif /* SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_AIR_HPP_ */
