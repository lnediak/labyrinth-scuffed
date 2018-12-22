#ifndef SRC_LABYRINTH_CORE_CONVEX_OBJECT_HPP_
#define SRC_LABYRINTH_CORE_CONVEX_OBJECT_HPP_

#include <algorithm>
#include <limits>
#include <utility>
#include <valarray>
#include <vector>

#include <stdint.h>

#include <cstring>

namespace convex_object {

/**
 * Represents a half-space according to:
 * https://en.wikipedia.org/wiki/Convex_polytope
 * basically, <= threshold is solid, > threshold is not.
 */
class LinearConstraint {

public:
	size_t numDims;
	std::valarray<double> normal;
	double threshold;

	LinearConstraint(size_t numDims, std::valarray<double> normal, double threshold):
		numDims(numDims), normal(normal), threshold(threshold) {}

};

class Ray {

public:
	size_t numDims;
	std::valarray<double> vector;
	std::valarray<double> point;

	Ray(size_t numDims, std::valarray<double> vector, std::valarray<double> point):
		numDims(numDims), vector(vector), point(point) {}

};

class ConvexObject {

	size_t numDims;
	std::vector<LinearConstraint> constraints;

public:
	ConvexObject(size_t numDims): numDims(numDims), constraints() {}

	size_t getNumDims() const {
		return numDims;
	}

	void addConstraint(const LinearConstraint& constraint) {
		constraints.push_back(constraint);
	}

	struct RayIntersection {

		// if the intersection is forward, then all greater values of t
		// satisfy the constraint of the hyperplane intersected, while lesser
		// values do not.
		// if the intersection is non-forward, all greater values of t
		// do not satisfy the constraint of the hyperplane,
		// but lesser values of t do.
		bool isForward;
		std::valarray<double> point;
		double t;

		bool operator<(const RayIntersection& other) const noexcept {
			return t < other.t;
		}

	};

	/**
	 * If !foundAnything, the return value is meaningless.
	 * Otherwise, it is the point of intersection with this convex object.
	 */
	RayIntersection rayTrace(const Ray& ray, bool* foundAnything) const {
		// let p be the ray's point, r the ray. Let <,> be dot product.
		// <p + tr, normal> = threshold; <tr, normal> = threshold - <p, normal>;
		// t = (threshold - <p, normal>) / <r, normal>
		// 
		// isForward = (<p, normal>) > threshold;

		std::vector<RayIntersection> intersections;
		for (const LinearConstraint& c: constraints) {
			double denominator = (ray.vector * c.normal).sum();
			double numerator = c.threshold - (ray.point * c.normal).sum();
			double t = (std::abs(denominator) < 1e-5)?
					-1000000:
					numerator / denominator;

			RayIntersection intersection;
			intersection.isForward = (ray.point * c.normal).sum() > c.threshold;
			if (t <= 0) {
				intersection.isForward = !intersection.isForward;
			}
			intersection.point = ray.point + t * ray.vector;
			intersection.t = t;
			intersections.push_back(std::move(intersection));
		}

		std::sort(intersections.begin(), intersections.end());

		int64_t lastForward = -1;
		bool encounteredNonForward = false;
		for (size_t i = 0; i < intersections.size(); i++) {
			if (intersections[i].isForward) {
				if (encounteredNonForward) {
					*foundAnything = false;
					return RayIntersection();
				}
				lastForward = i;
			} else {
				encounteredNonForward = true;
			}
		}
		if (lastForward == -1) {
			*foundAnything = false;
			return RayIntersection();
		}
		if (intersections[lastForward].t <= 0) {
			*foundAnything = false;
			return RayIntersection();
		}
		*foundAnything = true;
		return intersections[lastForward];
	}

};

}

#endif /* SRC_LABYRINTH_CORE_CONVEX_OBJECT_HPP_ */
