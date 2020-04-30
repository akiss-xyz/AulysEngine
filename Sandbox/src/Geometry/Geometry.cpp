#include "Geometry.h"

namespace App 
{
	// IMPROVEMENT: Could be constexpr if we make our own sin and cos.
	// Note: sadly cmath's sin and cos aren't constexpr because of some legacy things
	// that the library does, for example setting errno.
	Geometry::V getGeometry(int p, int q, int r) {
		auto testOne = sin(piOver(p)) * sin(piOver(r));
		auto testTwo = cos(piOver(q));
		
		if ( p == 4 && q == 3 && r == 4 )
			return Geometry::Euclidean;
		if ( testOne > testTwo ) {
			return Geometry::Spherical;
		}
		return Geometry::Hyperbolic;
	}
	
	Geometry::V getGeometry2D(uint32_t p, uint32_t q) {
		auto test = 1.0f / p + 1.0f / q;
		if ( test == 0.5f ) {
			return Geometry::Euclidean;
		}
		else if ( test > 0.5f ) {
			return Geometry::Spherical;
		}
		return Geometry::Hyperbolic;
	}
}; // namespace App
