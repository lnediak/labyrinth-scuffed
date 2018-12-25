#ifndef ORTHO_UNIT_DIR_HPP_
#define ORTHO_UNIT_DIR_HPP_

#include <cstdint>

namespace ortho_unit_dir {

/**
 * Literally. Simply an elementary unit vector in arbitrary dimensions.
 */
class OrthoUnitDir {

public:
	/**
	 * if f is true, i refers to the index of the axis.
	 * if f is false, i - n refers to the index of the axis.
	 */
	std::uint32_t i;
	/**
	 * this refers to the index of the axis the direction is parallel to.
	 */
	std::uint32_t ind;
	/**
	 * if f is true, reversedI is i + n.
	 * if f is false, reversedI is i - n.
	 */
	std::uint32_t reversedI;
	/**
	 * this refers to the number of dimensions there are.
	 */
	std::uint32_t n;
	/**
	 * this refers to whether or not the direction is in a positive direction along its axis.
	 */
	bool f;

	OrthoUnitDir(): i(0), ind(0), reversedI(0), n(0), f(false) {}

	/**
	 * if the direction is toward positive (f is true), i < n.
	 * Else i >= n. Given positive direction, i is the index of the dimension
	 * that this direction is in. Given negative direction i - n is the index.
	 */
	OrthoUnitDir(std::int32_t ind, bool f, std::int32_t n): ind(ind), n(n), f(f) {
		if (f) {
			i = ind;
			reversedI = ind + n;
		} else {
			i = ind + n;
			reversedI = ind;
		}
	}

protected:
	OrthoUnitDir(std::uint32_t i, std::uint32_t ind,
			std::uint32_t reversedI, std::uint32_t n, bool f):
		i(i), ind(ind), reversedI(reversedI), n(n), f(f) {}

public:
	OrthoUnitDir(std::int32_t i, std::int32_t n): i(i), n(n) {
		f = i < n;
		if (f) {
			reversedI = i + n;
			ind = i;
		} else {
			reversedI = i - n;
			ind = reversedI;
		}
	}

	bool operator==(const OrthoUnitDir& other) const {
		return (n == other.n) && (i == other.i)
				&& (reversedI == other.reversedI)
				&& (ind == other.ind)
				&& (f == other.f);
	}

	OrthoUnitDir getReversedDir() const {
		return OrthoUnitDir(reversedI, ind, i, n, !f);
	}

};

}

#endif /* ORTHO_UNIT_DIR_HPP_ */
