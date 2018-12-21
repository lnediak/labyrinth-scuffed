#include <iostream>

#include <cmath>

#include "libs/lodepng.h"

#include "labyrinth_core/render.hpp"

int main() {
	render_convex_object::Slice slice {
		{10, 0, 0}, // camera
		{-1, 0, 0}, // forward
		{0, 0, 1}, // up
		{0, 1, 0} // right
	};
	convex_object::ConvexObject object (3);
	object.addConstraint({3, {std::sqrt(3)/2, 0, 1/2}, 2});
	object.addConstraint({3, {1/2, 0, -std::sqrt(3)/2}, 2});
	object.addConstraint({3, {1/std::sqrt(2), 1/std::sqrt(2), 0}, 5});
	object.addConstraint({3, {1/std::sqrt(2), -1/std::sqrt(2), 0}, 5});
	object.addConstraint({3, {-1, 0, 0}, 1});
	const size_t width = 1080, height = 640;
	std::cout << "Beginning ray tracing stuff" << std::endl;
	std::valarray<uint32_t> img =
			render_convex_object::renderObjects(slice, {object},
					[](std::valarray<double> point)->uint32_t{
		double thingD = point[0] * 0xFFFFFFFF +
				point[1] * 0xFFFFFFFF + point[2] * 0xFFFFFFFF;
		uint64_t thing = *reinterpret_cast<uint64_t*>(&thingD);
		switch(thing % 3) {
		case 0:
			return 0xFF0000FF;
		case 1:
			return 0x00FF00FF;
		case 2:
			return 0x0000FFFF;
		}
		std::cout << point[0] << " " << point[1] << " " << point[2] << std::endl;
		return 0xFFFFFFFF;
	}, static_cast<uint32_t>(0x000000FF), width, height, 90);
	for (uint32_t bob: img) {
		if (!(bob & 0x000000FF)) {
			std::cout << std::hex << bob << std::dec << std::endl;
		}
	}
	std::cout << "Now writing image" << std::endl;
	std::vector<unsigned char> towrite (width * height * 4);
	for (size_t i = 0; i < img.size(); i++) {
		towrite[i * 4 + 0] = (img[i] & 0xFF000000) >> 24;
		towrite[i * 4 + 1] = (img[i] & 0x00FF0000) >> 16;
		towrite[i * 4 + 2] = (img[i] & 0x0000FF00) >> 8;
		towrite[i * 4 + 3] = (img[i] & 0x000000FF) >> 0;
	}
	unsigned lodepng_error = lodepng::encode("/home/study/produced.png", towrite,
			width, height);
	if (lodepng_error > 0) {
		std::cout << "lodepng error: " <<
				lodepng_error_text(lodepng_error) << std::endl;
	}
}
