#ifndef SRC_LABYRINTH_CORE_MAZE_MAZE_STONE_HPP_
#define SRC_LABYRINTH_CORE_MAZE_MAZE_STONE_HPP_

#include <algorithm>

#include "labyrinth_core/maze/maze_block.hpp"

namespace labyrinth_core {

namespace maze {

class MazeStone: public MazeBlock {

public:
	MazeStone(size_t dims, const std::uint64_t* location_in,
			const double* camera_in, const double* forward_in):
				MazeBlock(dims, location_in,
						camera_in, forward_in) {}

	bool isVoid() const override {
		return false;
	}

	void rayTrace(const double* direction, std::string* errMsg,
			std::vector<Color>* colors,
			std::vector<double>* t_values) const override {
		size_t n = getNumDims();

		struct Intersection {
			double t;
			bool isForward;
			bool operator<(const Intersection& other) const noexcept {
				return t < other.t;
			}
		};
		std::vector<Intersection> intersections;
		for (size_t i = 0; i < n; i++) {
			double numeratori = numerators[i];
			double numeratorin = numerators[i + numDims];
			double denominator = direction[i];
			double ti, tin;
			if (std::abs(denominator) < 1e-6) {
				ti = -1000000;
				tin = -1000000;
			} else {
				ti = numeratori / denominator;
				tin = numeratorin / -denominator;
			}
			intersections.push_back(Intersection{ti,
				(ti <= 0)? numeratori > 0: numeratori < 0});
			intersections.push_back(Intersection{tin,
				(tin <= 0)? numeratorin > 0: numeratorin < 0});
		}
		std::sort(intersections.begin(), intersections.end());

		std::int64_t lastForward = -1;
		bool encounteredNonForward = false;
		for (size_t i = 0; i < intersections.size(); i++) {
			if (intersections[i].isForward) {
				if (encounteredNonForward) {
					colors->clear();
					t_values->clear();
					return;
				}
				lastForward = i;
			} else {
				encounteredNonForward = true;
			}
		}
		if (lastForward == -1) {
			colors->clear();
			t_values->clear();
			return;
		}
		double t = intersections[lastForward].t;
		if (t <= 0) {
			colors->clear();
			t_values->clear();
			return;
		}

		// here we evaluate the texture according to a funky formula (lol)
		std::uint64_t* location = getLocation();
		double* camera = getCameraLocation();
		double sum = 0;
		for (size_t i = 0; i < n; i++) {
			sum += (camera[i] + t * direction[i]) - location[i];
		}
		// normalize to [100, 156]
		// from [-n/2, n/2]
		unsigned char color = sum * (28 / (n/2.0)) + 128;
		colors = std::vector<Color>{Color{color, color, color, 0xFF}};
		t_values = std::vector<double>{t};
	}

};

}

}

#endif /* SRC_LABYRINTH_CORE_MAZE_MAZE_STONE_HPP_ */
