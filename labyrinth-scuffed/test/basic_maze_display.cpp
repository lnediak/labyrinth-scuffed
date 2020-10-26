#include <GL/glew.h>

#include <labyrinth_desktop/maze/maze_display.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <utility>

namespace {

labyrinth_desktop::maze::MazeDisplay* display = nullptr;

void errCallback(int id, const char* err) {
	std::cerr << err << std::endl;
}

void keyCallback(GLFWwindow*, int key, int, int action, int) {
	if (action == GLFW_PRESS) {
		display->onKeyPress(key);
	} else if (action == GLFW_RELEASE) {
		display->onKeyRelease(key);
	}
}

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
	display->onWindowResize(width, height);
}

}

int initDisplay3D(size_t width, size_t height) {
	labyrinth_core::maze::Maze::MazeGenerationOptions genOpts;
	genOpts.setDimensions({6, 6, 6});
	genOpts.setSeed("1");
	genOpts.setDensity(1);
	genOpts.setBranchProbability(0.05);
	genOpts.setBranchDeathProbability(0.01);
	genOpts.setTwistProbability(0.5);
	genOpts.setFlowProbability(0.7);
	genOpts.setRestrictNewAmount(1);
	genOpts.setLoopProbability(0);
	genOpts.setBlockProbability(0);
	genOpts.setMaxUseless(50000000);
	labyrinth_core::maze::Maze maze (genOpts);

	const double camera[] = {1, 1, 1};

	labyrinth_desktop::maze::MazeDisplay::DisplayOptions displayOpts;
	displayOpts.setVelocity(1.5);
	displayOpts.setRotatSensitivity(60);
	displayOpts.setSquareness(labyrinth_desktop::maze::
			MazeDisplay::DisplayOptions::Squareness::SMOOTH);
	displayOpts.setSliceLocs({
		{-1.0, 1.0, 1.0, -1.0}
	});
#define MACRO_ADD_CONTROL(key, ...) displayOpts.addControl({key}, \
	{labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation( \
			labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation:: \
			Operation::__VA_ARGS__)});
	MACRO_ADD_CONTROL(GLFW_KEY_W, MOVE_FORWARD)
	MACRO_ADD_CONTROL(GLFW_KEY_E, MOVE_UP)
	MACRO_ADD_CONTROL(GLFW_KEY_D, MOVE_RIGHT)
	MACRO_ADD_CONTROL(GLFW_KEY_Q, MOVE_DOWN);
	MACRO_ADD_CONTROL(GLFW_KEY_A, MOVE_LEFT);
	MACRO_ADD_CONTROL(GLFW_KEY_S, MOVE_BACKWARDS);
	MACRO_ADD_CONTROL(GLFW_KEY_I, ROTATE_UP);
	MACRO_ADD_CONTROL(GLFW_KEY_L, ROTATE_RIGHT);
	MACRO_ADD_CONTROL(GLFW_KEY_K, ROTATE_DOWN);
	MACRO_ADD_CONTROL(GLFW_KEY_J, ROTATE_LEFT);
	MACRO_ADD_CONTROL(GLFW_KEY_O, ROTATE_CLOCKWISE);
	MACRO_ADD_CONTROL(GLFW_KEY_U, ROTATE_COUNTERCLOCKWISE);
#undef MACRO_ADD_CONTROL
#define MACRO_ADD_CONTROL(key, ...) displayOpts.addControl({key}, \
	{labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation(__VA_ARGS__)});
	MACRO_ADD_CONTROL(GLFW_KEY_R, 0);
	MACRO_ADD_CONTROL(GLFW_KEY_F, 1);
	MACRO_ADD_CONTROL(GLFW_KEY_C, 2);
	MACRO_ADD_CONTROL(GLFW_KEY_X, 3);
	// rotational bindings later
