#ifndef INCLUDE_LABYRINTH_DESKTOP_MAZE_MAZE_DISPLAY_HPP_
#define INCLUDE_LABYRINTH_DESKTOP_MAZE_MAZE_DISPLAY_HPP_

#include <GL/gl.h>

#include <labyrinth_core/maze/maze_viewer.hpp>
#include <labyrinth_desktop/controls_handler.hpp>
#include <labyrinth_desktop/displayable.hpp>

#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace labyrinth_desktop {

namespace maze {

class MazeDisplay: public Displayable {

public:
	class DisplayOptions;

	class MazeDisplayOperation {

	public:
		enum class Operation {

			MOVE_FORWARD,
			MOVE_UP,
			MOVE_RIGHT,
			MOVE_DOWN,
			MOVE_LEFT,
			MOVE_BACKWARDS,
			ROTATE_UP,
			ROTATE_RIGHT,
			ROTATE_DOWN,
			ROTATE_LEFT,
			ROTATE_CLOCKWISE,
			ROTATE_COUNTERCLOCKWISE,

			SWITCH_SLICE,
			TOGGLE_ROTAT_BINDING,
			SET_ROTAT_BINDING

		};

	private:
		Operation type;
		// these will only be used for
		// SWITCH_SLICE and *_ROTAT_BINDING
		size_t slice1;
		size_t slice2;
		labyrinth_core::maze::MazeViewer::RotationalBinding binding;

	public:
		MazeDisplayOperation(Operation type): type(type),
		slice1(0), slice2(0),
		binding(labyrinth_core::maze::MazeViewer::RotationalBinding::NONE) {}

		/**
		 * For switch_slice.
		 */
		MazeDisplayOperation(size_t slice):
			type(Operation::SWITCH_SLICE), slice1(slice), slice2(0),
			binding(labyrinth_core::maze::MazeViewer::RotationalBinding::NONE) {}

		/**
		 * For toggle_rotat_binding.
		 */
		MazeDisplayOperation(size_t from, size_t to):
			type(Operation::TOGGLE_ROTAT_BINDING), slice1(from), slice2(to),
			binding(labyrinth_core::maze::MazeViewer::RotationalBinding::NONE) {}

		/**
		 * For set_rotat_binding.
		 */
		MazeDisplayOperation(size_t from, size_t to,
				labyrinth_core::maze::MazeViewer::RotationalBinding bind):
			type(Operation::SET_ROTAT_BINDING), slice1(from), slice2(to),
			binding(bind) {}

		Operation getType() const {
			return type;
		}

		void operate(MazeDisplay* display, const DisplayOptions& options) const;

	};

	static bool compatible(MazeDisplayOperation::Operation first,
			MazeDisplayOperation::Operation second,
			const DisplayOptions& options);

	class DisplayOptions: public ControlsHandler<MazeDisplayOperation> {

	public:
		enum class Squareness {

			SMOOTH, JUST_ROTATION, SQUARE

		};

	private:
		// number of blocks to move in 1 second
		double velocity;
		// angle to turn in 1 second
		double rotatSensitivity;
		// the locations and rectangles of the slices in the window
		// format {l, u, r, b}
		std::vector<std::tuple<double, double, double, double>> sliceLocs;
		Squareness squareness;

	public:
		void operate(MazeDisplay* display,
				const std::unordered_set<int>& keys) const;

		double getVelocity() const {
			return velocity;
		}

		void setVelocity(double newVelocity) {
			if (newVelocity > 0) {
				velocity = newVelocity;
			}
		}

		double getRotatSensitivity() const {
			return rotatSensitivity;
		}

		void setRotatSensitivity(double newRotatSensitivity) {
			if (newRotatSensitivity > 0) {
				rotatSensitivity = newRotatSensitivity;
			}
		}

		size_t getNumSlices() const {
			return sliceLocs.size();
		}

		decltype(sliceLocs) getSliceLocs() const {
			return sliceLocs;
		}

		// the locations and rectangles of the slices in the window
		// format {l, u, r, b}
		void setSliceLocs(decltype(sliceLocs) inSliceLocs) {
			sliceLocs = inSliceLocs;
		}

		Squareness getSquareness() const {
			return squareness;
		}

		void setSquareness(Squareness newSquareness) {
			squareness = newSquareness;
		}

	};

private:
	labyrinth_core::maze::Maze maze;
	labyrinth_core::maze::MazeViewer viewer;
	DisplayOptions options;
	GLuint texture;
	GLuint buf;
	GLuint program;
	GLuint posLoc;
	GLuint texcLoc;
	std::string errMsg;
	double* camera;

	double mouseX, mouseY;
	std::unordered_set<int> keyboardState;
	// this can also be the last time that the keyboard state was changed.
	std::chrono::time_point<
	std::chrono::high_resolution_clock> lastOperate;

	void setAspect(size_t width, size_t height) {
		auto sliceLocs = options.getSliceLocs();
		for (size_t i = 0; i < options.getNumSlices(); i++) {
			double w = std::get<2>(sliceLocs[i]) -
					std::get<0>(sliceLocs[i]);
			double h = std::get<1>(sliceLocs[i]) -
					std::get<3>(sliceLocs[i]);
			viewer.setSliceAspect(i, w * width / (h * height));
		}
	}

public:
	// here width and height are the width and height of the window.
	MazeDisplay(labyrinth_core::maze::Maze&& inMaze,
			const double* inCamera,
			const DisplayOptions& displayOptions,
			const labyrinth_core::maze::MazeViewer::ViewerOptions& viewerOptions,
			size_t width, size_t height);

	MazeDisplay(MazeDisplay& other) = delete;
	MazeDisplay(const MazeDisplay& other) = delete;
	MazeDisplay(MazeDisplay&& other) = delete;
	MazeDisplay& operator=(MazeDisplay& other) = delete;
	MazeDisplay& operator=(const MazeDisplay& other) = delete;
	MazeDisplay& operator=(MazeDisplay&& other) = delete;

	~MazeDisplay();

	std::string getErrMsg() const {
		return errMsg;
	}

	void display() override;

	void onWindowResize(size_t width, size_t height) override;

	void onKeyPress(int key) override {
		keyboardState.insert(key);
	}

	void onKeyRelease(int key) override {
		keyboardState.erase(key);
	}

	void onCharType(int unicode_codepoint) override {}

	void onMouseMove(double x, double y) override {
		mouseX = x;
		mouseY = y;
	}

	void onMousePress(MouseButton button) override {
		// TODO: IMPLEMENT MOUSE CLICK
	}

	void onMouseRelease(MouseButton button) override {
		// bla look above
	}

	void onMouseScroll(double xoff, double yoff) override {}

};

}

}

#endif /* INCLUDE_LABYRINTH_DESKTOP_MAZE_MAZE_DISPLAY_HPP_ */
