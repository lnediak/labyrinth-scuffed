#ifndef INCLUDE_LABYRINTH_DESKTOP_MAZE_MAZE_DISPLAY_HPP_
#define INCLUDE_LABYRINTH_DESKTOP_MAZE_MAZE_DISPLAY_HPP_

#include <labyrinth_core/maze/maze_viewer.hpp>
#include <labyrinth_desktop/controls_handler.hpp>
#include <labyrinth_desktop/displayable.hpp>

#include <limits>
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
		virtual ~MazeDisplayOperation() {}

		virtual void operate(MazeDisplay* display,
				const DisplayOptions& options) = 0;

	};

	class DisplayOptions: public ControlsHandler<MazeDisplayOperation> {

		// number of blocks to move in 1 second
		double velocity;
		// angle to turn in 1 second
		double rotatSensitivity;

		void operate(MazeDisplay* display, std::unordered_set<int> keys) const {
			auto iter = controls.find(keys);
			if (iter != controls.end()) {
				for (MazeDisplayOperation* oper: iter->second) {
					oper->operate(display, *this);
				}
			}
		}

	public:
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

	};

};

}

}

#endif /* INCLUDE_LABYRINTH_DESKTOP_MAZE_MAZE_DISPLAY_HPP_ */
