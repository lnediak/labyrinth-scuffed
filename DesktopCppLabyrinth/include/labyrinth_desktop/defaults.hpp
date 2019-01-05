#ifndef INCLUDE_LABYRINTH_DESKTOP_DEFAULTS_HPP_
#define INCLUDE_LABYRINTH_DESKTOP_DEFAULTS_HPP_

#include <labyrinth_core/maze/maze_viewer.hpp>

namespace labyrinth_desktop {

namespace maze {

labyrinth_core::maze::MazeViewer::ViewerOptions
getDefaultMazeViewerOptions(size_t numDims, size_t totalWidth, size_t totalHeight) {
	if (numDims == 4) {
	labyrinth_core::maze::MazeViewer::ViewerOptions toreturn (numDims);
	labyrinth_core::maze::MazeViewer::Slice slice1 (numDims, 200, 200);
	toreturn.addSlice(slice1);
	labyrinth_core::maze::MazeViewer::Slice slice2 (numDims, 200, 200);
	const double secondDim[] = {0, 1, 0, 0};
	const double thirdDim[] = {0, 0, 1, 0};
	const double forthDim[] = {0, 0, 0, 1};
	slice2.setUp(forthDim);
	toreturn.addSlice(slice2);
	labyrinth_core::maze::MazeViewer::Slice slice3 = slice2;
	slice3.setRight(thirdDim);
	toreturn.addSlice(slice3);
	labyrinth_core::maze::MazeViewer::Slice slice4 = slice3;
	slice4.setForward(secondDim);
	toreturn.addSlice(slice4);
	return toreturn;
	} else {
		labyrinth_core::maze::MazeViewer::ViewerOptions* toreturn = nullptr;
		return *toreturn;
	}
}

}

}

#endif /* INCLUDE_LABYRINTH_DESKTOP_DEFAULTS_HPP_ */
