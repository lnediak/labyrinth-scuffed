#ifndef SRC_LABYRINTH_CORE_BLOCK_BASED_THING_HPP_
#define SRC_LABYRINTH_CORE_BLOCK_BASED_THING_HPP_

#include "labyrinth_core/abstract_block.hpp"
#include "labyrinth_core/ortho_unit_dir.hpp"

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

	virtual void setSelected(const std::uint64_t* location, std::string* errMsg) = 0;

	virtual void unselect(std::string* errMsg) = 0;

	class BlockStream {

	public:
		/**
		 * This is nullptr when it is done. Writes error message into errMsg.
		 */
		virtual AbstractBlock* getNextBlock(std::string* errMsg) = 0;

	};

	virtual BlockStream* createBlockStream(
			const double* direction, std::string* errMsg) const = 0;

	void rayTrace(const double* direction, std::string* errMsg,
				std::vector<Color>* colors,
				std::vector<double>* t_values) const override {
		std::string err;
		BlockStream* bstream = createBlockStream(direction, &err);
		std::vector<Color> cols;
		std::vector<double> ts;
		AbstractBlock* prevBlock = nullptr;
		AbstractBlock* currBlock;
		std::vector<Color> tmpcols;
		std::vector<double> tmpts;
		while ((currBlock = bstream->getNextBlock(&err)) != nullptr) {
			delete prevBlock;
			if (!err.empty()) {
				delete currBlock;
				*errMsg = err;
				return;
			}
			prevBlock = currBlock;
			if (!currBlock->isVoid()) {
				currBlock->rayTrace(direction, &err, &tmpcols, &tmpts);
				if (!err.empty()) {
					delete currBlock;
					*errMsg = err;
					return;
				}
				if (tmpcols.size() != tmpts.size()) {
					delete currBlock;
					*errMsg = "Invalid AbstractThing rayTrace "
							"(colors and t_values not matching)";
					return;
				}
				cols.insert(cols.end(), tmpcols.begin(), tmpcols.end());
				ts.insert(ts.end(), tmpts.begin(), tmpts.end());
			}
		}
		delete prevBlock;
		delete bstream;

		*colors = std::move(cols);
		*t_values = std::move(ts);
	}

};

}

#endif /* SRC_LABYRINTH_CORE_BLOCK_BASED_THING_HPP_ */
