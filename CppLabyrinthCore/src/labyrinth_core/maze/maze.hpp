#ifndef SRC_LABYRINTH_CORE_MAZE_MAZE_HPP_
#define SRC_LABYRINTH_CORE_MAZE_MAZE_HPP_

#include <labyrinth_core/color.hpp>

#include <algorithm>
#include <string>

#include <cstdint>

namespace labyrinth_core {

namespace maze {

class Maze {

	std::uint8_t* data;
	size_t dataLength;
	size_t numDims;
	std::uint32_t* dimensions;
	size_t* tempProds;

public:
	Maze(size_t numDims, const std::uint32_t* inDimensions): numDims(numDims) {
		dimensions = new std::uint32_t[numDims];
		tempProds = new std::uint64_t[numDims];
		size_t currProd = 1;
		for (std::int64_t j = numDims - 1; j >= 0; j--) {
			tempProds[j] = currProd;
			currProd *= (dimensions[j] = inDimensions[j]);
		}
		dataLength = currProd;
		data = new std::uint8_t[dataLength];
		for (size_t i = 0; i < dataLength; i++) {
			data[i] = 1;
		}
	}

	Maze(Maze& other) = delete;
	Maze(const Maze& other) = delete;
	Maze(Maze&& other) = delete;
	Maze& operator=(Maze& other) = delete;
	Maze& operator=(const Maze& other) = delete;
	Maze& operator=(Maze&& other) = delete;

	/**
	 * Shallow copy.
	 */
	Maze(const Maze& other, int): data(other.data), dataLength(other.dataLength),
			numDims(other.numDims), dimensions(other.dimensions),
			tempProds(other.tempProds) {}

	/**
	 * Shallow copy.
	 */
	Maze& operator()(const Maze& other, int) {
		delete[] data;
		data = other.data;
		dataLength = other.dataLength;
		numDims = other.numDims;
		delete[] dimensions;
		dimensions = other.dimensions;
		delete[] tempProds;
		tempProds = other.tempProds;
		return *this;
	}

	/**
	 * If you shallow copied it somewhere,
	 * call this before going out of scope.
	 */
	void invalidate() {
		data = nullptr;
		dimensions = nullptr;
		tempProds = nullptr;
	}

	~Maze() {
		delete[] data;
		delete[] dimensions;
		delete[] tempProds;
	}

	size_t getNumDims() const {
		return numDims;
	}

	/**
	 * Makes a copy. You delete this.
	 */
	std::uint32_t* getDimensions() const {
		std::uint32_t* toreturn = new std::uint32_t[numDims];
		std::copy(dimensions, dimensions + numDims, toreturn);
		return toreturn;
	}

	size_t getInd(const std::int32_t* location) const {
		std::uint64_t* tmpTempProds = tempProds;
		std::uint64_t* tempProdsEnd = tempProds + numDims;
		size_t result = 0;
		for (; tmpTempProds < tempProdsEnd; ++tmpTempProds, ++location) {
			result += (*tmpTempProds) * (*location);
		}
		return result;
	}

	std::uint8_t getBlock(size_t ind) const {
		return data[ind];
	}

	std::uint8_t getBlock(const std::int32_t* location) const {
		return getBlock(getInd(location));
	}

	void setBlock(size_t ind, std::uint8_t newBlock) {
		data[ind] = newBlock;
	}

	void setBlock(const std::int32_t* location, std::uint8_t newBlock) {
		setBlock(getInd(location), newBlock);
	}

	/**
	 * Offsets is location relative to the center of the block.
	 */
	static Color getBlockColor(std::uint8_t block,
			size_t numDims, const double* offsets) {
		if (block == 0) {
			return Color{0, 0, 0, 0};
		} else {
			double sum = 0;
			const double* offsetsEnd = offsets + numDims;
			for (; offsets < offsetsEnd; ++offsets) {
				sum += *offsets;
			}
			// normalize from (-n/2, n/2) to (100, 156)
			std::uint8_t value = 128 + (sum * 56 / numDims);
			return Color{value, value, value, 0xFF};
		}
	}

};

} // maze

} // labyrinth_core

#endif /* SRC_LABYRINTH_CORE_MAZE_MAZE_HPP_ */
