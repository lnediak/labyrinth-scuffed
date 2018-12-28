#ifndef SRC_LABYRINTH_CORE_MAZE_MAZE_KERNEL_HPP_
#define SRC_LABYRINTH_CORE_MAZE_MAZE_KERNEL_HPP_

#include <labyrinth_core/color.hpp>
#include <labyrinth_core/maze/maze.hpp>

#include <algorithm>

#include <cmath>

namespace labyrinth_core {

namespace maze {

class MazeKernel {

	Maze maze;
	size_t numDims;
	std::uint32_t* dimensions;
	double* camera;

	// these are so that one doesn't have to call new and delete each time
	double* steps;
	std::int8_t* signs;
	std::int32_t* currBlock;
	// location = camera + t * direction
	double* location;
	// currBlock + offsets = location
	double* offsets;

	struct RayIntersection {

		double t;
		bool isForward;

		bool operator<(const RayIntersection& other) const noexcept {
			return t < other.t;
		}

	};

	RayIntersection* intersections;

public:
	MazeKernel(const Maze& maze, const double* inCamera): maze(maze, 0) {
		numDims = maze.getNumDims();
		dimensions = maze.getDimensions();
		camera = new double[numDims];
		setCamera(inCamera);

		steps = new double[numDims];
		signs = new std::int8_t[numDims];
		currBlock = new std::int32_t[numDims];
		location = new double[numDims];
		offsets = new double[numDims];
		intersections = new RayIntersection[numDims * 2];
	}

	MazeKernel(MazeKernel& other) = delete;
	MazeKernel(const MazeKernel& other) = delete;
	MazeKernel(MazeKernel&& other) = delete;
	MazeKernel& operator=(MazeKernel& other) = delete;
	MazeKernel& operator=(const MazeKernel& other) = delete;
	MazeKernel& operator=(MazeKernel&& other) = delete;

	~MazeKernel() {
		maze.invalidate();
		delete[] dimensions;
		delete[] camera;

		delete[] steps;
		delete[] signs;
		delete[] currBlock;
		delete[] location;
		delete[] offsets;
		delete[] intersections;
	}

	void setCamera(const double* newCamera) {
		std::copy(newCamera, newCamera + maze.getNumDims(), camera);
	}

private:
	bool isInBounds(size_t numDims,
			const std::int32_t* location,
			const std::uint32_t* dimensions) {
		const std::int32_t* locationEnd = location + numDims;
		for (; location < locationEnd; ++location, ++dimensions) {
			if ((*location < 0) ||
					(*location >= static_cast<std::int32_t>(*dimensions))) {
				return false;
			}
		}
		return true;
	}

	double intersectMaze(const double* direction, bool* intersects) {
		// check if it intersects the maze at all
		// using convex object method
		for (size_t i = 0; i < numDims; i++) {
			// threshold = <normal, some_point_on_plane>
			// t = (threshold - <camera, normal>) / <ray, normal>
			double numerator1 = dimensions[i] - 0.5 - location[i];
			double diri = direction[i];
			bool isDiriBelowThreshold = std::abs(diri) < 1e-6;
			double t1 = isDiriBelowThreshold?
					-1000000: numerator1 / diri;

			intersections[i].t = t1;
			intersections[i].isForward = (t1 > 0)?
					numerator1 < 0: numerator1 >= 0;


			double numerator2 = 0.5 + location[i];
			double t2 = isDiriBelowThreshold?
					-1000000: numerator2 / -diri;

			intersections[i + numDims].t = t2;
			intersections[i + numDims].isForward = (t2 > 0)?
					numerator2 < 0: numerator2 >= 0;
		}
		std::sort(intersections, intersections + numDims * 2);
		int64_t lastForward = -1;
		bool encounteredNonForward = false;
		for (size_t i = 0; i < numDims * 2; i++) {
			if (intersections[i].isForward) {
				if (encounteredNonForward) {
					*intersects = false;
					return -1000000;
				}
				lastForward = i;
			} else {
				encounteredNonForward = true;
			}
		}
		if (lastForward == -1) {
			*intersects = false;
			return -1000000;
		}
		double t = intersections[lastForward].t;
		if (t <= 0) {
			*intersects = false;
		}
		*intersects = true;
		return t;
	}

public:
	Color operator()(const double* direction, Color backgroundColor) {
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
		bool intersects = true;
		if (!isInBounds(numDims, currBlock, dimensions)) {
			// just for the sake of floating-point imprecision.
			double t = intersectMaze(direction, &intersects) + 1e-6;
			if (intersects) {
				for (size_t i = 0; i < numDims; i++) {
					location[i] += direction[i] * t;
					currBlock[i] = std::floor(location[i] + 0.5);
					offsets[i] = location[i] - currBlock[i];
				}
			}
		}


		Color result = backgroundColor;
		if (intersects) {
			while (isInBounds(numDims, currBlock, dimensions)) {
				std::uint8_t block = maze.getBlock(currBlock);
				if (block != 0) {
					result = Maze::getBlockColor(block, numDims, offsets);
					break;
				}

				//size_t minStepInd = 0;
				double minStep = steps[0] - 0.5 - signs[0] * offsets[0];
				double currStep;
				for (size_t i = 1; i < numDims; i++) {
					currStep = steps[i] - 0.5 - signs[i] * offsets[i];
					if (currStep < minStep) {
						//minStepInd = i;
						minStep = currStep;
					}
				}
				// paranoia
				minStep += 1e-6;
				// we can safely assume that we don't have a parallel plane selected
				for (size_t i = 0; i < numDims; i++) {
					location[i] += direction[i] * minStep;
					currBlock[i] = std::floor(location[i] + 0.5);
					offsets[i] = location[i] - currBlock[i];
				}
			}
		}

		return result;
	}

};

} // maze

} // labyrinth_core

#endif /* SRC_LABYRINTH_CORE_MAZE_MAZE_KERNEL_HPP_ */
