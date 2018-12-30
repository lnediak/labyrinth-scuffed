#ifndef SRC_LABYRINTH_CORE_MAZE_MAZE_RENDERER_HPP_
#define SRC_LABYRINTH_CORE_MAZE_MAZE_RENDERER_HPP_

#include <labyrinth_core/concurrent_queue.hpp>
#include <labyrinth_core/num_threads.hpp>
#include <labyrinth_core/maze/maze_kernel.hpp>

#include <thread>
#include <vector>

#include <cmath>

namespace labyrinth_core {

namespace maze {

class MazeRenderer {

	Maze maze;
	MazeKernel kernel;

	std::vector<std::thread> threads;
	// each thread has its own
	std::vector<double*> tmpdirs;

	struct Task {

		bool isDeath;
		std::uint8_t* output;
		const double* forward;
		const double* right;
		const double* up;
		double rcomponent;
		double ucomponent;
		Color backgroundColor;

	};

	mutable size_t taskCount;
	mutable std::mutex taskMutex;
	mutable std::condition_variable taskVar;
	mutable multithread::ConcurrentQueue<Task> taskQueue;

public:
	MazeRenderer(const Maze& maze, const double* camera):
		maze(maze, 0), kernel(maze, camera) {
		size_t numThreads = multithread::getNumThreads();
		size_t numDims = maze.getNumDims();
		taskCount = 0;
		for (size_t i = 0; i < numThreads; i++) {
			tmpdirs.push_back(new double[numDims]);
			threads.push_back(std::thread([this, numDims, i] () -> void {
				while (true) {
					Task task = taskQueue.pop();
					if (task.isDeath) {
						break;
					}
					double magnitude = 0;
					double rcomponent = task.rcomponent;
					double ucomponent = task.ucomponent;
					double* tmpdirection = tmpdirs[i];
					double* tmpdirection_end = tmpdirection + numDims;
					double* iter_tmpdir = tmpdirection;
					const double* iter_forward = task.forward;
					const double* iter_right = task.right;
					const double* iter_up = task.up;
					// just a loop
					for (iter_tmpdir = tmpdirection;
							iter_tmpdir < tmpdirection_end;
							++iter_tmpdir, ++iter_forward,
							++iter_right, ++iter_up) {
						double value = *iter_tmpdir = (*iter_forward) +
								rcomponent * (*iter_right) +
								ucomponent * (*iter_up);
						magnitude += value * value;
					}
					magnitude = std::sqrt(magnitude);
					for (iter_tmpdir = tmpdirection;
							iter_tmpdir < tmpdirection_end; ++iter_tmpdir) {
						*iter_tmpdir /= magnitude;
					}

					kernel(task.output, tmpdirection, task.backgroundColor);
					std::unique_lock<std::mutex> lock (taskMutex);
					taskCount--;
					lock.unlock();
					if (taskCount == 0) taskVar.notify_one();
				}
			}));
		}
	}

	MazeRenderer(MazeRenderer& other) = delete;
	MazeRenderer(const MazeRenderer& other) = delete;
	MazeRenderer(MazeRenderer&& other) = delete;
	MazeRenderer& operator=(MazeRenderer& other) = delete;
	MazeRenderer& operator=(const MazeRenderer& other) = delete;
	MazeRenderer& operator=(MazeRenderer&& other) = delete;

	~MazeRenderer() {
		maze.invalidate();
		for (double* tmpdir: tmpdirs) {
			delete[] tmpdir;
		}
		for (size_t i = 0; i < threads.size(); i++) {
			taskQueue.push(Task{
				true, nullptr, nullptr, nullptr, nullptr,
				0.0, 0.0, Color{0, 0, 0, 0}
			});
		}
	}

	void setCamera(const double* newCamera) {
		kernel.setCamera(newCamera);
	}

	void render(std::uint8_t* output, const double* forward,
			const double* right, const double* up,
			size_t width, size_t height, double fov) const {
		// 0.00872664625997164788462 = 0.5 * PI / 180
		double tanFov = std::tan(fov * 0.00872664625997164788462);
		double scale = (height > width)? 2 * tanFov / height: 2 * tanFov / width;

		Color backgroundColor {0, 0, 0, 0xFF};

		std::uint8_t* output_iter = output;
		taskCount = width * height;
		for (size_t row = 0; row < height; row++) {
			for (size_t col = 0; col < width; col++, output_iter += 4) {
				double rcomponent = (col - width/2.0) * scale;
				double ucomponent = (height/2.0 - row) * scale;
				taskQueue.push(Task{
					false, output_iter, forward, right, up,
					rcomponent, ucomponent, backgroundColor
				});
			}
		}
		std::unique_lock<std::mutex> lock (taskMutex);
		taskVar.wait(lock, [this] () -> bool {
			return taskCount == 0;
		});
	}

};

} // maze

} // labyrinth_core

#endif /* SRC_LABYRINTH_CORE_MAZE_MAZE_RENDERER_HPP_ */
