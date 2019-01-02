#ifndef INCLUDE_LABYRINTH_CORE_MAZE_MAZE_HPP_
#define INCLUDE_LABYRINTH_CORE_MAZE_MAZE_HPP_

#include <labyrinth_core/color.hpp>

#include <algorithm>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

#include <cmath>
#include <cstdint>
#include <cstdlib>

namespace labyrinth_core {

namespace maze {

class Maze {

	std::uint8_t* data;
	size_t dataLength;
	size_t numDims;
	std::uint32_t* dimensions;
	size_t* tempProds;

public:
	class MazeGenerationOptions {

		std::vector<std::uint32_t> dimensions;
		std::string seed;
		double density;
		double branchProbability;
		double twistProbability;
		double flowProbability;
		bool allowLoops;

		void validateDimensions() {
			for (size_t i = 0; i < dimensions.size(); i++) {
				if (dimensions[i] < 3) {
					dimensions[i] = 3;
				}
			}
			if (dimensions.size() < 2) {
				dimensions = {3, 3};
			}
		}

		static double clampUnit(double in) {
			if (in < 0) {
				return 0;
			}
			if (in > 1) {
				return 1;
			}
			return in;
		}

	public:
		std::vector<std::uint32_t> getDimensions() const {
			return dimensions;
		}

		void setDimensions(const std::vector<std::uint32_t>& newDims) {
			dimensions = newDims;
			validateDimensions();
		}

		void setDimensions(std::vector<std::uint32_t>&& newDims) {
			dimensions = newDims;
			validateDimensions();
		}

		std::string getSeed() const {
			return seed;
		}

		void setSeed(const std::string& newSeed) {
			seed = newSeed;
		}

		void setSeed(std::string&& newSeed) {
			seed = newSeed;
		}

		double getDensity() const {
			return density;
		}

		void setDensity(double newDensity) {
			density = clampUnit(newDensity);
		}

		double getBranchProbability() const {
			return branchProbability;
		}

		void setBranchProbability(double newBranchProbab) {
			branchProbability = clampUnit(newBranchProbab);
		}

		double getTwistProbability() const {
			return twistProbability;
		}

		void setTwistProbability(double newTwistProbab) {
			twistProbability = clampUnit(newTwistProbab);
		}

		double getFlowProbability() const {
			return flowProbability;
		}

		void setFlowProbability(double newFlowProbab) {
			flowProbability = clampUnit(newFlowProbab);
		}

		bool getAllowLoops() const {
			return allowLoops;
		}

		void setAllowLoops(bool newAllowLoops) {
			allowLoops = newAllowLoops;
		}

	};

	Maze(size_t inNumDims, const std::uint32_t* inDimensions): numDims(inNumDims) {
		if (numDims < 2) {
			numDims = 2;
		}
		dimensions = new std::uint32_t[numDims];
		tempProds = new std::uint64_t[numDims];
		size_t currProd = 1;
		for (std::int64_t j = numDims - 1; j >= 0; j--) {
			tempProds[j] = currProd;
			currProd *=
					(dimensions[j] =
							std::max(
									static_cast<std::uint32_t>(3),
									inDimensions[j]));
		}
		dataLength = currProd;
		data = new std::uint8_t[dataLength];
		std::mt19937 mtrand (1);
		std::uniform_int_distribution<std::int16_t> distro (0, 20);
		for (size_t i = 0; i < dataLength; i++) {
			data[i] = distro(mtrand) > 0? 0: 1;
		}
	}

