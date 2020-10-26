#ifndef INCLUDE_LABYRINTH_DESKTOP_DISPLAYABLE_HPP_
#define INCLUDE_LABYRINTH_DESKTOP_DISPLAYABLE_HPP_

#include <string>

namespace labyrinth_desktop {

class Displayable {

public:
	virtual ~Displayable() {}

	virtual void display() = 0;

	virtual void onWindowResize(size_t width, size_t height) = 0;

	virtual void onKeyPress(int key) = 0;

	virtual void onKeyRelease(int key) = 0;

	virtual void onCharType(int unicode_codepoint) = 0;

	virtual void onMouseMove(double x, double y) = 0;

	enum class MouseButton {

		LEFT, MIDDLE, RIGHT

	};

	virtual void onMousePress(MouseButton button) = 0;

	virtual void onMouseRelease(MouseButton button) = 0;

	virtual void onMouseScroll(double xoff, double yoff) = 0;

};

}

#endif /* INCLUDE_LABYRINTH_DESKTOP_DISPLAYABLE_HPP_ */
