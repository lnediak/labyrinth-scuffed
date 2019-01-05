#ifndef INCLUDE_LABYRINTH_DESKTOP_CONTROLS_HANDLER_HPP_
#define INCLUDE_LABYRINTH_DESKTOP_CONTROLS_HANDLER_HPP_

#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace labyrinth_desktop {

template<class T>
class ControlsHandler {

	template<class IntSet,
	class = typename std::enable_if<
	std::is_same<
	typename IntSet::value_type,
	int>::value>::type>
	class IntSetHash {

	public:
		typedef IntSet argument_type;
		typedef size_t result_type;

		result_type operator()(argument_type argument) const noexcept {
			result_type toreturn = 1;
			for (int i: argument) {
				if (i < 0) {
					toreturn *=
							std::numeric_limits<unsigned int>::max() -
							static_cast<unsigned int>(-i) + 1;
				} else {
					toreturn *= static_cast<unsigned int>(i);
				}
			}
			return toreturn;
		}

	};

	std::unordered_map<
	std::unordered_set<int>,
	std::vector<T*>,
	IntSetHash<std::unordered_set<int>>> controls;

public:
	ControlsHandler(ControlsHandler& other) = delete;
	ControlsHandler(const ControlsHandler& other) = delete;
	// note: this is not delete
	ControlsHandler(ControlsHandler&& other) = default;
	ControlsHandler& operator=(ControlsHandler& other) = delete;
	ControlsHandler& operator=(const ControlsHandler& other) = delete;
	// note: this is not delete
	ControlsHandler& operator=(ControlsHandler&& other) = default;

	virtual ~ControlsHandler() {
		for (const auto& pair: controls) {
			for (T* oper: pair.second) {
				delete oper;
			}
		}
	}

	/**
	 * Do not free the pointers. Just let it go out of scope.
	 */
	decltype(controls) getControls() const {
		return controls;
	}

	/**
	 * Specifies that when all of keys are pressed,
	 * all the operations in operations will be executed.
	 */
	void addControl(std::unordered_set<int> keys,
			std::vector<T*> operations) {
		controls[keys] = operations;
	}

	void removeControl(std::unordered_set<int> keys) {
		if (controls.count(keys) > 0) {
			auto vec = controls[keys];
			for (auto ptr: vec) {
				delete ptr;
			}
			controls.erase(keys);
		}
	}

};

}

#endif /* INCLUDE_LABYRINTH_DESKTOP_CONTROLS_HANDLER_HPP_ */
