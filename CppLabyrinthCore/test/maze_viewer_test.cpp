#include <labyrinth_core/maze/maze_viewer.hpp>

#include <chrono>
#include <iostream>
#include <sstream>

#include "libs/lodepng.h"

void writeIMG(const labyrinth_core::maze::MazeViewer& viewer,
		size_t width, size_t height) {
	const std::string dirname = "/home/study/produceviewer/";
	static size_t i = 0;
	std::stringstream ss;
	ss << dirname << i << ".png";
	auto start = std::chrono::high_resolution_clock::now();
	std::vector<std::uint8_t*> result = viewer.render();
	double timeSpent = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::high_resolution_clock::now() - start).count();
	std::cout << 1000000000/timeSpent << "fps, " <<
			timeSpent/1000000000 << "s" << std::endl;
	unsigned lodepng_error = lodepng::encode(ss.str(), result[0],
			width, height);
	if (lodepng_error > 0) {
		std::cout << "lodepng error: " <<
				lodepng_error_text(lodepng_error) << std::endl;
	}
	i++;
}

void gen3D() {
	constexpr size_t width = 6000, height = 4000;
	constexpr size_t numDims = 3;
	const std::uint32_t inDimensions[] = {
			10, 10, 10
	};
	labyrinth_core::maze::Maze maze (numDims, inDimensions);
	const double camera[] = {-5, 5, 5};
	labyrinth_core::maze::MazeViewer::Slice slice (numDims, width, height);
	labyrinth_core::maze::MazeViewer::ViewerOptions options (numDims, 100);
	options.addSlice(slice);
	labyrinth_core::maze::MazeViewer viewer (maze, options, camera);
	for (size_t i = 0; i < 360; i += 10) {
		viewer.rotateRight(10);
		writeIMG(viewer, width, height);
	}
	viewer.moveForward(100);
	writeIMG(viewer, width, height);
	viewer.moveBackwards(2);
	writeIMG(viewer, width, height);
	viewer.moveLeft(1);
	writeIMG(viewer, width, height);
	viewer.moveRight(3);
	writeIMG(viewer, width, height);
	viewer.moveLeft(2);
	writeIMG(viewer, width, height);
	viewer.moveForward(0.5);
	writeIMG(viewer, width, height);
	viewer.rotateLeft(15);
	writeIMG(viewer, width, height);
	viewer.rotateUp(30);
	writeIMG(viewer, width, height);
	viewer.moveForward(4);
	writeIMG(viewer, width, height);
	viewer.rotateRight(30);
	writeIMG(viewer, width, height);
	viewer.rotateDown(30);
	writeIMG(viewer, width, height);
	for (size_t i = 0; i <= 360; i += 10) {
		viewer.rotateLeft(10);
		writeIMG(viewer, width, height);
	}
}

int main() {
	gen3D();
}
