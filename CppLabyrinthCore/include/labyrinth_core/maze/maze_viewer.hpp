#ifndef INCLUDE_LABYRINTH_CORE_MAZE_MAZE_VIEWER_HPP_
#define INCLUDE_LABYRINTH_CORE_MAZE_MAZE_VIEWER_HPP_

#include <labyrinth_core/maze/maze_renderer.hpp>

#include <algorithm>
#include <utility>
#include <vector>

#include <cmath>

namespace labyrinth_core {

namespace maze {

class MazeViewer {

	Maze maze;
	MazeRenderer renderer;
	double* camera;

	static void projection(size_t numDims, double* output, const double* vec,
			const double* basisVec1, const double* basisVec2) {
		// the projection is B(B^T B)^-1 B^T vec.
		// since basisVec1 is orthogonal to basisVec2, B^T B is I_2.
		// B^T vec should be [basisVec1 dot vec, basisVec2 dot vec]^T
		// then the projection is easy to get.
		double btvecx = 0, btvecy = 0;
		for (size_t i = 0; i < numDims; i++) {
			btvecx += basisVec1[i] * vec[i];
			btvecy += basisVec2[i] * vec[i];
		}
		for (size_t i = 0; i < numDims; i++) {
			output[i] = basisVec1[i] * btvecx +
					basisVec2[i] * btvecy;
		}
	}

	/**
	 * Makes vec orthogonal to basisVec1 and basisVec2.
	 * Assumes that basisVec1 and basisVec2 are orthogonal already.
	 */
	static void makeOrthonormal(size_t numDims, double* vec,
			const double* basisVec1, const double* basisVec2) {
		// Then vec - projection is it after normalization.
		// If it is too close to 0, offset vec by a bit randomly
		// and it should work then.
		double* tmp = new double[numDims];
		double magnitude;
		while (true) {
			projection(numDims, tmp, vec, basisVec1, basisVec2);
			magnitude = 0;
			for (size_t i = 0; i < numDims; i++) {
				tmp[i] = vec[i] - tmp[i];
				magnitude += tmp[i] * tmp[i];
			}
			magnitude = std::sqrt(magnitude);
			if (magnitude >= 1e-3) {
				break;
			}
			for (size_t i = 0; i < numDims; i++) {
				vec[i] += 1e-3;
			}
		}
		for (size_t i = 0; i < numDims; i++) {
			vec[i] = tmp[i] / magnitude;
		}
		delete[] tmp;
	}

public:
	class Slice {

		size_t numDims;
		double* forward;
		double* right;
		double* up;
		size_t width;
		size_t height;

	public:
		Slice(size_t inNumDims, size_t width, size_t height):
					numDims(inNumDims), width(width), height(height) {
			if (numDims < 3) {
				numDims = 3;
			}
			forward = new double[numDims];
			right = new double[numDims];
			up = new double[numDims];
			for (size_t i = 0; i < numDims; i++) {
				forward[i] = 0;
				right[i] = 0;
				up[i] = 0;
			}
			forward[0] = 1;
			right[1] = 1;
			up[2] = 1;
		}

		Slice(const Slice& other): numDims(other.numDims),
				width(other.width), height(other.height) {
			forward = new double[numDims];
			std::copy(other.forward, other.forward + numDims, forward);
			right = new double[numDims];
			std::copy(other.right, other.right + numDims, right);
			up = new double[numDims];
			std::copy(other.up, other.up + numDims, up);
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
			std::copy(other.forward, other.forward + numDims, forward);
			std::copy(other.right, other.right + numDims, right);
			std::copy(other.up, other.up + numDims, up);
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
			makeOrthonormal(numDims, forward, right, up);
		}

		void setRight(const double* inRight) {
			std::copy(inRight, inRight + numDims, right);
			makeOrthonormal(numDims, right, forward, up);
		}

		void setUp(const double* inUp) {
			std::copy(inUp, inUp + numDims, up);
			makeOrthonormal(numDims, up, forward, right);
		}

		bool setWidth(size_t newWidth) {
			if (newWidth < 10) {
				return false;
			}
			width = newWidth;
			return true;
		}

