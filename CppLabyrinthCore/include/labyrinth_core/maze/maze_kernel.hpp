#ifndef INCLUDE_LABYRINTH_CORE_MAZE_MAZE_KERNEL_HPP_
#define INCLUDE_LABYRINTH_CORE_MAZE_MAZE_KERNEL_HPP_

#include <labyrinth_core/color.hpp>
#include <labyrinth_core/maze/maze.hpp>

#include <algorithm>
#include <iostream>

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
	}

	void setCamera(const double* newCamera) {
		std::copy(newCamera, newCamera + maze.getNumDims(), camera);
	}

private:
	bool isInBounds(size_t numDims,
			const std::int32_t* location,
			const std::uint32_t* dimensions) const {
		const std::int32_t* locationEnd = location + numDims;
		for (; location < locationEnd; ++location, ++dimensions) {
			if ((*location < 0) ||
					(*location >= static_cast<std::int32_t>(*dimensions))) {
				return false;
			}
		}
		return true;
	}

	double intersectMaze(const double* direction, bool* intersects) const {
		// check if it intersects the maze at all
		// using convex object method
		double lastForwardT = -1000000;
		double firstNonForwardT = 1000000;
		for (size_t i = 0; i < numDims; i++) {
			// threshold = <normal, some_point_on_plane>
			// t = (threshold - <camera, normal>) / <ray, normal>
			double numerator1 = dimensions[i] - 0.5 - location[i];
			double diri = direction[i];
			bool isDiriBelowThreshold = (-1e-6 < diri) && (diri < 1e-6);
			double t1 = isDiriBelowThreshold?
					-1000000: numerator1 / diri;
			bool isForward1 = (t1 > 0)?
					numerator1 < 0: numerator1 >= 0;
			if (isForward1) {
				if (t1 > lastForwardT) {
					lastForwardT = t1;
				}
			} else {
				if (t1 < firstNonForwardT) {
					firstNonForwardT = t1;
				}
			}


			double numerator2 = 0.5 + location[i];
			double t2 = isDiriBelowThreshold?
					-1000000: numerator2 / -diri;
			bool isForward2 = (t2 > 0)?
					numerator2 < 0: numerator2 >= 0;
			if (isForward2) {
				if (t2 > lastForwardT) {
					lastForwardT = t2;
				}
			} else {
				if (t2 < firstNonForwardT) {
					firstNonForwardT = t2;
				}
			}
		}
		if (lastForwardT <= 0) {
			*intersects = false;
			return -1000000;
		}
		if (firstNonForwardT <= lastForwardT) {
			*intersects = false;
			return -1000000;
		}
		*intersects = true;
		return lastForwardT;
	}

public:
	/**
	 * Writes output into output.
	 */
	void operator()(std::uint8_t* output,
			const double* direction, Color backgroundColor) const {
		double *iter_camera, *iter_steps;
		std::int8_t* iter_signs;
		std::int32_t* iter_currBlock;
		double *iter_location, *iter_offsets;
		const double *iter_direction, *dir_end = direction + numDims;
		for (iter_camera = camera, iter_steps = steps, iter_signs = signs,
				iter_currBlock = currBlock, iter_location = location,
				iter_offsets = offsets, iter_direction = direction;
				iter_direction < dir_end;
				++iter_camera, ++iter_steps, ++iter_signs, ++iter_currBlock,
				++iter_location, ++iter_offsets, ++iter_direction) {
			double diri = *iter_direction;
			double step = ((-1e-6 < diri) && (diri < 1e-6))?
					-1000000: 1/diri;
			if (step < 0) {
				*iter_signs = -1;
				*iter_steps = -step;
			} else {
				*iter_signs = 1;
				*iter_steps = step;
			}


			double loc = *iter_location = *iter_camera;
			// since blocks are [-0.5, 0.5]
			double currB = *iter_currBlock = std::floor(loc + 0.5);
			*iter_offsets = loc - currB;
		}
		bool intersects = true;
		if (!isInBounds(numDims, currBlock, dimensions)) {
			// just for the sake of floating-point imprecision.
			double t = intersectMaze(direction, &intersects) + 1e-6;
			if (intersects) {
				for (iter_currBlock = currBlock,
						iter_location = location,
						iter_offsets = offsets,
						iter_direction = direction;
						iter_direction < dir_end;
						++iter_currBlock, ++iter_location,
						++iter_offsets, ++iter_direction) {
					double loc = (*iter_location += (*iter_direction) * t);
					double currB = *iter_currBlock = std::floor(loc + 0.5);
					*iter_offsets = loc - currB;
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
				double minStep = steps[0] * (0.5 - signs[0] * offsets[0]);
				double currStep;
				for (size_t i = 1; i < numDims; i++) {
					currStep = steps[i] * (0.5 - signs[i] * offsets[i]);
					if (currStep < minStep) {
						//minStepInd = i;
						minStep = currStep;
					}
				}
				// paranoia (it actually doesn't work without this)
				minStep += 1e-6;
				// we can safely assume that we don't have a parallel plane selected
				for (iter_currBlock = currBlock,
						iter_location = location,
						iter_offsets = offsets,
						iter_direction = direction;
						iter_direction < dir_end;
						++iter_currBlock, ++iter_location,
						++iter_offsets, ++iter_direction) {
					double loc = (*iter_location += (*iter_direction) * minStep);
					double currB = *iter_currBlock = std::floor(loc + 0.5);
					*iter_offsets = loc - currB;
				}
			}
		}

		*output = result.r; ++output;
		*output = result.g; ++output;
		*output = result.b; ++output;
		*output = result.a;
	}

};

} // maze

} // labyrinth_core

#endif /* INCLUDE_LABYRINTH_CORE_MAZE_MAZE_KERNEL_HPP_ */
