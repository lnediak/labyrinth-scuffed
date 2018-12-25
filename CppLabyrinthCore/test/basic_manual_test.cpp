#include <iostream>
#include <sstream>

#include <cmath>

#include "libs/lodepng.h"

#include "basic_labyrinth_core/render.hpp"

void genIMG(const render_convex_object::Slice& slice,
		const convex_object::ConvexObject& object,
		const std::string& filename) {
	const size_t width = 640, height = 480;
	std::valarray<uint32_t> img =
			render_convex_object::renderObjects(slice, {object},
					[](std::valarray<double> point)->uint32_t {
		unsigned thing = 0;
		for (double d: point) {
			thing += (d - std::floor(d)) * 9;
		}
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
	std::vector<unsigned char> towrite (width * height * 4);
	for (size_t i = 0; i < img.size(); i++) {
		towrite[i * 4 + 0] = (img[i] & 0xFF000000) >> 24;
		towrite[i * 4 + 1] = (img[i] & 0x00FF0000) >> 16;
		towrite[i * 4 + 2] = (img[i] & 0x0000FF00) >> 8;
		towrite[i * 4 + 3] = (img[i] & 0x000000FF) >> 0;
	}
	unsigned lodepng_error = lodepng::encode(filename, towrite,
			width, height);
	if (lodepng_error > 0) {
		std::cout << "lodepng error: " <<
				lodepng_error_text(lodepng_error) << std::endl;
	}
}

void gen3D() {
	render_convex_object::Slice slice {
		{10, 0, 0}, // camera
		{0, 0, -1}, // forward
		{0, 1, 0}, // right
		{-1, 0, 0} // up
	};
	convex_object::ConvexObject object (3);
	object.addConstraint({3, {std::sqrt(3)/2, 0, 0.5}, 2});
	object.addConstraint({3, {0.5, 0, -std::sqrt(3)/2}, 2});
	object.addConstraint({3, {1, 1, 0}, 5});
	object.addConstraint({3, {1, -1, 0}, 5});
	object.addConstraint({3, {-1, 0, 1}, 10});
	for (size_t i = 30; i <= 150; i++) {
		std::cout << i << std::endl;
		double deg2rad = 0.0174532925199432957692;
		double sin = std::sin(i * deg2rad);
		double cos = std::cos(i * deg2rad);
		render_convex_object::Slice actual {
			slice.camera,
			slice.forward * cos + slice.up * sin,
			slice.right,
			-slice.forward * sin + slice.up * cos
		};
		std::stringstream ss;
		ss << "/home/study/produce3d/" << i << ".png";
		genIMG(actual, object, ss.str());
	}
}

void gen4D() {
	render_convex_object::Slice slice {
		{10, 0, 0, 0}, // camera
		{0, 0, 0, 1}, // forward
		{0, 1, 0, 0}, // right
		{0, 0, 1, 0} // up
	};
	convex_object::ConvexObject object (4);
	object.addConstraint({4, {std::sqrt(3)/2, 0, 0.5, 0}, 2});
	object.addConstraint({4, {0.5, 0, -std::sqrt(3)/2, 0}, 2});
	object.addConstraint({4, {1, 1, 0, 0}, 5});
	object.addConstraint({4, {1, -1, 0, 0}, 5});
	object.addConstraint({4, {-1, 0, 1, 0}, 10});
	for (size_t i = 30; i <= 150; i++) {
		std::cout << i << std::endl;
		double deg2rad = 0.0174532925199432957692;
		double sin = std::sin(i * deg2rad);
		double cos = std::cos(i * deg2rad);
		render_convex_object::Slice actual {
			slice.camera,
			{-sin, 0, 0, cos},
			slice.right,
			slice.up
		};
		std::stringstream ss;
		ss << "/home/study/produce4drand/" << i << ".png";
		genIMG(actual, object, ss.str());
	}
}

int main() {
	gen4D();
}
