#pragma once

#include "Aulys.h"

#include "Maths.h"
#include "Geometry.h"
#include "GeometryMaths.h"
#include "Models.h"

namespace App 
{
	float tilingNormalizedCircumRadius(uint32_t p, uint32_t q);

	struct TilePoints {
		glm::vec3 start;
		glm::vec3 midpoint;
		glm::vec3 end;
		glm::vec3 centre;
		float radius;
	};
	
	TilePoints tilePoints(uint32_t q, uint32_t p);

	struct Sphere {
		glm::vec3 center;
		float radius;
		glm::vec3 normal;
		float offset;

		Sphere() : center(0.0f), radius(0.0f), normal(0.0f), offset(0.0f) {};
		Sphere(glm::vec3 center, float rad, glm::vec3 normal, float offset)
			: center(center), radius(rad), normal(normal), offset(offset) {};

		static glm::vec3 distanePointPlane(glm::vec3 normal, float offset, glm::vec3 point) {
			return (point - (normal * offset)) * normal;
		}

		void setPlaneFromPoints(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
			this->center = {0.0f, 0.0f, 0.0};
			this->radius = std::numeric_limits<float>::infinity();

			this->normal = normalize(glm::cross((c - a), (b - a)));
			this->offset = Sphere::distancePointPlane(normal, 0.0f, a);
		}

		template<typename T> 
		static float distancePointPlane(T normal, float offset, T point) {
			return dot((point - offset * normal), normal);
		}

		template<typename T> 
		static float distancePointPlane(Geometry::V g, T normal, float offset, T point) {
			return dotg(g, (point - offset * normal), normal);
		}
	};

	std::array<Sphere, 3> interiorMirrors(uint32_t p, uint32_t q);

	struct SimplexFacets 
	{
		Sphere s1;
		Sphere s2;
		Sphere s3;
		Sphere cellMirror;
	}; // struct SimplexFacets

	SimplexFacets simplexFacetsUHS(uint32_t p, uint32_t q, uint32_t r);

	std::array<glm::vec3, 3> circlePoints(glm::vec3 center, float rad);

	std::array<glm::vec3, 3> planePoints(Sphere f);

	auto kleinFromUHS(Sphere f);

	std::array<glm::vec4, 4> simplexFacetsKlein(uint32_t p, uint32_t q, uint32_t r);

	glm::mat4 oneGen(Geometry::V g, glm::vec4 f);

	std::vector<glm::mat4> simplexInverseGenerators(Geometry::V g,
			std::array<glm::vec4, 4> kleinFacets);
}; // namespace App
