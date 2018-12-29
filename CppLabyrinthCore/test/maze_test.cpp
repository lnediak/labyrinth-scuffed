#include <labyrinth_core/maze/maze_renderer.hpp>

#include <chrono>
#include <iostream>

#include "libs/lodepng.h"

int main() {
	const size_t width = 1000, height = 1000;
	const size_t numDims = 3;
	const std::uint32_t inDimensions[] = {
			10, 10, 10
	};
	labyrinth_core::maze::Maze maze (numDims, inDimensions);
	std::uint8_t* output = new std::uint8_t[width * height * 4];
	const double camera[] = {-5, -5, 5};
	const double theta = 44.8;
	const double forward[] = {std::cos(theta), std::sin(theta), 0};
	const double right[] = {-std::sin(theta), std::cos(theta), 0};
	const double up[] = {0, 0, 1};
	auto start = std::chrono::high_resolution_clock::now();
	labyrinth_core::maze::MazeRenderer::render(output, maze,
			camera, forward, right, up,
			width, height, 100);
	std::cout << 1000000000/static_cast<double>(
			std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::high_resolution_clock::now() - start).count()) << std::endl;
	unsigned lodepng_error = lodepng::encode("/home/study/produced.png", output,
			width, height);
	delete[] output;
	if (lodepng_error > 0) {
		std::cout << "lodepng error: " <<
				lodepng_error_text(lodepng_error) << std::endl;
	}
}
