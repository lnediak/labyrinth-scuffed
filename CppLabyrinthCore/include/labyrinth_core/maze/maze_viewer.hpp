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

	/**
	 * Outputs (into output) the projection of vec onto the space spanned by
	 * basisVec1 and basisVec2, assuming they are an orthonormal basis.
	 */
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
		double aspect;

		friend MazeViewer;

	public:
		Slice(size_t inNumDims, size_t width, size_t height):
					numDims(inNumDims), width(width),
					height(height) {
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
			aspect = static_cast<double>(width) / height;
		}

		Slice(const Slice& other): numDims(other.numDims),
				width(other.width), height(other.height),
				aspect(other.aspect) {
			forward = new double[numDims];
			std::copy(other.forward, other.forward + numDims, forward);
			right = new double[numDims];
			std::copy(other.right, other.right + numDims, right);
			up = new double[numDims];
			std::copy(other.up, other.up + numDims, up);
		}

		Slice(Slice&& other): numDims(other.numDims),
				width(other.width), height(other.height),
				aspect(other.aspect) {
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
			aspect = other.aspect;
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
			aspect = other.aspect;
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

		bool setAspect(double newAspect) {
			if ((newAspect < 1e-4) || (newAspect > 1e+4)) {
				return false;
			}
			aspect = newAspect;
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
		size_t numThreads;
		double fov;

		friend MazeViewer;

	public:
		explicit ViewerOptions(size_t numDims): numDims(numDims),
				numThreads(8), fov(100) {}

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

		bool setSliceAspect(size_t index, double aspect) {
			if (index >= slices.size()) {
				return false;
			}
			if (!slices[index].setAspect(aspect)) {
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

		bool toggleRotatBinding(size_t from, size_t to) {
			if ((from >= slices.size()) || (to >= slices.size()) ||
					(from == to)) {
				return false;
			}
			switch(rotatBindings[from][to]) {
			case RotationalBinding::NONE:
				rotatBindings[from][to] = RotationalBinding::MIMIC;
				break;
			case RotationalBinding::MIMIC:
				rotatBindings[from][to] = RotationalBinding::MATRIX;
				break;
			case RotationalBinding::MATRIX:
				rotatBindings[from][to] = RotationalBinding::NONE;
				break;
			}
			return true;
		}

		bool setNumThreads(size_t newNumThreads) {
			if ((newNumThreads < 1) || (newNumThreads > 1000)) {
				return false;
			}
			numThreads = newNumThreads;
			return true;
		}

		double getFov() const {
			return fov;
		}

		bool setFov(double newFov) {
			if ((newFov < 1) || (newFov > 179)) {
				return false;
			}
			fov = newFov;
			return true;
		}

	};

private:
	ViewerOptions options;
	std::vector<std::uint8_t*> outputs;
	size_t currSlice;

public:
	MazeViewer(const Maze& maze, const ViewerOptions& inOptions,
			const double* inCamera): maze(maze, 0),
					renderer(maze, inCamera, inOptions.getFov()),
					options(inOptions) {
		if (maze.getNumDims() != options.numDims) {
			options = ViewerOptions(maze.getNumDims());
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
		maze.invalidate();
		delete[] camera;
		for (std::uint8_t* output: outputs) {
			delete[] output;
		}
	}

	size_t getNumDims() const {
		return maze.getNumDims();
	}

	size_t getNumSlices() const {
		return options.slices.size();
	}

	std::vector<std::pair<size_t, size_t>> getSliceSizes() const {
		std::vector<std::pair<size_t, size_t>> toreturn;
		for (const Slice& slice: options.slices) {
			toreturn.push_back(std::make_pair(slice.width, slice.height));
		}
		return toreturn;
	}

	void setCamera(const double* inCamera) {
		std::copy(inCamera, inCamera + maze.getNumDims(), camera);
		renderer.setCamera(inCamera);
	}

	void setFov(double newFov) {
		options.setFov(newFov);
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

	void setSliceAspect(size_t index, double aspect) {
		options.setSliceAspect(index, aspect);
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

	void toggleRotatBinding(size_t from, size_t to) {
		options.toggleRotatBinding(from, to);
	}

	void setCurrSlice(size_t index) {
		if (index >= options.slices.size()) {
			return;
		}
		currSlice = index;
	}

private:
	void move(const double* direction, double amount) {
		MazeKernel kernel (maze, camera);
		std::uint8_t output[4];
		MazeKernel::Result result =
				kernel(output, direction, Color{0, 0, 0, 0xFF});
		double t = result.t - 1e-3;
		if ((t >= amount) || result.block.empty()) {
			const double* direction_end = direction + options.numDims;
			double* iter_camera = camera;
			for (; direction < direction_end; ++direction, ++iter_camera) {
				*iter_camera += amount * (*direction);
			}
		} else {
			const double* direction_end = direction + options.numDims;
			double* iter_camera = camera;
			for (; direction < direction_end; ++direction, ++iter_camera) {
				*iter_camera += t * (*direction);
			}
		}
		renderer.setCamera(camera);
	}

public:
	void moveForward(double amount) {
		if (currSlice >= options.slices.size()) {
			return;
		}
		move(options.slices[currSlice].forward, amount);
	}

	void moveUp(double amount) {
		if (currSlice >= options.slices.size()) {
			return;
		}
		move(options.slices[currSlice].up, amount);
	}

	void moveRight(double amount) {
		if (currSlice >= options.slices.size()) {
			return;
		}
		move(options.slices[currSlice].right, amount);
	}

	void moveDown(double amount) {
		if (currSlice >= options.slices.size()) {
			return;
		}
		double* direction = new double[options.numDims];
		for (size_t i = 0; i < options.numDims; i++) {
			direction[i] = -options.slices[currSlice].up[i];
		}
		move(direction, amount);
		delete[] direction;
	}

	void moveLeft(double amount) {
		if (currSlice >= options.slices.size()) {
			return;
		}
		double* direction = new double[options.numDims];
		for (size_t i = 0; i < options.numDims; i++) {
			direction[i] = -options.slices[currSlice].right[i];
		}
		move(direction, amount);
		delete[] direction;
	}

	void moveBackwards(double amount) {
		if (currSlice >= options.slices.size()) {
			return;
		}
		double* direction = new double[options.numDims];
		for (size_t i = 0; i < options.numDims; i++) {
			direction[i] = -options.slices[currSlice].forward[i];
		}
		move(direction, amount);
		delete[] direction;
	}

private:
	enum class Dir {
		FORWARD, RIGHT, UP
	};

	double* getDir(size_t slice, Dir dir) {
		switch(dir) {
		case Dir::FORWARD:
			return options.slices[slice].forward;
		case Dir::RIGHT:
			return options.slices[slice].right;
		case Dir::UP:
			return options.slices[slice].up;
		}
		// impossible though. Just to calm down Eclipse.
		return nullptr;
	}

	void rotateSliceMimic(size_t slice, Dir from, Dir to,
			double sinTheta, double cosTheta) {
		double* fromDir = getDir(slice, from);
		double* toDir = getDir(slice, to);
		double tmp1, tmp2;
		for (size_t i = 0; i < options.numDims; i++) {
			tmp1 = fromDir[i];
			tmp2 = toDir[i];
			fromDir[i] = tmp1 * cosTheta + tmp2 * sinTheta;
			toDir[i] = -tmp1 * sinTheta + tmp2 * cosTheta;
		}
	}

	void rotateVecByMatrix(double* vec, Dir from, Dir to,
			double sinTheta, double cosTheta) {
		double* tmp = new double[options.numDims];
		double* remain = new double[options.numDims];
		const double* fromDir = getDir(currSlice, from);
		const double* toDir = getDir(currSlice, to);
		projection(options.numDims, tmp, vec, fromDir, toDir);
		double fromComponent = 0, toComponent = 0;
		for (size_t i = 0; i < options.numDims; i++) {
			fromComponent += tmp[i] * fromDir[i];
			toComponent += tmp[i] * toDir[i];
			remain[i] = vec[i] - tmp[i];
		}
		double newFromComponent = cosTheta * fromComponent +
				-sinTheta * toComponent;
		double newToComponent = sinTheta * fromComponent +
				cosTheta * toComponent;
		for (size_t i = 0; i < options.numDims; i++) {
			vec[i] = remain[i] + newFromComponent * fromDir[i] +
					newToComponent * toDir[i];
		}
		delete[] remain;
		delete[] tmp;
	}

	void rotateSliceMatrix(size_t slice, Dir from, Dir to,
			double sinTheta, double cosTheta) {
		rotateVecByMatrix(options.slices[slice].forward,
				from, to, sinTheta, cosTheta);
		rotateVecByMatrix(options.slices[slice].right,
				from, to, sinTheta, cosTheta);
		rotateVecByMatrix(options.slices[slice].up,
				from, to, sinTheta, cosTheta);
	}

	void gramSchmidtSlice(size_t slice) {
		double* forward = options.slices[slice].forward;
		double* right = options.slices[slice].right;
		double* up = options.slices[slice].up;
		double proj_u1_v2 = 0;
		double mag_forward = 0;
		for (size_t i = 0; i < options.numDims; i++) {
			proj_u1_v2 += forward[i] * right[i];
			mag_forward += forward[i] * forward[i];
		}
		proj_u1_v2 /= mag_forward;
		for (size_t i = 0; i < options.numDims; i++) {
			right[i] -= proj_u1_v2 * forward[i];
		}
		double btvecx = 0, btvecy = 0;
		double mag_right = 0;
		for (size_t i = 0; i < options.numDims; i++) {
			btvecx += forward[i] * up[i];
			btvecy += right[i] * up[i];
			mag_right += right[i] * right[i];
		}
		btvecx /= mag_forward;
		btvecy /= mag_right;
		double mag_up = 0;
		for (size_t i = 0; i < options.numDims; i++) {
			up[i] -= forward[i] * btvecx + right[i] * btvecy;
			mag_up += up[i] * up[i];
		}
		mag_forward = std::sqrt(mag_forward);
		mag_right = std::sqrt(mag_right);
		mag_up = std::sqrt(mag_up);
		for (size_t i = 0; i < options.numDims; i++) {
			forward[i] /= mag_forward;
			right[i] /= mag_right;
			up[i] /= mag_up;
		}
	}

	void rotate(Dir from, Dir to, double theta) {
		if (currSlice >= options.slices.size()) {
			return;
		}
		// 0.0174532925199432957692 is PI / 180
		double radTheta = theta * 0.0174532925199432957692;
		double sinTheta = std::sin(radTheta);
		double cosTheta = std::cos(radTheta);
		for (size_t i = 0; i < options.slices.size(); i++) {
			if (i == currSlice) {
				continue;
			}
			switch(options.rotatBindings[currSlice][i]) {
			case RotationalBinding::NONE:
				break;
			case RotationalBinding::MIMIC:
				rotateSliceMimic(i, from, to, sinTheta, cosTheta);
				break;
			case RotationalBinding::MATRIX:
				rotateSliceMatrix(i, from, to, sinTheta, cosTheta);
				break;
			}
			gramSchmidtSlice(i);
		}
		rotateSliceMimic(currSlice, from, to, sinTheta, cosTheta);
		gramSchmidtSlice(currSlice);
	}

public:
	void rotateUp(double theta) {
		rotate(Dir::FORWARD, Dir::UP, theta);
	}

	void rotateRight(double theta) {
		rotate(Dir::FORWARD, Dir::RIGHT, theta);
	}

	void rotateDown(double theta) {
		rotate(Dir::UP, Dir::FORWARD, theta);
	}

	void rotateLeft(double theta) {
		rotate(Dir::RIGHT, Dir::FORWARD, theta);
	}

	void rotateClockwise(double theta) {
		rotate(Dir::RIGHT, Dir::UP, theta);
	}

	void rotateCounterClockwise(double theta) {
		rotate(Dir::UP, Dir::RIGHT, theta);
	}


	/**
	 * Don't free these pointers. Okay?
	 * They will be all freed in the destructor.
	 */
	std::vector<std::uint8_t*> render() const {
		for (size_t i = 0; i < options.slices.size(); i++) {
			renderer.render(outputs[i],
					options.slices[i].forward,
					options.slices[i].right,
					options.slices[i].up,
					options.slices[i].width,
					options.slices[i].height,
					options.slices[i].aspect,
					options.fov);
		}
		renderer.waitForFinished();
		return outputs;
	}

};

} // maze

} // labyrinth_core

#endif /* INCLUDE_LABYRINTH_CORE_MAZE_MAZE_VIEWER_HPP_ */
