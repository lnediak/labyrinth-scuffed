#include <iostream>

#include <labyrinth_core/maze/maze.hpp>

#include <iostream>

int main() {
	labyrinth_core::maze::Maze::MazeGenerationOptions options;
	const std::uint32_t width = 3000, height = 3000;
	options.setDimensions({width, height});
	options.setSeed("1");
	options.setDensity(1);
	options.setBranchProbability(0.05);
	options.setBranchDeathProbability(0.01);
	options.setTwistProbability(0.5);
	options.setFlowProbability(0.7);
	options.setRestrictNewAmount(1);
	options.setLoopProbability(0);
	options.setBlockProbability(0);
	options.setMaxUseless(50000000);
	labyrinth_core::maze::Maze maze (options);
	for (size_t row = 0; row < height; row++) {
		for (size_t col = 0; col < width; col++) {
			std::cout << ((maze.getBlock(row * width + col) == 0)?
					"  ": "XX");
		}
		std::cout << std::endl;
	}
}
