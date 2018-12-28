#ifndef SRC_OLD_LABYRINTH_CORE_BLOCK_BASED_THING_HPP_
#define SRC_OLD_LABYRINTH_CORE_BLOCK_BASED_THING_HPP_

#include <old_labyrinth_core/abstract_block.hpp>
#include <old_labyrinth_core/ortho_unit_dir.hpp>

namespace labyrinth_core {

class BlockBasedThing: public AbstractThing {

public:
	virtual AbstractBlock* getBlockAt(const std::uint64_t* location) const = 0;

	/**
	 * t_value and face are output parameters.
	 */
	virtual AbstractBlock* rayTraceForBlock(const double* direction,
			std::string* errMsg, double* t_value,
			ortho_unit_dir::OrthoUnitDir* face) const = 0;

	/**
	 * This is like if the cursor is pointing at a block or the like.
	 * Should display with an outline or some form of highlighting.
	 */
	virtual void setSelected(const std::uint64_t* location, std::string* errMsg) = 0;

	virtual void unselect(std::string* errMsg) = 0;

};

}

#endif /* SRC_OLD_LABYRINTH_CORE_BLOCK_BASED_THING_HPP_ */
