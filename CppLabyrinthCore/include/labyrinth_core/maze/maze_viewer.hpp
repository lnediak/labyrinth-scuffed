#ifndef INCLUDE_LABYRINTH_CORE_MAZE_MAZE_VIEWER_HPP_
#define INCLUDE_LABYRINTH_CORE_MAZE_MAZE_VIEWER_HPP_

#include <labyrinth_core/maze/maze_renderer.hpp>

#include <algorithm>
#include <utility>
#include <vector>

namespace labyrinth_core {

namespace maze {

class MazeViewer {

	Maze maze;
	MazeRenderer renderer;
	double* camera;

public:
	class Slice {

		size_t numDims;
		double* forward;
		double* right;
		double* up;
		size_t width;
		size_t height;

	public:
		Slice(size_t numDims, const double* inForward,
				const double* inRight, const double* inUp,
				size_t width, size_t height):
					numDims(numDims), width(width), height(height) {
			forward = new double[numDims];
			setForward(inForward);
			right = new double[numDims];
			setRight(inRight);
			up = new double[numDims];
			setUp(inUp);
		}

		Slice(const Slice& other): numDims(other.numDims),
				width(other.width), height(other.height) {
			forward = new double[numDims];
			setForward(other.forward);
			right = new double[numDims];
			setRight(other.right);
			up = new double[numDims];
			setUp(other.up);
		}

		Slice(Slice&& other): numDims(other.numDims),
				width(other.width), height(other.height) {
			forward = other.forward;
			other.forward = nullptr;
			right = other.right;
			other.right = nullptr;
			up = other.up;
			other.up = nullptr;
		}

		Slice& operator=(const Slice& other) {
			if (this == &other) {
				return *this;
			}
			if (numDims != other.numDims) {
				numDims = other.numDims;
				delete[] forward;
				forward = new double[numDims];
				delete[] right;
				right = new double[numDims];
				delete[] up;
				up = new double[numDims];
			}
			setForward(other.forward);
			other.forward = nullptr;
			setRight(other.right);
			other.right = nullptr;
			setUp(other.up);
			other.up = nullptr;
			width = other.width;
			height = other.height;
			return *this;
		}

		Slice& operator=(Slice&& other) {
			if (this == &other) {
				return *this;
			}
			numDims = other.numDims;
			delete[] forward;
			forward = other.forward;
			other.forward = nullptr;
			delete[] right;
			right = other.right;
			other.right = nullptr;
			delete[] up;
			up = other.up;
			other.up = nullptr;
			width = other.width;
			height = other.height;
			return *this;
		}

		void setForward(const double* inForward) {
			std::copy(inForward, inForward + numDims, forward);
		}

		void setRight(const double* inRight) {
			std::copy(inRight, inRight + numDims, right);
		}

		void setUp(const double* inUp) {
			std::copy(inUp, inUp + numDims, up);
		}

		~Slice() {
			delete[] forward;
			delete[] right;
			delete[] up;
		}

	};

	enum class RotationalBinding {

		NONE, MIMIC, MATRIX

	};

	class ViewerOptions {

		std::vector<std::tuple<Slice, size_t, size_t>> slices;
		std::vector<std::vector<RotationalBinding>> rotatBindings;
		double fov;

	public:
		explicit ViewerOptions(double fov = 90): fov(fov) {}

		void addSlice(const Slice& slice, size_t width, size_t height) {
			slices.push_back(std::make_tuple(slice, width, height));
			size_t lastIndex = slices.size() - 1;
			for (size_t i = 0; i < lastIndex; i++) {
				rotatBindings[i].push_back(RotationalBinding::MATRIX);
			}
			rotatBindings.push_back(std::vector<RotationalBinding>(
					lastIndex + 1, RotationalBinding::MATRIX));
		}

	};

private:
	ViewerOptions options;
	std::vector<std::uint8_t*> outputs;

	MazeViewer(const Maze& maze, std::vector<Slice> slices,
			const ViewerOptions& options,
			const double* inCamera): maze(maze, 0),
					renderer(maze, inCamera),
					options(options) {
		camera = new double[maze.getNumDims()];
		setCamera(inCamera);
	}

	MazeViewer(MazeViewer& other) = delete;
	MazeViewer(const MazeViewer& other) = delete;
	MazeViewer(MazeViewer&& other) = delete;
	MazeViewer& operator=(MazeViewer& other) = delete;
	MazeViewer& operator=(const MazeViewer& other) = delete;
	MazeViewer& operator=(MazeViewer&& other) = delete;

	~MazeViewer() {
		delete[] camera;
	}

	void setCamera(const double* inCamera) {
		std::copy(inCamera, inCamera + maze.getNumDims(), camera);
	}

	std::vector<std::uint8_t*> render(); // FIXME: IMPLEMENT NEXT IMMEDIATLY

};

} // maze

} // labyrinth_core

#endif /* INCLUDE_LABYRINTH_CORE_MAZE_MAZE_VIEWER_HPP_ */
