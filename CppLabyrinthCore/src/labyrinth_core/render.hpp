#ifndef SRC_LABYRINTH_CORE_RENDER_HPP_
#define SRC_LABYRINTH_CORE_RENDER_HPP_

#include <algorithm>
#include <utility>
#include <valarray>
#include <vector>

#include <cmath>

#include <stdint.h>

#include "convex_object.hpp"

namespace render_convex_object {

struct Slice {

	std::valarray<double> camera;
	std::valarray<double> forward;
	std::valarray<double> up;
	std::valarray<double> right;

};

/**
 * Functor must have operator()(std::valarray<double>) defined.
 * Make sure all numDims are consistent. Otherwise output will be empty.
 * fov is in degrees.
 */
template <class Functor>
std::valarray<uint32_t> renderObjects(const Slice& slice,
		const std::vector<convex_object::ConvexObject>& objects,
		const Functor& getColor, uint32_t backgroundColor,
		size_t width, size_t height, double fov) {
	// 0.00872664625997164788462 = 0.5 * PI / 180
	double tanFov = std::tan(fov * 0.00872664625997164788462);
	double aspect = static_cast<double>(height) / width;
	double scale = (aspect > 1)? 2 * tanFov / height : 2 * tanFov / width;



	size_t numDims = slice.camera.size();
	for (const convex_object::ConvexObject& object: objects) {
		if (object.getNumDims() != numDims) {
			return std::valarray<uint32_t>();
		}
	}
	std::valarray<uint32_t> toreturn (backgroundColor, width * height);
	for (size_t row = 0; row < height; row++) {
		for (size_t col = 0; col < width; col++) {
			double rcomponent = (col - width/2) * scale;
			double ucomponent = (row - height/2) * scale;
			convex_object::Ray ray (numDims,
					slice.forward +
					rcomponent * slice.right +
					ucomponent * slice.up,

					slice.camera);
			std::vector<convex_object::ConvexObject::RayIntersection> intersections;
			bool foundAnything;
			for (const convex_object::ConvexObject& object: objects) {
				convex_object::ConvexObject::RayIntersection intersection =
						object.rayTrace(ray, &foundAnything);
				if (foundAnything) {
					intersections.push_back(std::move(intersection));
				}
			}
			if (intersections.empty()) {
				toreturn[row * width + col] = backgroundColor;
				continue;
			}
			std::sort(intersections.begin(), intersections.end());
			toreturn[row * width + col] = getColor(intersections[0].point);
		}
	}

	return toreturn;
}

}

#endif /* SRC_LABYRINTH_CORE_RENDER_HPP_ */
