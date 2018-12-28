#include <labyrinth_core/maze/maze_kernel.hpp>

#include <limits>

#include <cmath>

namespace {

bool isInBounds(size_t numDims, std::int32_t* location, std::uint32_t* dimensions) {
	std::int32_t* locationEnd = location + numDims;
	for (; location < locationEnd; ++location, ++dimensions) {
		if ((*location < 0) || (*location >= *dimensions)) {
			return false;
		}
	}
	return true;
}

}

Color labyrinth_core::maze::MazeKernel::operator()(
		double* direction, Color backgroundColor) {
	size_t numDims = maze.getNumDims();
	std::uint32_t* dimensions = maze.getDimensions();
	double* steps = new double[numDims];
	std::int8_t* signs = new std::int8_t[numDims];
	std::int32_t* currBlock = new std::int32_t[numDims];
	// location = camera + t * direction
	double* location = new double[numDims];
	// currBlock + offsets = location
	double* offsets = new double[numDims];
	for (size_t i = 0; i < numDims; i++) {
		steps[i] = (std::abs(direction[i]) < 1e-6)? -1000000: 1/direction[i];
		if (steps[i] < 0) {
			signs[i] = -1;
			steps[i] = -steps[i];
		} else {
			signs[i] = 1;
		}


		location[i] = camera[i];
		// since blocks are [-0.5, 0.5]
		currBlock[i] = std::floor(location[i] + 0.5);
		offsets[i] = location[i] - currBlock[i];
	}
	Color result = backgroundColor;
	if (!isInBounds(numDims, currBlock, dimensions)) {
		// check if it intersects the maze at all
		// using convex object method
		struct RayIntersection {
			double t;
			bool isForward;
		};
		RayIntersection* intersections = new RayIntersection[numDims * 2];
		for (size_t i = 0; i < numDims; i++) {
			// FINISH THIS
		}

		delete[] intersections;
	}
	while (true) {
		size_t minStepInd = 0;
		double minStep = steps[0] - 0.5 - signs[0] * offsets[0];
		double currStep;
		for (size_t i = 1; i < numDims; i++) {
			currStep = steps[i] - 0.5 - signs[i] * offsets[i];
			if (currStep < minStep) {
				minStepInd = i;
				minStep = currStep;
			}
		}
		// we can safely assume that we don't have a parallel plane selected
		for (size_t i = 0; i < numDims; i++) {
			location[i] += direction[i] * minStep;
			if (i == minStepInd) {
				// because of floating-point imprecision
				currBlock[i] += signs[i];
			} else {
				// in theory this is not necessary,
				// but floating-point imprecision makes me paranoid
				currBlock[i] = std::floor(location[i] + 0.5);
			}
			offsets[i] = location[i] - currBlock[i];
		}
		if (!isInBounds(numDims, currBlock, dimensions)) {
			break;
		}
		std::uint8_t block = maze.getBlock(currBlock);
		if (block == 0) {
			continue;
		}
		result = Maze::getBlockColor(block, offsets);
		break;
	}

	delete[] offsets;
	delete[] location;
	delete[] currBlock;
	delete[] signs;
	delete[] steps;
	delete[] dimensions;

	return result;
}
