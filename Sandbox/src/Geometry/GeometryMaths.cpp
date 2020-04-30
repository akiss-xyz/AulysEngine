#include "GeometryMaths.h"

namespace App 
{

	float horosphereHSDF(Geometry::V g, glm::vec3 samplePoint, glm::vec3 lightPoint, float offset) {
		return log( -dotg(g, samplePoint, lightPoint) ) - offset;
	};

	float horosphereHSDF(Geometry::V g, glm::vec4 samplePoint, glm::vec4 lightPoint, float offset) {
		return log( -dotg(g, samplePoint, lightPoint) ) - offset;
	};

	std::array<glm::mat4, 6> createCubeGenerators(Geometry::V g, float hCWH) {
		return {translateByVector(g, glm::vec3( 2.0f * hCWH,  0.0f,  0.0f)),
		        translateByVector(g, glm::vec3(-2.0f * hCWH,  0.0f,  0.0f)),
		        translateByVector(g, glm::vec3( 0.0f,  2.0f * hCWH,  0.0f)),
		        translateByVector(g, glm::vec3( 0.0f, -2.0f * hCWH,  0.0f)),
		        translateByVector(g, glm::vec3( 0.0f,  0.0f,  2.0f * hCWH)),
		        translateByVector(g, glm::vec3( 0.0f,  0.0f, -2.0f * hCWH))};
	}

	std::array<glm::mat4, 6> createCubeGenerators(Geometry::V g) {
		return createCubeGenerators(g, defhCWH);
	}

	std::array<glm::mat4, 6> inverseCubeGenerators(std::array<glm::mat4, 6> generators) {
		return {generators[1], generators[0], generators[3],
		        generators[2], generators[5], generators[4]};
	}

	glm::vec4 constructSpherePoint(glm::vec3 dir, float distance) {
		auto w = cos(distance);
		return glm::vec4(direction(dir) * (float)sqrt(1 - w * w), w);
	}

	glm::vec4 constructHyperboloidPoint(Geometry::V g, glm::vec3 dir, float distance) {
		float w = cosh(distance);
		float magnitudeSquared = w * w - 1;
		glm::vec3 d = direction(dir) * (float)sqrt(magnitudeSquared);
		return glm::vec4(d.x, d.y, d.z, w);
	}

	glm::vec4 constructPointInGeometry(Geometry::V g, glm::vec3 dir, float distance) {
		switch (g) {
			case Geometry::Euclidean:
				return glm::vec4(direction(dir) * distance, 1);
			case Geometry::Hyperbolic:
				return constructHyperboloidPoint(g, dir, distance);
			case Geometry::Spherical:
				return constructSpherePoint(dir, distance);
			default:
				AU_ASSERT(false, "The parameter Geometry::V g = \"{0}\" is not supported. "
					"(Not one of Geometry::Euclidean, Geometry::Hyperbolic, or "
					"Geometry::Spherical", g);
				return glm::vec4(0.0f);
		}
	}

	float getTriangleSide(Geometry::V g, float alpha, float beta, float gamma) {
		switch(g) {
			case Geometry::Euclidean:
				return 0.0f; // No such thing!
			case Geometry::Hyperbolic:
				return acosh((cos(alpha) + cos(beta) * cos(gamma)) / (sin(beta) * sin(gamma)));	
			case Geometry::Spherical:
				return acos((cos(alpha) + cos(beta) * cos(gamma)) / (sin(beta) * sin(gamma)));
			default:
				AU_ASSERT(false, "Geometry::V g=\"{0}\" isn't supported, check you called with the "
						"correct value.", g);
				return 0.0f;
		}
	}

	float getTrianglePSide(uint32_t p, uint32_t q) {
		auto g = getGeometry2D(p, q);

		if (g == Geometry::Euclidean) {
			return float(sin(piOver(p))) * EUCLIDEAN_HYPOTENUSE;
		}
		//                        gamma,      beta,       alpha
		//                        ^ This is the one we want.
		return getTriangleSide(g, piOver(p), piOver(q), PI / 2.0f);
	}

	float getTriangleHypotenuse(uint32_t p, uint32_t q) {
		auto g = getGeometry2D(p, q);

		if ( g == Geometry::Euclidean ) {
			return EUCLIDEAN_HYPOTENUSE;
		}

		//                        alpha,  beta,         gamma!
		return getTriangleSide(g, PI / 2, piOver( q ), piOver( p ));
	}

	glm::vec4 planeDualPoint(Geometry::V g, const glm::vec4& fKlein) {
		if(abs(fKlein.w) < pow(10.0f, -7)) {
			return {fKlein.x, fKlein.y, fKlein.z, 0.0f};
		}
		float inverseW = 1.0f / fKlein.w;
		return normalize(g,
				glm::vec4(fKlein.x * inverseW, fKlein.y * inverseW, fKlein.z * inverseW, 1.0f));
   	}

	glm::vec4 reflectInFacet(Geometry::V g, const glm::vec4& fKlein, const glm::vec4& vMinkowski) {
		glm::vec4 plane = planeDualPoint(g, fKlein);
		return vMinkowski - (plane * (2 * dotg(g, plane, vMinkowski) / dotg(g, plane, plane)));
	}

	float poincareToHyperbolic(float p) {
		return 2*atanh(p);
	}

	float hyperbolicToPoincare(float h) {
		return tanh(0.5f * h);
	}

	float poincareToKlein(float p) {
		return 2 * p / ( 1 + p * p );
	}

	float sphericalToStereographic(float s) {
		return 2 * atan(0.5f * s);
	}

	float stereographicToGnomonic(float s) {
		return 2 * s / (1 - s * s);
	}

	float geodesicPlaneHSDF(Geometry::V g, glm::vec3 vSample, glm::vec3 vDualPoint, float offset) {
		return asinh(-dotg(g, vSample, vDualPoint));
	}

	int fixOutsideCentralCell(glm::mat4 m, const std::array<glm::mat4, 6>& invGens) {
		auto v = m * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		float dist = dotg(v, v);
		int bestIndex = -1;
		for (int i = 0; i < 6; i++) {
			v = m * inverse(invGens[i]) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			if (float newDist = dotg(v, v); newDist < dist) {
				dist = newDist;
				bestIndex = i;
			}
		}
		if (bestIndex != -1) {
			m = m * inverse(invGens[bestIndex]); // Why do this?
		}
		return bestIndex;
	}

	glm::mat4 gramSchmidt(Geometry::V g, glm::mat4 m) {
		if (g == Geometry::Euclidean) {
			AU_ASSERT(false, "Called gramSchmidt on Euclidean geometry.");
			return glm::mat4{0.0f};
		}

		glm::mat4 n = glm::transpose(m);
		auto temp = glm::vec4{ 0.0f };
		auto temp2 = glm::vec4{ 0.0f };
		for (int i = 0; i < 4; i++) {
			float invRowNorm = 1.0f / length(g, n[i]);
			for (int l = 0; l < 4; l++) {
				n[i][l] = n[i][l] * invRowNorm;
			}
			for (int j = i + 1; j < 4; j++) {
				float component = dotg(g, n[i], n[j]);
				for (int k = 0; k < 4; k++) {
					n[j][k] -= component * n[i][k];
				}
			}
		}
		return glm::transpose(n);
	}

}; // namespace App
