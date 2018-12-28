#include <labyrinth_core/maze/maze_renderer.hpp>

#include <iostream>

#include "libs/lodepng.h"

int main() {
	const size_t width = 640, height = 480;
	const size_t numDims = 3;
	const std::uint32_t inDimensions[] = {
			10, 10, 10
	};
	labyrinth_core::maze::Maze maze (numDims, inDimensions);
	std::uint8_t* output = new std::uint8_t[width * height * 4];
	const double camera[] = {-1, 0, 0};
	const double forward[] = {1, 0, 0};
	const double right[] = {0, 1, 0};
	const double up[] = {0, 0, 1};
	labyrinth_core::maze::MazeRenderer::render(output, maze,
			camera, forward, right, up,
			width, height, 100);
	unsigned lodepng_error = lodepng::encode("/home/study/produced.png", output,
			width, height);
	delete[] output;
	if (lodepng_error > 0) {
		std::cout << "lodepng error: " <<
				lodepng_error_text(lodepng_error) << std::endl;
	}
}
