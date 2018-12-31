#include <labyrinth_core/maze/maze_renderer.hpp>

#include <chrono>
#include <iostream>
#include <sstream>

#include "libs/lodepng.h"

void gen3D() {
	const size_t width = 4000, height = 6000;
	const size_t numDims = 3;
	const std::uint32_t inDimensions[] = {
			10, 10, 10
	};
	labyrinth_core::maze::Maze maze (numDims, inDimensions);
	std::uint8_t* output = new std::uint8_t[width * height * 4];
	const double camera[] = {-5, -5, 5};
	labyrinth_core::maze::MazeRenderer renderer (maze, camera);
	for (double theta = 0; theta <= 90; theta++) {
		double angle = theta * 0.0174532925199432957692;
		const double forward[] = {std::cos(angle), std::sin(angle), 0};
		const double right[] = {-std::sin(angle), std::cos(angle), 0};
		const double up[] = {0, 0, 1};
		auto start = std::chrono::high_resolution_clock::now();
		renderer.render(output, forward, right, up, width, height, 100);
		renderer.waitForFinished();
		double timeSpent = std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::high_resolution_clock::now() - start).count();
		std::cout << 1000000000/timeSpent << "fps, " <<
				timeSpent/1000000000 << "s" << std::endl;
		std::stringstream ss;
		ss << "/home/study/producebla/" << theta << ".png";
		unsigned lodepng_error = lodepng::encode(ss.str(), output,
				width, height);
		if (lodepng_error > 0) {
			std::cout << "lodepng error: " <<
					lodepng_error_text(lodepng_error) << std::endl;
		}
	}
	delete[] output;
}

int main() {
	gen3D();
}