#undef MACRO_ADD_CONTROL

	labyrinth_core::maze::MazeViewer::ViewerOptions viewerOpts (3);
	viewerOpts.setNumThreads(16);
	viewerOpts.setFov(110);
	const size_t w = 1080, h = 720;
	labyrinth_core::maze::MazeViewer::Slice slice1 (3, w, h);
	viewerOpts.addSlice(slice1);

	display = new labyrinth_desktop::maze::MazeDisplay(
			std::move(maze), camera, displayOpts, viewerOpts, width, height);
	return 0;
}

int initDisplay4D(size_t width, size_t height) {
	labyrinth_core::maze::Maze::MazeGenerationOptions genOpts;
	genOpts.setDimensions({5, 5, 5, 5});
	genOpts.setSeed("2");
	genOpts.setDensity(1);
	genOpts.setBranchProbability(0.05);
	genOpts.setBranchDeathProbability(0.01);
	genOpts.setTwistProbability(0.5);
	genOpts.setFlowProbability(0.7);
	genOpts.setRestrictNewAmount(1);
	genOpts.setLoopProbability(0);
	genOpts.setBlockProbability(0);
	genOpts.setMaxUseless(50000000);
	labyrinth_core::maze::Maze maze (genOpts);

	const double camera[] = {1, 1, 1, 1};

	labyrinth_desktop::maze::MazeDisplay::DisplayOptions displayOpts;
	displayOpts.setVelocity(1.5);
	displayOpts.setRotatSensitivity(60);
	displayOpts.setSquareness(labyrinth_desktop::maze::
			MazeDisplay::DisplayOptions::Squareness::SMOOTH);
	displayOpts.setSliceLocs({
		{-1.0,  1.0, -0.1,  0.1},
		{ 0.1,  1.0,  1.0,  0.1},
		{-1.0, -0.1, -0.1, -1.0},
		{ 0.1, -0.1,  1.0, -1.0}
	});
#define MACRO_ADD_CONTROL(key, ...) displayOpts.addControl({key}, \
	{labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation( \
			labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation:: \
			Operation::__VA_ARGS__)});
	MACRO_ADD_CONTROL(GLFW_KEY_W, MOVE_FORWARD)
	MACRO_ADD_CONTROL(GLFW_KEY_E, MOVE_UP)
	MACRO_ADD_CONTROL(GLFW_KEY_D, MOVE_RIGHT)
	MACRO_ADD_CONTROL(GLFW_KEY_Q, MOVE_DOWN);
	MACRO_ADD_CONTROL(GLFW_KEY_A, MOVE_LEFT);
	MACRO_ADD_CONTROL(GLFW_KEY_S, MOVE_BACKWARDS);
	MACRO_ADD_CONTROL(GLFW_KEY_I, ROTATE_UP);
	MACRO_ADD_CONTROL(GLFW_KEY_L, ROTATE_RIGHT);
	MACRO_ADD_CONTROL(GLFW_KEY_K, ROTATE_DOWN);
	MACRO_ADD_CONTROL(GLFW_KEY_J, ROTATE_LEFT);
	MACRO_ADD_CONTROL(GLFW_KEY_O, ROTATE_CLOCKWISE);
	MACRO_ADD_CONTROL(GLFW_KEY_U, ROTATE_COUNTERCLOCKWISE);
#undef MACRO_ADD_CONTROL
#define MACRO_ADD_CONTROL(key, ...) displayOpts.addControl({key}, \
	{labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation(__VA_ARGS__)});
	MACRO_ADD_CONTROL(GLFW_KEY_R, 0);
	MACRO_ADD_CONTROL(GLFW_KEY_F, 1);
	MACRO_ADD_CONTROL(GLFW_KEY_C, 2);
	MACRO_ADD_CONTROL(GLFW_KEY_X, 3);
	// rotational bindings later
