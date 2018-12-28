#ifndef SRC_LABYRINTH_CORE_MAZE_MAZE_RENDERER_HPP_
#define SRC_LABYRINTH_CORE_MAZE_MAZE_RENDERER_HPP_

#include <labyrinth_core/maze/maze_kernel.hpp>

#include <cmath>

namespace labyrinth_core {

namespace maze {

class MazeRenderer {

public:
	static void render(std::uint8_t* output, const Maze& maze,
			const double* camera, const double* forward,
			const double* right, const double* up,
			size_t width, size_t height, double fov) {
		// 0.00872664625997164788462 = 0.5 * PI / 180
		double tanFov = std::tan(fov * 0.00872664625997164788462);
		double scale = (height > width)? 2 * tanFov / height : 2 * tanFov / width;

		MazeKernel kernel (maze, camera);
		size_t numDims = maze.getNumDims();
		Color tmpcolor;
		Color backgroundColor {0, 0, 0, 0xFF};
		double* tmpdirection = new double[numDims];
		double* tmpdirection_end = tmpdirection + numDims;

		double *iter_tmpdir;
		const double *iter_forward, *iter_right, *iter_up;
		std::uint8_t* output_iter = output;
		for (size_t row = 0; row < height; row++) {
			for (size_t col = 0; col < width; col++) {
				double rcomponent = (col - width/2.0) * scale;
				double ucomponent = (height/2.0 - row) * scale;
				// just a loop
				for (iter_tmpdir = tmpdirection, iter_forward = forward,
						iter_right = right, iter_up = up;
						iter_tmpdir < tmpdirection_end;
						++iter_tmpdir, ++iter_forward,
						++iter_right, ++iter_up) {
					*iter_tmpdir = (*iter_forward) +
							rcomponent * (*iter_right) +
							ucomponent * (*iter_up);
				}

				tmpcolor = kernel(tmpdirection, backgroundColor);
				*output_iter = tmpcolor.r; ++output_iter;
				*output_iter = tmpcolor.g; ++output_iter;
				*output_iter = tmpcolor.b; ++output_iter;
				*output_iter = tmpcolor.a; ++output_iter;
			}
		}
		delete[] tmpdirection;
	}

};

}

}

#endif /* SRC_LABYRINTH_CORE_MAZE_MAZE_RENDERER_HPP_ */
