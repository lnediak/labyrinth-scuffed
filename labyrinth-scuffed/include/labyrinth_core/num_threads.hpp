#ifndef INCLUDE_LABYRINTH_CORE_NUM_THREADS_HPP_
#define INCLUDE_LABYRINTH_CORE_NUM_THREADS_HPP_

#include <string>

namespace labyrinth_core {

namespace multithread {

constexpr size_t getNumThreads() {
	return 8;
}

} // multithread

} // labyrinth_core

#endif /* INCLUDE_LABYRINTH_CORE_NUM_THREADS_HPP_ */
