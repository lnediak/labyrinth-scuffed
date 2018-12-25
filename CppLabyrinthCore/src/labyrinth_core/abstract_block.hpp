#ifndef SRC_LABYRINTH_CORE_ABSTRACT_BLOCK_HPP_
#define SRC_LABYRINTH_CORE_ABSTRACT_BLOCK_HPP_

#include <cstdint>

#include "labyrinth_core/abstract_thing.hpp"

namespace labyrinth_core {

class AbstractBlock: public AbstractThing {

public:
	virtual ~AbstractBlock() {}

	/**
	 * Returns whether the block is simply void/air/nothingness.
	 */
	virtual bool isVoid() const = 0;

	virtual std::uint64_t* getLocation() const = 0;

};

}

#endif /* SRC_LABYRINTH_CORE_ABSTRACT_BLOCK_HPP_ */
