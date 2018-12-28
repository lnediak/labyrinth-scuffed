#ifndef SRC_OLD_LABYRINTH_CORE_ABSTRACT_BLOCK_HPP_
#define SRC_OLD_LABYRINTH_CORE_ABSTRACT_BLOCK_HPP_

#include <old_labyrinth_core/abstract_thing.hpp>
#include <cstdint>


namespace labyrinth_core {

class AbstractBlock {

public:
	virtual ~AbstractBlock() {}

	/**
	 * Returns whether the block is simply void/air/nothingness.
	 */
	virtual bool isVoid() const = 0;

	virtual size_t getNumDims() const = 0;

	/**
	 * This can be nullptr (if the block does not store location information)
	 */
	virtual std::uint64_t* getLocation() const = 0;

	/**
	 * IMPORTANT!!!!!! point is relative to the center of the block.
	 * If the block is opaque, simply returns the color of the texture at point.
	 * Otherwise, scans the texture in the direction of direction from point.
	 */
	virtual void rayTrace(const double* point, const double* direction,
			std::string* errMsg,
			std::vector<Color>* colors, std::vector<double>* t_values) const = 0;

};

}

#endif /* SRC_OLD_LABYRINTH_CORE_ABSTRACT_BLOCK_HPP_ */
