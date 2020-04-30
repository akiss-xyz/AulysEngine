#pragma once

#include "Maths.h"
#include "Geometry.h"
#include "GeometryMaths.h"
#include "Simplex.h"

namespace App 
{

	inline void addPointLightObj(Geometry::V g, int i, glm::vec3 pos, glm::vec4 colorInt,
			Aulys::Ref<std::array<glm::vec4, 4>> lightPositions, Aulys::Ref<std::array<glm::vec4, 5>>& lightIntensities) {
		(*lightPositions)[i] = constructPointInGeometry(g, direction(pos), length(pos));
		(*lightIntensities)[i] = colorInt;
	}

	inline auto initLights(Geometry::V g) {
		auto lightPositions = std::make_shared<std::array<glm::vec4, 4>>();
		auto lightIntensities = std::make_shared<std::array<glm::vec4, 5>>();
		addPointLightObj(g, 0, { 0.0f,  0.0f, 1.0f }, { 0.0f,  0.0f,  1.0f,  2.0f },
				lightPositions, lightIntensities );
		addPointLightObj(g, 1, { 1.2f,  0.0f,  0.0f }, { 1.0f,  0.0f,  0.0f,  2.0f },
				lightPositions, lightIntensities );
		addPointLightObj(g, 2, { 0.0f,  1.1f,  0.0f }, { 0.0f,  1.0f,  0.0f,  2.0f },
				lightPositions, lightIntensities );
		addPointLightObj(g, 3, {-1.2f,  0.0f,  0.0f }, { 1.0f,  1.0f,  1.0f,  2.0f },
				lightPositions, lightIntensities );
		(*lightIntensities)[4] = { 0.49f, 0.28f, 1.0f, 2.0f };
		return std::make_pair(lightPositions, lightIntensities);
	}

/**************************************************************************************************/
/*** Objects ***/
/**************************************************************************************************/
	
	namespace ObjectType 
	{
		enum V {
			Empty = -1,
			Sphere = 0,
			Ellipsoid = 1,
		};
	}; // namespace ObjectType

	template<unsigned int count>
	struct Objects {
		Objects() = default;
		Objects(std::array<glm::mat4, count> objectBoosts,
			std::array<glm::mat4, count> invObjectBoosts,
			std::array<glm::vec3, count> objectRadii,
			std::array<ObjectType::V, count> objectTypes)
				: objectBoosts(objectBoosts), invObjectBoosts(invObjectBoosts),
				objectRadii(objectRadii), objectTypes(objectTypes) {};

		std::array<glm::mat4, count> objectBoosts = {};
		std::array<glm::mat4, count> invObjectBoosts = {};
		std::array<glm::vec3, count> objectRadii = {};
		std::array<ObjectType::V, count> objectTypes = {};

		int getCount() const { return count; } 
	};

	template<unsigned int count>
	[[nodiscard]] inline Aulys::Ref<Objects<count>> initObjects(Geometry::V g) noexcept {
		auto objects = std::make_shared<Objects<count>>();

		addSphereObject(0, g, {0.0f, -0.7f, 0.0f}, 0.2f, *objects);

		for(unsigned int i = 1; i < count; i++) {
			addEmptyObject(i, *objects);
		}

		return objects;
	}

	template<unsigned int count>
	inline auto addEmptyObject(unsigned int index, Objects<count>& objects) {
		objects.objectBoosts[index] = glm::mat4(1.0f);
		objects.invObjectBoosts[index] = glm::mat4(1.0f);
		objects.objectRadii[index] = glm::vec3(0.0f, 0.0f, 0.0f);
		objects.objectTypes[index] = ObjectType::Empty;
	}

	template<unsigned int count>
	inline auto addSphereObject(unsigned int index, Geometry::V g, const glm::vec3 pos, 
			const float radius, 
			Objects<count>& objects) {
		auto objectMatrix = translateByVector(g, pos);
		objects.objectBoosts[index] = objectMatrix;
		objects.invObjectBoosts[index] = inverse(objectMatrix);
		objects.objectRadii[index] = glm::vec3(radius, radius, radius);
		objects.objectTypes[index] = ObjectType::Sphere;
	}

