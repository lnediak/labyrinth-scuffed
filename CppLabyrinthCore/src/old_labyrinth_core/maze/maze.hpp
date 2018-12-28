#ifndef SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_HPP_
#define SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_HPP_

#include <string>

#include <cstdint>

class Maze {

	unsigned char* data;
	size_t dataLength;
	size_t numDims;
	std::uint32_t* dimensions;
	size_t* tempProds;

public:
	Maze(size_t numDims, std::uint32_t* inDimensions): numDims(numDims) {
		dimensions = new std::uint64_t[numDims];
		tempProds = new std::uint64_t[numDims];
		size_t currProd = 1;
		for (std::int64_t j = numDims - 1; j >= 0; j--) {
			tempProds[j] = currProd;
			currProd *= (dimensions[j] = inDimensions[j]);
		}
		dataLength = currProd;
		data = new unsigned char[dataLength](0);
	}

	Maze(Maze& other) = delete;
	Maze(const Maze& other) = delete;
	Maze(Maze&& other) = delete;
	Maze& operator=(Maze& other) = delete;
	Maze& operator=(const Maze& other) = delete;
	Maze& operator=(Maze&& other) = delete;

	~Maze() {
		delete[] data;
		delete[] dimensions;
		delete[] tempProds;
	}

	size_t getInd(const std::int32_t* location) const {
		std::uint32_t* tmpTempProds = tempProds;
		std::uint32_t* tempProdsEnd = tempProds + numDims;
		size_t result = 0;
		for (; tmpTempProds < tempProdsEnd; ++tmpTempProds, ++location) {
			result += (*tmpTempProds) * (*location);
		}
		return result;
	}

	unsigned char getBlock(size_t ind) const {
		return data[ind];
	}

	unsigned char getBlock(const std::int32_t* location) const {
		return getBlock(getInd(location));
	}

	void setBlock(size_t ind, unsigned char newBlock) {
		data[ind] = newBlock;
	}

	void setBlock(const std::int32_t* location, unsigned char newBlock) {
		setBlock(getInd(location), newBlock);
	}

};

#endif /* SRC_OLD_LABYRINTH_CORE_MAZE_MAZE_HPP_ */
