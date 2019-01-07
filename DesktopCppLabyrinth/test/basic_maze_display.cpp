#include <GL/glew.h>

#include <labyrinth_desktop/maze/maze_display.hpp>

#include <GLFW/glfw3.h>

#include <iostream>

namespace {

labyrinth_core::maze::Maze* maze = nullptr;
labyrinth_desktop::maze::MazeDisplay* display = nullptr;

int initDisplay3D() {
	labyrinth_core::maze::Maze::MazeGenerationOptions genOpts;
	genOpts.setDimensions({5, 5, 5});
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
	maze = new labyrinth_core::maze::Maze(genOpts);

	const double camera[] = {1, 1, 1};

	labyrinth_desktop::maze::MazeDisplay::DisplayOptions displayOpts;
	displayOpts.setVelocity(5);
	displayOpts.setRotatSensitivity(250);
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
	const size_t width = 200, height = 200;
	labyrinth_core::maze::MazeViewer::Slice slice1 (3, width, height);
	viewerOpts.addSlice(slice1);

	display = new labyrinth_desktop::maze::MazeDisplay(
			*maze, camera, displayOpts, viewerOpts);
	return 0;
}

int initDisplay4D() {
	labyrinth_core::maze::Maze::MazeGenerationOptions genOpts;
	genOpts.setDimensions({5, 5, 5, 5});
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
	maze = new labyrinth_core::maze::Maze(genOpts);

	const double camera[] = {1, 1, 1, 1};

	labyrinth_desktop::maze::MazeDisplay::DisplayOptions displayOpts;
	displayOpts.setVelocity(5);
	displayOpts.setRotatSensitivity(250);
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
	const size_t width = 200, height = 200;
	labyrinth_core::maze::MazeViewer::Slice slice1 (4, width, height);
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
			*maze, camera, displayOpts, viewerOpts);
	return 0;
}

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

int main() {
	glfwSetErrorCallback(&errCallback);
	if (!glfwInit()) {
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, 0);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", 0, 0);
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

	initDisplay4D();

	glfwSetKeyCallback(window, &keyCallback);
	glfwSetFramebufferSizeCallback(window, &framebufferSizeCallback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		display->display();
		std::cout << display->getErrMsg();
		glfwPollEvents();
	}
	delete display;
	delete maze;
}