#undef MACRO_ADD_CONTROL

	labyrinth_core::maze::MazeViewer::ViewerOptions viewerOpts (4);
	viewerOpts.setNumThreads(16);
	viewerOpts.setFov(110);
	const size_t w = 486, h = 324;
	labyrinth_core::maze::MazeViewer::Slice slice1 (4, w, h);
	viewerOpts.addSlice(slice1);
	labyrinth_core::maze::MazeViewer::Slice slice2 = slice1;
	const double secondDim[] = {0, 1, 0, 0};
	const double thirdDim[] = {0, 0, 1, 0};
	const double forthDim[] = {0, 0, 0, 1};
	slice2.setUp(forthDim);
	viewerOpts.addSlice(slice2);
	labyrinth_core::maze::MazeViewer::Slice slice3 = slice2;
	slice3.setRight(thirdDim);
	viewerOpts.addSlice(slice3);
	labyrinth_core::maze::MazeViewer::Slice slice4 = slice3;
	slice4.setForward(secondDim);
	viewerOpts.addSlice(slice4);

	display = new labyrinth_desktop::maze::MazeDisplay(
			std::move(maze), camera, displayOpts, viewerOpts, width, height);
	return 0;
}

int initDisplay5D(size_t width, size_t height) {
	labyrinth_core::maze::Maze::MazeGenerationOptions genOpts;
	genOpts.setDimensions({5, 5, 5, 5, 5});
	genOpts.setSeed("2");
	genOpts.setDensity(1);
	genOpts.setBranchProbability(0.05);
	genOpts.setBranchDeathProbability(0.01);
	genOpts.setTwistProbability(0.5);
	genOpts.setFlowProbability(0.7);
	genOpts.setRestrictNewAmount(1);
	genOpts.setLoopProbability(0);
	genOpts.setBlockProbability(0);
	genOpts.setMaxUseless(50000000);
	labyrinth_core::maze::Maze maze (genOpts);

	const double camera[] = {1, 1, 1, 1, 1};

	labyrinth_desktop::maze::MazeDisplay::DisplayOptions displayOpts;
	displayOpts.setVelocity(1.5);
	displayOpts.setRotatSensitivity(60);
	displayOpts.setSquareness(labyrinth_desktop::maze::
			MazeDisplay::DisplayOptions::Squareness::SMOOTH);
	displayOpts.setSliceLocs({
		{-1.0,  1.0, -0.4,  0.1},
		{-0.3,  1.0,  0.3,  0.1},
		{ 0.4,  1.0,  1.0,  0.1},
		{-1.0, -0.1, -0.4, -1.0},
		{-0.3, -0.1,  0.3, -1.0},
		{ 0.4, -0.1,  1.0, -1.0}
	});
#define MACRO_ADD_CONTROL(key, ...) displayOpts.addControl({key}, \
	{labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation( \
			labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation:: \
			Operation::__VA_ARGS__)});
	MACRO_ADD_CONTROL(GLFW_KEY_W, MOVE_FORWARD)
	MACRO_ADD_CONTROL(GLFW_KEY_E, MOVE_UP)
	MACRO_ADD_CONTROL(GLFW_KEY_D, MOVE_RIGHT)
	MACRO_ADD_CONTROL(GLFW_KEY_Q, MOVE_DOWN);
	MACRO_ADD_CONTROL(GLFW_KEY_A, MOVE_LEFT);
	MACRO_ADD_CONTROL(GLFW_KEY_S, MOVE_BACKWARDS);
	MACRO_ADD_CONTROL(GLFW_KEY_I, ROTATE_UP);
	MACRO_ADD_CONTROL(GLFW_KEY_L, ROTATE_RIGHT);
	MACRO_ADD_CONTROL(GLFW_KEY_K, ROTATE_DOWN);
	MACRO_ADD_CONTROL(GLFW_KEY_J, ROTATE_LEFT);
	MACRO_ADD_CONTROL(GLFW_KEY_O, ROTATE_CLOCKWISE);
	MACRO_ADD_CONTROL(GLFW_KEY_U, ROTATE_COUNTERCLOCKWISE);
#undef MACRO_ADD_CONTROL
#define MACRO_ADD_CONTROL(key, ...) displayOpts.addControl({key}, \
	{labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation(__VA_ARGS__)});
	MACRO_ADD_CONTROL(GLFW_KEY_R, 0);
	MACRO_ADD_CONTROL(GLFW_KEY_F, 1);
	MACRO_ADD_CONTROL(GLFW_KEY_C, 2);
	MACRO_ADD_CONTROL(GLFW_KEY_X, 3);
	MACRO_ADD_CONTROL(GLFW_KEY_Z, 4);
	MACRO_ADD_CONTROL(GLFW_KEY_V, 5);
	// rotational bindings later
