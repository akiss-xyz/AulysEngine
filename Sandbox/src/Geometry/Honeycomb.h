#pragma once

#include "Aulys.h"

#include "GeometryMaths.h"

namespace App 
{
	float piHPQCalc(uint32_t p, uint32_t q) {
		return acos(sqrt(pow(cos(PI/p), 2) + pow(cos(PI/q), 2)));
	}

	/* Returns:
	 *   - 0.0f is an error value, you'll never get it otherwise.
	 * */
	float inRadiusCalc(uint32_t p, uint32_t q, uint32_t r, Geometry::V g) {
		float inRadius = sin(piOver(p)) * cos(piOver(r)) / sin(piHPQCalc(p, q));

		switch(g) {
			case Geometry::Hyperbolic:
				return acosh(inRadius);
			case Geometry::Euclidean:
				return 1.0f; // m_euclideanScale
			case Geometry::Spherical:
				return acos(inRadius);
			default:
				AU_ASSERT(false, "Geometry value g=\"{0}\" (could be passed in or calculated from"
					" p={1}, q={2}, r={3}) isn't appropriate for this function."
					" Check your arguments.", g, p, q, r);
				return 0.0f;
		}
	}

	float inRadiusCalc(uint32_t p, uint32_t q, uint32_t r) {
		return inRadiusCalc(p, q, r, getGeometry(p, q, r));
	}

	float midRadiusCalc(uint32_t p, uint32_t q, uint32_t r, Geometry::V g, float inRadius) {
		switch(g) {
			case Geometry::Hyperbolic:
				return asinh(sinh(inRadius) / sin(piOver(r)));
			case Geometry::Euclidean:
				return sqrt(2) * 1.0f;
			case Geometry::Spherical:
				return asin(sin(inRadius) / sin(piOver(r)));
			default:
				AU_ASSERT(false, "Geometry value g=\"{0}\" (could be passed in or calculated from"
					" p={1}, q={2}, r={3}) isn't appropriate for this function."
					" Check your arguments.", g, p, q, r);
				return 0.0f;
		}
	}

	float midRadiusCalc(uint32_t p, uint32_t q, uint32_t r, Geometry::V g) {
		return midRadiusCalc(p, q, r, g, inRadiusCalc(p, q, r, g));
	}

	float midRadiusCalc(uint32_t p, uint32_t q, uint32_t r) {
		return midRadiusCalc(p, q, r, getGeometry(p, q, r));
	}
}; // namespace App

