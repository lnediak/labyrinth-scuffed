#ifndef SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_STONE_HPP_
#define SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_STONE_HPP_

#include <old_labyrinth_core/maze/maze_block.hpp>
#include <algorithm>


namespace labyrinth_core {

namespace maze {

class MazeStone: public MazeBlock {

public:
	MazeStone(size_t dims, const std::uint64_t* location_in):
		MazeBlock(dims, location_in) {}

	bool isVoid() const override {
		return false;
	}

	void rayTrace(const double* point, const double* direction,
			std::string* errMsg,
			std::vector<Color>* colors,
			std::vector<double>* t_values) const override {
		size_t n = getNumDims();
		double sum = 0;
		const double* point_end = point + n;
		for (; point < point_end; ++point) {
			sum += *point;
		}
		// normalize to [100, 156]
		// from [-n/2, n/2]
		unsigned char color = sum * (28 / (n/2.0)) + 128;
		// just a very grey texture here...
		colors = std::vector<Color>{Color{color, color, color, 0xFF}};
		t_values = std::vector<double>{0};
	}

};

}

}

#endif /* SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_STONE_HPP_ */
