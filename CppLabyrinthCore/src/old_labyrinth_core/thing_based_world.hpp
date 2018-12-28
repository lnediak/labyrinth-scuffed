#ifndef SRC_OLD_LABYRINTH_CORE_THING_BASED_WORLD_HPP_
#define SRC_OLD_LABYRINTH_CORE_THING_BASED_WORLD_HPP_

#include <old_labyrinth_core/abstract_thing.hpp>
#include <old_labyrinth_core/abstract_world.hpp>
#include <limits>
#include <map>
#include <vector>


namespace labyrinth_core {

class ThingBasedWorld: public AbstractWorld {

public:
	/**
	 * The pointers will not be actually copied. Don't free them.
	 */
	virtual std::vector<AbstractThing*> getThings() const = 0;

	/**
	 * If a collision occurs (under some threshold), later AbstractThings
	 * in the vector have higher priority.
	 */
	Color rayTrace(const double* direction, Color backgroundColor,
			std::string* errMsg) const override {
		std::vector<AbstractThing*> things = getThings();
		// size_t is the index
		std::map<double, std::pair<Color, size_t>> results;

		{
			std::string tmpErrMsg;
			std::vector<Color> colors;
			std::vector<double> t_values;
			for (size_t i = 0; i < things.size(); i++) {
				AbstractThing* thing = things[i];
				thing->rayTrace(direction, &tmpErrMsg, &colors, &t_values);
				if (!tmpErrMsg.empty()) {
					*errMsg = tmpErrMsg;
					return Color{0, 0, 0, 0};
				}
				if (colors.size() != t_values.size()) {
					*errMsg = "Invalid AbstractThing rayTrace "
							"(colors and t_values not matching)";
					return Color{0, 0, 0, 0};
				}

				for (size_t ii = 0; ii < colors.size(); ii++) {
					results[t_values[ii]] = std::make_pair(colors[ii], i);
				}
			}
		}

		// eliminate near duplicates. Will stop if an opaque color is found.
		std::vector<Color> cleaned_results;
		double currT = std::numeric_limits<double>::min();
		Color currColor;
		size_t currI;
		for (const auto& result: results) {
			if (result.first - currT > 1e-5) {
				if (currT != std::numeric_limits<double>::min()) {
					if (currColor.a == 0xFF) {
						break;
					}
					cleaned_results.push_back(currColor);
				}
				currT = result.first;
				currColor = result.second.first;
				currI = result.second.second;
			} else {
				// we assume currT is not std::numeric_limits<double>::min()
				if (result.second.second > currI) {
					currT = result.first;
					currColor = result.second.first;
					currI = result.second.second;
				}
			}
		}
		if (currT != std::numeric_limits<double>::min()) {
			cleaned_results.push_back(currColor);
		} else {
			return backgroundColor;
		}

		unsigned long r, g, b, a;
		for (Color color: cleaned_results) {
			r += color.r;
			g += color.g;
			b += color.b;
			a += color.a;
		}
		r /= cleaned_results.size();
		g /= cleaned_results.size();
		b /= cleaned_results.size();
		a /= cleaned_results.size();

		return Color{r, g, b, a};
	}

};

}

#endif /* SRC_OLD_LABYRINTH_CORE_THING_BASED_WORLD_HPP_ */
