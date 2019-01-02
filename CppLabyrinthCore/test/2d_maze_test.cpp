#include <labyrinth_core/maze/maze.hpp>

#include <iostream>

int main() {
	labyrinth_core::maze::Maze::MazeGenerationOptions options;
	const std::uint32_t width = 50, height = 50;
	options.setDimensions({width, height});
	options.setSeed("1");
	options.setDensity(0.1);
	options.setBranchProbability(0.1);
	options.setTwistProbability(0.2);
	options.setFlowProbability(1);
	options.setAllowLoops(true);
	labyrinth_core::maze::Maze maze (options);
	for (size_t row = 0; row < height; row++) {
		for (size_t col = 0; col < width; col++) {
			std::cout << ((maze.getBlock(row * width + col) == 0)?
					"  ": "XX");
		}
		std::cout << std::endl;
	}
}