		bool setHeight(size_t newHeight) {
			if (newHeight < 10) {
				return false;
			}
			height = newHeight;
			return true;
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

		size_t numDims;
		std::vector<Slice> slices;
		// rotatBindings[i][j] specifies how rotating in slice i affects slice j.
		std::vector<std::vector<RotationalBinding>> rotatBindings;
		double fov;

	public:
		explicit ViewerOptions(size_t numDims,
				double fov = 90): numDims(numDims), fov(fov) {}

		bool addSlice(const Slice& slice) {
			if (slice.numDims != numDims) {
				return false;
			}
			slices.push_back(slice);
			size_t lastIndex = slices.size() - 1;
			for (size_t i = 0; i < lastIndex; i++) {
				rotatBindings[i].push_back(RotationalBinding::MATRIX);
			}
			rotatBindings.push_back(std::vector<RotationalBinding>(
					lastIndex + 1, RotationalBinding::MATRIX));
			return true;
		}

		bool resizeSlice(size_t index, size_t width, size_t height) {
			if (index >= slices.size()) {
				return false;
			}
			if (!slices[index].setWidth(width)) {
				return false;
			}
			if (!slices[index].setHeight(height)) {
				return false;
			}
			return true;
		}

		bool deleteSlice(size_t index) {
			if (index >= slices.size()) {
				return false;
			}
			slices.erase(slices.begin() + index);
			rotatBindings.erase(rotatBindings.begin() + index);
			for (size_t i = 0; i < rotatBindings.size(); i++) {
				rotatBindings[i].erase(rotatBindings[i].begin() + index);
			}
			return true;
		}

		/**
		 * Sets the rotational binding specifying how from is to affect to.
		 */
		bool setRotatBinding(size_t from, size_t to, RotationalBinding binding) {
			if ((from >= slices.size()) || (to >= slices.size()) ||
					(from == to)) {
				return false;
			}
			rotatBindings[from][to] = binding;
			return true;
		}

	};

private:
	ViewerOptions options;
	std::vector<std::uint8_t*> outputs;
	size_t currSlice;

	MazeViewer(const Maze& maze, const ViewerOptions& inOptions,
			const double* inCamera): maze(maze, 0),
					renderer(maze, inCamera),
					options(inOptions) {
		if (maze.getNumDims() != options.numDims) {
			options = ViewerOptions();
		}
		camera = new double[options.numDims];
		setCamera(inCamera);
		for (size_t i = 0; i < options.slices.size(); i++) {
			outputs.push_back(
					new std::uint8_t
					[options.slices[i].width *
					 options.slices[i].height * 4]);
		}
		currSlice = 0;
	}

	MazeViewer(MazeViewer& other) = delete;
	MazeViewer(const MazeViewer& other) = delete;
	MazeViewer(MazeViewer&& other) = delete;
	MazeViewer& operator=(MazeViewer& other) = delete;
	MazeViewer& operator=(const MazeViewer& other) = delete;
	MazeViewer& operator=(MazeViewer&& other) = delete;

	~MazeViewer() {
		delete[] camera;
		for (std::uint8_t* output: outputs) {
			delete[] output;
		}
	}

	void setCamera(const double* inCamera) {
		std::copy(inCamera, inCamera + maze.getNumDims(), camera);
		renderer.setCamera(inCamera);
	}

	void addSlice(const Slice& slice) {
		if (!options.addSlice(slice)) {
			return;
		}
		outputs.push_back(new std::uint8_t[slice.width * slice.height * 4]);
	}

	void resizeSlice(size_t index, size_t width, size_t height) {
		if (!options.resizeSlice(index, width, height)) {
			return;
		}
		delete[] outputs[index];
		outputs[index] = new std::uint8_t[width * height * 4];
	}

	void deleteSlice(size_t index) {
		if (!options.deleteSlice(index)) {
			return;
		}
		delete[] outputs[index];
		outputs.erase(outputs.begin() + index);
	}

	void setRotatBinding(size_t from, size_t to, RotationalBinding binding) {
		options.setRotatBinding(from, to, binding);
	}

	void setCurrSlice(size_t index) {
		if (index >= options.slices.size()) {
			return;
		}
		currSlice = index;
	}

private:
	enum class Dir {
		FORWARD, RIGHT, UP
	};

	void rotate(Dir from, Dir to, double theta) {
		if (currSlice >= options.slices.size()) {
			return;
		}
		// 0.0174532925199432957692 is PI / 180
		double radTheta = theta * 0.0174532925199432957692;
		double sinTheta = std::sin(radTheta);
		double cosTheta = std::cos(radTheta);
		//
		for (size_t i = 0; i < options.numDims; i++) {

		}
	}

public:


	/**
	 * Don't free these pointers. Okay?
	 * They will be all freed in the destructor.
	 */
	std::vector<std::uint8_t*> render() {
		for (size_t i = 0; i < options.slices.size(); i++) {
			renderer.render(outputs[i],
					options.slices[i].forward,
					options.slices[i].right,
					options.slices[i].up,
					options.slices[i].width,
					options.slices[i].height,
					options.fov);
		}
		renderer.waitForFinished();
		return outputs;
	}

};

} // maze

} // labyrinth_core

#endif /* INCLUDE_LABYRINTH_CORE_MAZE_MAZE_VIEWER_HPP_ */
