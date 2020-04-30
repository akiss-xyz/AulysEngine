#include "Simplex.h"

namespace App 
{ 
	float tilingNormalizedCircumRadius(uint32_t p, uint32_t q) {
		auto hypotenuse = getTriangleHypotenuse(p, q);
		auto g = getGeometry2D(p, q);

		switch( g ) {
			case Geometry::Euclidean:
				return EUCLIDEAN_HYPOTENUSE;
			case Geometry::Hyperbolic:
				if(hypotenuse == std::numeric_limits<float>::infinity()) {
					return DISK_RADIUS;
				}
				return hyperbolicToPoincare(hypotenuse) * DISK_RADIUS;
			case Geometry::Spherical:
				return sphericalToStereographic(hypotenuse) * DISK_RADIUS;
			default:
				AU_ASSERT(false, "getGeometry2D(p={0}, q={1}) returned an unexpected value \"{2}\"",
						p, q, g);
				return 1.0f;
		}
	}

	Sphere moveSphere(Geometry::V g, glm::vec3 v, float r) {
		if (g == Geometry::Euclidean) {
			return Sphere(v, r, glm::vec3(0.0f), 0.0f);
		}

		auto p = length(v);
		if (p == 0) {
			return Sphere(v, r, glm::vec3(0.0f), 0.0f);
		}
		const auto numeratorCenter = g == Geometry::Hyperbolic ? (1 - r * r) : (1 + r * r);
		const auto numeratorRadius = g == Geometry::Hyperbolic ? (1 - p * p) : (1 + p * p);
		const auto denom = (1 - p * p * r * r);

		return Sphere(direction(v) * p * numeratorCenter / denom,
			          r * numeratorRadius / denom, glm::vec3(0.0f), 0.0f);
	}

	TilePoints tilePoints(uint32_t q, uint32_t p) {
		auto cellGeometry = getGeometry2D(p, q);
		float circumference = tilingNormalizedCircumRadius(p, q);
		glm::vec3 start = {circumference, 0, 0};
		glm::vec3 axis = {0, 0, 1};
		glm::vec3 end = glm::rotate(glm::mat4(1.0f), 2*PI / p, axis) * glm::vec4(start, 0.0f);

		glm::vec3 segMidpoint;
		glm::vec3 centre{0.0f};
		float radius = 0.0f;

		if( cellGeometry == Geometry::Euclidean ) {
			segMidpoint = ( start + end ) / 2.0f;
		}
		else {
			auto piOverQ = piOver(q);
			float t1 = PI / p;
			float t2 = PI / 2 - piOverQ - t1;
			float factor = ( tan(t1) / tan(t2) + 1.0f ) / 2.0f;
			centre = factor * ( start + end );
			radius = length(centre - start);

			auto mag = length(centre) - radius;
			segMidpoint = normalize(centre) * mag;
		}
		
		return { start, segMidpoint,
			glm::rotate(glm::mat4(1.0f), -PI/2, axis) * glm::vec4(segMidpoint, 0.0f),
			centre, radius };
	}

	std::array<Sphere, 3> interiorMirrors(uint32_t p, uint32_t q) {
		auto [ p1, p2, p3, c, r ] = tilePoints(p, q);

		auto cellGeometry = getGeometry2D(p, q);

		Sphere s1{glm::vec3(0.0f), std::numeric_limits<float>::infinity(), 
			glm::vec3(0.0f, -1.0f, 0.0f), 0.0f};

		Sphere s2;

		if(cellGeometry == Geometry::Euclidean) {
			s2 = { glm::vec3(0.0f), std::numeric_limits<float>::infinity(), p2, length(p2) };
		}
		else if(cellGeometry == Geometry::Hyperbolic || cellGeometry == Geometry::Spherical) {
			s2 = { c, r, glm::vec3(0.0f), 0.0f };
		}
		else {
			AU_ASSERT(false, "getGeometry2D(p={0}, q={1}) returned a value \"{2}\" that we don't support.",
					p, q, cellGeometry);
		}

		Sphere s3 = { glm::vec3(0.0f), std::numeric_limits<float>::infinity(), -p3, 0.0f };

		return { s2, s1, s3 };
	}