#undef MACRO_ADD_CONTROL

	labyrinth_core::maze::MazeViewer::ViewerOptions viewerOpts (5);
	viewerOpts.setNumThreads(16);
	viewerOpts.setFov(110);
	const size_t w = 324, h = 324;
	labyrinth_core::maze::MazeViewer::Slice slice1 (5, w, h);
	viewerOpts.addSlice(slice1);
	labyrinth_core::maze::MazeViewer::Slice slice2 = slice1;
	const double thirdDim[] = {0, 0, 1, 0, 0};
	const double forthDim[] = {0, 0, 0, 1, 0};
	const double fifthDim[] = {0, 0, 0, 0, 1};
	slice2.setUp(forthDim);
	viewerOpts.addSlice(slice2);
	labyrinth_core::maze::MazeViewer::Slice slice3 = slice2;
	slice3.setUp(fifthDim);
	viewerOpts.addSlice(slice3);
	labyrinth_core::maze::MazeViewer::Slice slice4 = slice2;
	slice4.setRight(thirdDim);
	viewerOpts.addSlice(slice4);
	labyrinth_core::maze::MazeViewer::Slice slice5 = slice4;
	slice5.setUp(fifthDim);
	viewerOpts.addSlice(slice5);
	labyrinth_core::maze::MazeViewer::Slice slice6 = slice5;
	slice6.setRight(forthDim);
	viewerOpts.addSlice(slice6);

	display = new labyrinth_desktop::maze::MazeDisplay(
			std::move(maze), camera, displayOpts, viewerOpts, width, height);
	return 0;
}

int initDisplay12D(size_t width, size_t height) {
	labyrinth_core::maze::Maze::MazeGenerationOptions genOpts;
	genOpts.setDimensions({4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4});
	genOpts.setSeed("2");
	genOpts.setDensity(1);
	genOpts.setBranchProbability(0);
	genOpts.setBranchDeathProbability(0);
	genOpts.setTwistProbability(0.5);
	genOpts.setFlowProbability(0.7);
	genOpts.setRestrictNewAmount(1);
	genOpts.setLoopProbability(0);
	genOpts.setBlockProbability(0);
	genOpts.setMaxUseless(50000000);
	//const std::uint32_t theDims[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
	//labyrinth_core::maze::Maze maze (12, theDims);
	labyrinth_core::maze::Maze maze (genOpts);

	const double camera[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	labyrinth_desktop::maze::MazeDisplay::DisplayOptions displayOpts;
	displayOpts.setVelocity(5);
	displayOpts.setRotatSensitivity(250);
	displayOpts.setSquareness(labyrinth_desktop::maze::
			MazeDisplay::DisplayOptions::Squareness::SMOOTH);
	displayOpts.setSliceLocs({
		{-1.0,  1.0, -0.1,  0.1},
		{ 0.1,  1.0,  1.0,  0.1},
		{-1.0, -0.1, -0.1, -1.0},
		{ 0.1, -0.1,  1.0, -1.0}
	});
#define MACRO_ADD_CONTROL(key, ...) displayOpts.addControl({key}, \
	{labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation( \
			labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation:: \
			Operation::__VA_ARGS__)});
	MACRO_ADD_CONTROL(GLFW_KEY_W, MOVE_FORWARD)
	MACRO_ADD_CONTROL(GLFW_KEY_E, MOVE_UP)
	MACRO_ADD_CONTROL(GLFW_KEY_D, MOVE_RIGHT)
	MACRO_ADD_CONTROL(GLFW_KEY_Q, MOVE_DOWN);
	MACRO_ADD_CONTROL(GLFW_KEY_A, MOVE_LEFT);
	MACRO_ADD_CONTROL(GLFW_KEY_S, MOVE_BACKWARDS);
	MACRO_ADD_CONTROL(GLFW_KEY_I, ROTATE_UP);
	MACRO_ADD_CONTROL(GLFW_KEY_L, ROTATE_RIGHT);
	MACRO_ADD_CONTROL(GLFW_KEY_K, ROTATE_DOWN);
	MACRO_ADD_CONTROL(GLFW_KEY_J, ROTATE_LEFT);
	MACRO_ADD_CONTROL(GLFW_KEY_O, ROTATE_CLOCKWISE);
	MACRO_ADD_CONTROL(GLFW_KEY_U, ROTATE_COUNTERCLOCKWISE);
#undef MACRO_ADD_CONTROL
#define MACRO_ADD_CONTROL(key, ...) displayOpts.addControl({key}, \
	{labyrinth_desktop::maze::MazeDisplay::MazeDisplayOperation(__VA_ARGS__)});
	MACRO_ADD_CONTROL(GLFW_KEY_R, 0);
	MACRO_ADD_CONTROL(GLFW_KEY_F, 1);
	MACRO_ADD_CONTROL(GLFW_KEY_C, 2);
	MACRO_ADD_CONTROL(GLFW_KEY_X, 3);
	// rotational bindings later
