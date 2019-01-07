#ifndef INCLUDE_LABYRINTH_CORE_MAZE_MAZE_RENDERER_HPP_
#define INCLUDE_LABYRINTH_CORE_MAZE_MAZE_RENDERER_HPP_

#include <labyrinth_core/concurrent_queue.hpp>
#include <labyrinth_core/maze/maze_kernel.hpp>
#include <labyrinth_core/num_threads.hpp>

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include <cmath>

namespace labyrinth_core {

namespace maze {

class MazeRenderer {

	Maze maze;
	double* camera;

	std::vector<std::thread> threads;

	struct Task {

		bool isDeath;
		std::uint8_t* output;
		const double* forward;
		const double* right;
		const double* up;
		size_t width;
		size_t height;
		// this task is all rows for which row % numThreads = modulo
		size_t modulo;
		double xscale;
		double yscale;
		Color backgroundColor;

	};

	mutable size_t taskCount;
	mutable std::mutex taskMutex;
	mutable std::condition_variable taskVar;
	mutable multithread::ConcurrentQueue<Task> taskQueue;

public:
	MazeRenderer(const Maze& inMaze, const double* inCamera): maze(inMaze, 0) {
		size_t numThreads = multithread::getNumThreads();
		size_t numDims = maze.getNumDims();
		camera = new double[numDims];
		setCamera(inCamera);
		taskCount = 0;
		for (size_t i = 0; i < numThreads; i++) {
			threads.push_back(std::thread(
					[this, numThreads, numDims, i] () -> void {
				MazeKernel kernel (maze, camera);
				double* tmpdirection = new double[numDims];
				double* tmpdirection_end = tmpdirection + numDims;
				double* iter_tmpdir;
				while (true) {
					Task task = taskQueue.pop();
					if (task.isDeath) {
						delete[] tmpdirection;
						break;
					}
					kernel.setCamera(camera);
					size_t width = task.width;
					size_t height = task.height;
					double xscale = task.xscale;
					double yscale = task.yscale;
					double magnitude;
					double rcomponent, ucomponent;
					const double* forward = task.forward;
					const double* iter_forward;
					const double* right = task.right;
					const double* iter_right;
					const double* up = task.up;
					const double* iter_up;
					std::uint8_t* output_iter =
							task.output + task.modulo * width * 4;
					for (size_t row = task.modulo;
							row < task.height; row += numThreads) {
						for (size_t col = 0; col < task.width; col++) {
							magnitude = 0;
							rcomponent = (col - width/2.0) * xscale;
							ucomponent = (height/2.0 - row) * yscale;
							iter_tmpdir = tmpdirection;
							iter_forward = forward;
							iter_right = right;
							iter_up = up;
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

							kernel(output_iter, tmpdirection, task.backgroundColor);
							output_iter += 4;
						}
						output_iter += width * (numThreads - 1) * 4;
					}
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
		for (size_t i = 0; i < threads.size(); i++) {
			taskQueue.push(Task{
				true, nullptr, nullptr, nullptr, nullptr,
				0, 0, 0, 0.0, 0.0, Color{0, 0, 0, 0}
			});
		}
		for (std::thread& thread: threads) {
			thread.join();
		}
		delete[] camera;
	}

	void setCamera(const double* newCamera) {
		std::copy(newCamera, newCamera + maze.getNumDims(), camera);
	}

	void waitForFinished() const {
		std::unique_lock<std::mutex> lock (taskMutex);
		taskVar.wait(lock, [this] () -> bool {
			return taskCount == 0;
		});
	}

	void render(std::uint8_t* output, const double* forward,
			const double* right, const double* up,
			size_t width, size_t height, double aspect, double fov) const {
		// 0.00872664625997164788462 = 0.5 * PI / 180
		double tanFov = std::tan(fov * 0.00872664625997164788462);
		double xscale, yscale;
		if (aspect > 1) {
			xscale = 2 * tanFov / width;
			yscale = 2 * tanFov / (height * aspect);
		} else {
			xscale = 2 * tanFov * aspect / width;
			yscale = 2 * tanFov / height;
		}

		Color backgroundColor {0, 0, 0, 0xFF};

		for (size_t i = 0; i < threads.size(); i++) {
			std::lock_guard<std::mutex> lock (taskMutex);
			taskCount++;
			taskQueue.push(Task{
				false, output, forward, right, up,
				width, height, i, xscale, yscale, backgroundColor
			});
		}
	}

};

} // maze

} // labyrinth_core

#endif /* INCLUDE_LABYRINTH_CORE_MAZE_MAZE_RENDERER_HPP_ */