	template<unsigned int count>
	inline auto addEllipsoidObject(unsigned int index, Geometry::V g, const glm::vec3 pos,
			const glm::vec3 radii, Objects<count>& objects) {
		glm::mat4 objectMatrix = glm::translate(glm::mat4(1.0f), pos);

		float buf[16] = {radii[0], 0.0f,        0.0f,        0.0f,
		                 0.0f,     radii[1],    0.0f,        0.0f, 
		                 0.0f,     0.0f,        radii[2],    0.0f,
		                 0.0f,     0.0f,        0.0f,        1.0f};
		glm::mat4 scaleMatrix = glm::make_mat4(buf);

		objectMatrix = scaleMatrix * objectMatrix;
		objects.objectBoosts[index] = objectMatrix;
		objects.invObjectBoosts[index] = inverse(objectMatrix);
		objects.objectRadii[index] = radii;
		objects.objectTypes[index] = ObjectType::Ellipsoid;
	}

	struct InitGeneratorsReturnV {
		InitGeneratorsReturnV(std::array<glm::vec4, 4> simplexMirrors,
				std::array<glm::vec4, 4> simplexDualPoints,
				std::array<glm::vec4, 3> halfCubeDualPoints,
				std::array<glm::mat4, 6> invGens)
			: simplexMirrors(simplexMirrors), simplexDualPoints(simplexDualPoints),
			halfCubeDualPoints(halfCubeDualPoints), invGens(invGens) {};

		std::array<glm::vec4, 4> simplexMirrors;
		std::array<glm::vec4, 4> simplexDualPoints;
		std::array<glm::vec4, 3> halfCubeDualPoints;
		std::array<glm::mat4, 6> invGens;
	};

	inline InitGeneratorsReturnV initGenerators(uint32_t p, uint32_t q, uint32_t r, float hCWK) {
		auto g = getGeometry(p, q, r);
		bool isCubical = ( p == 4 && q == 3 );

		std::array<glm::vec4, 4> simplexMirrors{};
		std::array<glm::vec4, 4> simplexDualPoints{};
		std::array<glm::vec4, 3> hCDP{};
		std::vector<glm::vec4> controllerDualPoints{};
		std::array<glm::mat4, 6> invGens{};

		if( isCubical ) {
			auto invHCWK = 1.0f / hCWK;
			hCDP[0] = glm::vec4(invHCWK, 0.0f,    0.0f,    1.0f);
			hCDP[1] = glm::vec4(0.0f,    invHCWK, 0.0f,    1.0f);
			hCDP[2] = glm::vec4(0.0f,    0.0f,    invHCWK, 1.0f);
			if( g != Geometry::Euclidean) {
				for (uint32_t i = 0; i < 3; i++) {
					hCDP[i] = normalize(g, hCDP[i]);
				}
			}

			invGens = inverseCubeGenerators(createCubeGenerators(g));

			for( uint32_t i = 0; i < 4; i++) {
				simplexMirrors[i] = glm::vec4(glm::vec3(0.0f), 1.0f);
				simplexDualPoints[i] = glm::vec4(glm::vec3(0.0f), 1.0f);
			}
		}
		else { /* Isn't cubical, so the p,q,r passed in doesn't specify a cubical geometry! */
			simplexMirrors = simplexFacetsKlein(p, q, r);

			for(uint32_t i = 0; i < 4; i++) {
				simplexDualPoints[i] = planeDualPoint(g, simplexMirrors[i]);
			}
			
			auto inverseGenerators = simplexInverseGenerators(g, simplexMirrors);

			for(uint32_t i = 0; i < 2; i++) {
				inverseGenerators.push_back( translateByVector(g, glm::vec3(0.0f)) );
			}

			for (int i = 0; i < 6; i++) {
				invGens[i] = inverseGenerators[i];
			}
		}

		for(uint32_t i = 0; i < 6; i++) {
			controllerDualPoints.push_back( glm::vec4(1.0f) );
		}
		return InitGeneratorsReturnV{ simplexMirrors, simplexDualPoints, hCDP, invGens };
	}

	inline auto initGenerators(uint32_t p, uint32_t q, uint32_t r) {
		return initGenerators(p, q, r, defhCWK);
	}

}; // namespace App