	explicit Maze(const MazeGenerationOptions& options) {
		std::vector<std::uint32_t> dims = options.getDimensions();
		numDims = dims.size();
		if (numDims < 2) {
			numDims = 2;
		}
		dimensions = new std::uint32_t[numDims];
		tempProds = new std::uint64_t[numDims];
		size_t currProd = 1;
		for (std::int64_t j = numDims - 1; j >= 0; j--) {
			tempProds[j] = currProd;
			currProd *= (dimensions[j] = dims[j]);
		}
		dataLength = currProd;
		data = new std::uint8_t[dataLength];
		generate(options);
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

	template<class Iter, class = typename std::enable_if<
				std::is_same<
				typename std::iterator_traits<Iter>::value_type,
				std::int32_t
				>::value>::type>
	size_t getInd(Iter location) const {
		std::uint64_t* tmpTempProds = tempProds;
		std::uint64_t* tempProdsEnd = tempProds + numDims;
		size_t result = 0;
		for (; tmpTempProds < tempProdsEnd; ++tmpTempProds, ++location) {
			result += (*tmpTempProds) * (*location);
		}
		return result;
	}

	template<class Iter, class = typename std::enable_if<
			std::is_same<
			typename std::iterator_traits<Iter>::value_type,
			std::int32_t
			>::value>::type>
	void fromInd(size_t ind, Iter location) const {
		std::uint64_t* tmpTempProds = tempProds;
		std::uint64_t* tempProdsEnd = tempProds + numDims;
		std::uint64_t tmp;
		std::uint64_t tmpTempProd;
		for (; tmpTempProds < tempProdsEnd; ++tmpTempProds, ++location) {
			tmpTempProd = *tmpTempProds;
			*location = tmp = ind / tmpTempProd;
			ind -= tmp * tmpTempProd;
		}
	}

	std::uint8_t getBlock(size_t ind) const {
		return data[ind];
	}

	template<class Iter, class = typename std::enable_if<
				std::is_same<
				typename std::iterator_traits<Iter>::value_type,
				std::int32_t
				>::value>::type>
	std::uint8_t getBlock(Iter location) const {
		return getBlock(getInd(location));
	}

	void setBlock(size_t ind, std::uint8_t newBlock) {
		data[ind] = newBlock;
	}

	template<class Iter, class = typename std::enable_if<
				std::is_same<
				typename std::iterator_traits<Iter>::value_type,
				std::int32_t
				>::value>::type>
	void setBlock(Iter location, std::uint8_t newBlock) {
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

private:
	class Head {

		// location
		size_t ind;
		std::vector<std::int32_t> loc;
		// index of dimension if positive,
		// plus numDims if negative.
		size_t prevDir;

		bool isLocFine(const Maze& maze, size_t prevInd) const {
			size_t numDims = maze.getNumDims();
			for (size_t dir = 0; dir < numDims * 2; dir++) {
				size_t indInConsideration;
				if (dir < numDims) {
					if (loc[dir] + 1 >=
							static_cast<std::int32_t>(maze.dimensions[dir] - 1)) {
						continue;
					}
					indInConsideration = ind + maze.tempProds[dir];
				} else {
					if (loc[dir - numDims] - 1 <= 0) {
						continue;
					}
					indInConsideration = ind - maze.tempProds[dir - numDims];
				}
				if (indInConsideration == prevInd) {
					continue;
				}
				if (maze.getBlock(indInConsideration) == 0) {
					return false;
				}
			}
			return true;
		}

	public:
		Head(const Maze& maze, size_t inInd): ind(inInd) {
			loc.resize(maze.getNumDims());
			maze.fromInd(inInd, loc.begin());
			prevDir = maze.getNumDims() * 2;
		}

		size_t getNextDir(const Maze& maze,
				const MazeGenerationOptions& options,
				std::mt19937& mtrand) {
			size_t numDims = maze.getNumDims();

			double twistProbability = options.getTwistProbability();
			double flowProbability = options.getFlowProbability();
			bool allowLoops = options.getAllowLoops();

			// direction is index, value is cumulative weight
			double* possibilities = new double[numDims * 2];
			double currWeight = 0;
			for (size_t dir = 0; dir < numDims * 2; dir++) {
				// go in inverse directions
				// static_cast for overload ambiguity resolution
				if (std::abs(static_cast<int>(dir - prevDir)) == numDims) {
					possibilities[dir] = currWeight;
					continue;
				}
				if (dir < numDims) {
					if (loc[dir] + 1 >=
							static_cast<std::int32_t>(maze.dimensions[dir] - 1)) {
						possibilities[dir] = currWeight;
						continue;
					}
				} else {
					if (loc[dir - numDims] - 1 <= 0) {
						possibilities[dir] = currWeight;
						continue;
					}
				}
				if (!allowLoops) {
					size_t prevInd = ind;
					move(maze, dir);
					if (!isLocFine(maze, prevInd)) {
						possibilities[dir] = currWeight;
						reverse(maze, dir);
						continue;
					}
					reverse(maze, dir);
				}
				double weight;
				if (dir == prevDir) {
					weight = 1 - twistProbability;
				} else {
					weight = twistProbability;
				}
				if (dir < numDims) {
					weight *= flowProbability;
				} else {
					weight *= 1 - flowProbability;
				}
				currWeight += weight;
				possibilities[dir] = currWeight;
			}
			if (currWeight < 1e-6) {
				delete[] possibilities;
				return numDims * 2;
			}
			std::uniform_real_distribution<double> distro (0, currWeight);
			double result = distro(mtrand);
			size_t toreturn = 0;
			for (; toreturn < numDims * 2; toreturn++) {
				if (possibilities[toreturn] > result) {
					break;
				}
			}
			delete[] possibilities;
			return toreturn;
		}

	private:
		void move(const Maze& maze, size_t dir) {
			size_t numDims = maze.getNumDims();
			if (dir < numDims) {
				ind += maze.tempProds[dir];
				loc[dir]++;
			} else {
				ind -= maze.tempProds[dir - numDims];
				loc[dir - numDims]--;
			}
		}

		void reverse(const Maze& maze, size_t dir) {
			size_t numDims = maze.getNumDims();
			if (dir < numDims) {
				ind -= maze.tempProds[dir];
				loc[dir]--;
			} else {
				ind += maze.tempProds[dir - numDims];
				loc[dir - numDims]++;
			}
		}

	public:
		bool isEatUseful(const Maze& maze, size_t dir) {
			move(maze, dir);
			bool toreturn = maze.getBlock(ind) > 0;
			reverse(maze, dir);
			return toreturn;
		}

		void eat(Maze& maze, size_t dir, size_t& numBlocksBroken) {
			move(maze, dir);
			prevDir = dir;
			if (maze.getBlock(ind) > 0) {
				numBlocksBroken++;
			}
			maze.setBlock(ind, 0);
		}

	};

	friend Head;

	void generate(const MazeGenerationOptions& options) {
		double density = options.getDensity();
		double branchProbability = options.getBranchProbability();

		// fill the maze before generating
		std::uint8_t* iter_data = data;
		std::uint8_t* data_end = data + dataLength;
		for (; iter_data < data_end; ++iter_data) {
			// fill the maze first
			*iter_data = 1;
		}

		// oneoneone is literally the ind of (1, 1, 1, 1, ....)
		size_t oneoneone = 0;
		// last is the ind of (dims[0] - 2, dims[1] - 2, dims[2] - 2, ...)
		size_t last = 0;
		for (size_t i = 0; i < numDims; i++) {
			oneoneone += tempProds[i];
			last += (dimensions[i] - 2) * tempProds[i];
		}
		setBlock(oneoneone, 0);
		std::vector<Head> heads;
		heads.push_back(Head(*this, oneoneone));
		std::string seed = options.getSeed();
		std::seed_seq seq (seed.begin(), seed.end());
		std::mt19937 mtrand (seq);
		std::uniform_real_distribution<double> branchDistro;
		size_t numBlocksBroken = 1;
		size_t totalBlocks = 1;
		for (size_t i = 0; i < numDims; i++) {
			totalBlocks *= dimensions[i] - 2;
		}
		while (!heads.empty() &&
				((numBlocksBroken < density * totalBlocks) ||
						(getBlock(last) == 1))) {
			size_t headsize = heads.size();
			for (size_t headi = 0; headi < headsize; headi++) {
				size_t dir = heads[headi].getNextDir(*this, options, mtrand);
				if (dir >= numDims * 2) {
					heads.erase(heads.begin() + headi);
					headsize = heads.size();
					continue;
				}
				if (heads[headi].isEatUseful(*this, dir)) {
					size_t i = 0;
					while ((i < numDims * 2) &&
							(branchDistro(mtrand) < branchProbability)) {
						heads.push_back(heads[headi]);
						headsize = heads.size();
						i++;
					}
				}
				heads[headi].eat(*this, dir, numBlocksBroken);
			}
		}
	}

};

} // maze

} // labyrinth_core

#endif /* INCLUDE_LABYRINTH_CORE_MAZE_MAZE_HPP_ */
