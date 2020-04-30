#pragma once

#include "Aulys.h"

#include "Maths.h"
#include "Geometry.h"

#define EUCLIDEAN_HYPOTENUSE 1.0f/3.0f
#define DISK_RADIUS 1.0f

#define HALFIDEALCUBEWIDTHKLEIN 0.5773502692f
#define IDEALCUBECORNERKLEIN glm::vec4(HALFIDEALCUBEWIDTHKLEIN, HALFIDEALCUBEWIDTHKLEIN, HALFIDEALCUBEWIDTHKLEIN, 1.0f)

namespace App 
{
	float horosphereHSDF(Geometry::V g, glm::vec3 samplePoint, glm::vec3 lightPoint, float offset);
	float horosphereHSDF(Geometry::V g, glm::vec4 samplePoint, glm::vec4 lightPoint, float offset);

	std::array<glm::mat4, 6> createCubeGenerators(Geometry::V g, float hCWH);
	std::array<glm::mat4, 6> createCubeGenerators(Geometry::V g);
	std::array<glm::mat4, 6> inverseCubeGenerators(std::array<glm::mat4, 6> generators);

	glm::vec4 constructHyperboloidPoint(glm::vec3 dir, float distance);
	glm::vec4 constructPointInGeometry(Geometry::V g, glm::vec3 dir, float distance);

// 	inline glm::vec4 constructHyperboloidPoint(glm::vec3 dir, float distance);
// 	inline glm::vec4 constructPointInGeometry(Geometry::V g, glm::vec3 dir, float distance);

	float getTriangleSide(Geometry::V g, float alpha, float beta, float gamma);
	float getTrianglePSide(uint32_t p, uint32_t q);
	float getTriangleHypotenuse(uint32_t p, uint32_t q);

	glm::vec4 planeDualPoint(Geometry::V g, const glm::vec4& fKlein);

	// inline glm::vec4 planeDualPoint(Geometry::V g, const glm::vec4& fKlein);
	glm::vec4 reflectInFacet(Geometry::V g, const glm::vec4& fKlein, const glm::vec4& vMinkowski);

	float poincareToHyperbolic(float p);
	float hyperbolicToPoincare(float h);
	float poincareToKlein(float p);
	float sphericalToStereographic(float s);
	float stereographicToGnomonic(float s);

	float geodesicPlaneHSDF(Geometry::V g, glm::vec3 vSample, glm::vec3 vDualPoint, float offset);

	int fixOutsideCentralCell(glm::mat4 m, const std::array<glm::mat4, 6>& gens);
	glm::mat4 gramSchmidt(Geometry::V g, glm::mat4 m);
}; // namespace App