#undef MACRO_ADD_CONTROL

	labyrinth_core::maze::MazeViewer::ViewerOptions viewerOpts (12);
	viewerOpts.setNumThreads(8);
	viewerOpts.setFov(140);
	const size_t w = 200, h = 200;
	double dim[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	labyrinth_core::maze::MazeViewer::Slice slice1 (12, w, h);
	viewerOpts.addSlice(slice1);
	labyrinth_core::maze::MazeViewer::Slice slice2 (12, w, h);
	dim[3] = 1;
	slice2.setForward(dim);
	dim[3] = 0;
	dim[4] = 1;
	slice2.setRight(dim);
	dim[4] = 0;
	dim[5] = 1;
	slice2.setUp(dim);
	dim[5] = 0;
	viewerOpts.addSlice(slice2);
	labyrinth_core::maze::MazeViewer::Slice slice3 (12, w, h);
	dim[6] = 1;
	slice3.setForward(dim);
	dim[6] = 0;
	dim[7] = 1;
	slice3.setRight(dim);
	dim[7] = 0;
	dim[8] = 1;
	slice3.setUp(dim);
	dim[8] = 0;
	viewerOpts.addSlice(slice3);
	labyrinth_core::maze::MazeViewer::Slice slice4 (12, w, h);
	dim[9] = 1;
	slice4.setForward(dim);
	dim[9] = 0;
	dim[10] = 1;
	slice4.setRight(dim);
	dim[10] = 0;
	dim[11] = 1;
	slice4.setUp(dim);
	viewerOpts.addSlice(slice4);

	display = new labyrinth_desktop::maze::MazeDisplay(
			std::move(maze), camera, displayOpts, viewerOpts, width, height);
	return 0;
}

int main() {
	glfwSetErrorCallback(&errCallback);
	if (!glfwInit()) {
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, 0);

	const size_t width = 1080, height = 720;
	GLFWwindow* window = glfwCreateWindow(width, height,
			"MazeDisplay test", nullptr, nullptr);
	if (window == 0) {
		return 1;
	}
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << glewGetErrorString(err) << std::endl;
		glfwTerminate();
		return 1;
	}

	initDisplay4D(width, height);

	glfwSetKeyCallback(window, &keyCallback);
	glfwSetFramebufferSizeCallback(window, &framebufferSizeCallback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	size_t fpsCount = 0;
	auto beg = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		display->display();
		std::cout << display->getErrMsg();
		glfwPollEvents();
		auto dur = std::chrono::high_resolution_clock::now() - beg;
		double secs = std::chrono::duration_cast<
				std::chrono::duration<double>>(dur).count();
		fpsCount++;
		if (secs >= 1) {
			std::cout << fpsCount << "fps" << std::endl;
			fpsCount = 0;
			beg = std::chrono::high_resolution_clock::now();
		}
	}
	delete display;
}

