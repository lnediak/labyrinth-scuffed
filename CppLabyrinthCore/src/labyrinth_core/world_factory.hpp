#ifndef SRC_LABYRINTH_CORE_WORLD_FACTORY_HPP_
#define SRC_LABYRINTH_CORE_WORLD_FACTORY_HPP_

#include "labyrinth_core/abstract_world.hpp"

namespace labyrinth_core {

class WorldFactory {

public:
	enum class WorldType {
		MAZE
	};

	static AbstractWorld* createEmptyWorld(WorldType type);

};

}

#endif /* SRC_LABYRINTH_CORE_WORLD_FACTORY_HPP_ */
