#ifndef SRC_LABYRINTH_CORE_MAZE_MAZE_KERNEL_HPP_
#define SRC_LABYRINTH_CORE_MAZE_MAZE_KERNEL_HPP_

#include <labyrinth_core/color.hpp>
#include <labyrinth_core/maze/maze.hpp>

#include <algorithm>

namespace labyrinth_core {

namespace maze {

class MazeKernel {

	Maze maze;
	double* camera;

public:
	MazeKernel(const Maze& maze, double* inCamera): maze(maze, 0) {
		camera = new double[maze.getNumDims()];
		setCamera(inCamera);
	}

	MazeKernel(MazeKernel& other) = delete;
	MazeKernel(const MazeKernel& other) = delete;
	MazeKernel(MazeKernel&& other) = delete;
	MazeKernel& operator=(MazeKernel& other) = delete;
	MazeKernel& operator=(const MazeKernel& other) = delete;
	MazeKernel& operator=(MazeKernel&& other) = delete;

	~MazeKernel() {
		delete[] camera;
	}

	void setCamera(double* newCamera) {
		std::copy(newCamera, newCamera + maze.getNumDims(), camera);
	}

	Color operator()(double* direction, Color backgroundColor);

};

} // maze

} // labyrinth_core

#endif /* SRC_LABYRINTH_CORE_MAZE_MAZE_KERNEL_HPP_ */