	SimplexFacets simplexFacetsUHS(uint32_t p, uint32_t q, uint32_t r) {
		auto g = getGeometry(p, q, r);

		if(g == Geometry::Invalid) {
			AU_ASSERT(false, "We called simplexFacets on an invalid geometry "
					"(calculated by getGeometry(p={0}, q={1}, r={2})).", p,q,r);
		}
		else {
			auto [p1, p2, p3, c, r] = tilePoints(p, q);

			auto cellGeometry = getGeometry2D(p, q);
			Sphere cellMirror{glm::vec3(0.0f), 0.0f, glm::vec3(1.0f), 0.0f};

			if(cellGeometry == Geometry::Euclidean) {
				auto center = p1;
				float radius = length(p2 - p1) / cos( piOver( r ) );
				Sphere cellMirror{center, radius, glm::vec3(1.0f), 0.0f};
			}
			else if(cellGeometry == Geometry::Hyperbolic) {
				auto halfSide = getTrianglePSide(q, p);
				auto mag = asinh( sinh(halfSide) / cos( piOver(r) ) );
			}
			else if (cellGeometry == Geometry::Spherical) {
				auto mag = asin(sin(getTrianglePSide(q, p)) / cos(piOver(r)));

				mag = sphericalToStereographic(mag);
				cellMirror = moveSphere(Geometry::Spherical, p1, mag);
			}
			else {
				AU_ASSERT(false, "cellGeometry (calculated by getGeometry2D(p={0}, q={1}) "
					"didn't give Geometry::Euclidean, Hyperbolic, or Spherical!", p, q);
			}
			auto [one, two, three] = interiorMirrors(p, q);
			return {one, two, three, cellMirror};
		}
	}

	std::array<glm::vec3, 3> circlePoints(glm::vec3 center, float rad) {
		float a = 2 * PI / 3;

		auto p1 = center + glm::vec3( rad, 0.0f, 0.0f );
		auto p2 = center + glm::vec3( rad * cos(a), rad * sin(a), 0);
		auto p3 = center + glm::vec3( rad * cos(2*a), rad * sin(2*a), 0);
		return {p1, p2, p3};
	}

	std::array<glm::vec3, 3> planePoints(Sphere f) {
		auto mid = f.normal * f.offset;
		// Rotate f.normal about {0, 0, 1} by pi/2.
		glm::vec3 offset = glm::rotate(glm::mat4(1.0f), PI / 2.0f, glm::vec3(0, 0, 1)) *
			glm::vec4(f.normal, 0.0f);
		glm::vec3 start = mid + offset;
		glm::vec3 end = mid - offset;
		return {start, mid, end};
	}

	auto kleinFromUHS(Sphere f) {
		std::array<glm::vec3, 3> idealPoints;
		if (f.radius == std::numeric_limits<float>::infinity() ) {
			if(f.offset < pow(10, -7)) {
				return glm::vec4( f.normal, 0.0f );
			}
			idealPoints = planePoints(f);
		}
		else {
			idealPoints = circlePoints(f.center, f.radius);
		}

		Sphere plane;
		
		plane.setPlaneFromPoints(
			UHSToPoincare( idealPoints[0] ),
			UHSToPoincare( idealPoints[1] ),
			UHSToPoincare( idealPoints[2] )
		);

		return glm::vec4(plane.normal.x, plane.normal.y, plane.normal.z, plane.offset);
	}

	std::array<glm::vec4, 4> simplexFacetsKlein(uint32_t p, uint32_t q, uint32_t r) {
		auto facetsUHS = simplexFacetsUHS(p, q, r);
		glm::vec4 vertexFacet = kleinFromUHS( facetsUHS.s1 );
		glm::vec4 edgeFacet = kleinFromUHS( facetsUHS.s2 );
		glm::vec4 faceFacet = kleinFromUHS( facetsUHS.s3 );
		glm::vec4 cellFacet = kleinFromUHS( facetsUHS.cellMirror );

		return {vertexFacet, edgeFacet, faceFacet, cellFacet};
	}


	glm::mat4 oneGen(Geometry::V g, glm::vec4 f) {
		auto r1 = reflectInFacet(g, f, {1,0,0,0});
		auto r2 = reflectInFacet(g, f, {0,1,0,0});
		auto r3 = reflectInFacet(g, f, {0,0,1,0});
		auto r4 = reflectInFacet(g, f, {0,0,0,1});

		float buf[16] = {
			r1.x, r1.y, r1.z, r1.w,
			r2.x, r2.y, r2.z, r2.w,
			r3.x, r3.y, r3.z, r3.w,
			r4.x, r4.y, r4.z, r4.w,
		};

		auto m = glm::make_mat4(buf);
		return m;
	}

	std::vector<glm::mat4> simplexInverseGenerators(Geometry::V g,
			std::array<glm::vec4, 4> kleinFacets) {
		return { oneGen(g, kleinFacets[0]), oneGen(g, kleinFacets[1]),
			oneGen(g, kleinFacets[2]), oneGen(g, kleinFacets[3]), };
	}
}; // namespace Sandbox
