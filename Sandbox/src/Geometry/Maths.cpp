#include "Maths.h"

namespace App {
// 	template<int d, typename T>
// 	constexpr float lorentzDot(const glm::vec<d, T>& v, const glm::vec<d, T>& w) {
// 		if(d == 3) {
// 			return v.x * w.x + v.y * w.y - v.z * w.z;
// 		}
// 		return v.x * w.x + v.y * w.y + v.z * w.z - v.w * w.w;
// 	}
// 	template<int d, typename T>
// 	constexpr float dot(const glm::vec<d, T>& v, const glm::vec<d, T>& w) {
// 		if(d == 3) {
// 			return v.x * w.x + v.y * w.y + v.z * w.z;
// 		}
// 		else if (d == 4) {
// 			return v.x * w.x + v.y * w.y + v.z * w.z + v.w * w.w;
// 		}
// 		return 0.0f; // Big trouble.)
// 	}

// 	template<int d, typename T> 
// 	constexpr float dot(Geometry::V g, const glm::vec<d, T>& v, const glm::vec<d, T>& w) {
// 		if (g == Geometry::Hyperbolic) {
// 			return lorentzDot(v, w);
// 		}
// 		if (g == Geometry::Euclidean) {
// 			return dot(v, w);
// 		}
// 		else {
// 			AU_ASSERT(false, "We dotted in non-hyperbolic or euclidean space!");
// 		}
// 		return 0.0f;
// 	}


	glm::mat4 translateByVector(Geometry::V geometry, glm::vec3 v) {
		auto dx = v.x; auto dy = v.y; auto dz = v.z;
		auto length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

		auto mat = glm::mat4(0.0f);

		if (length == 0) {
			return mat;
		}

		// Normalize the vector
		dx /= length; dy /= length; dz /= length;

		// Start building the matrix
		auto m03 = dx; auto m13 = dy; auto m23 = dz;
		float c1 = sinh(length); float c2 = cosh(length) - 1;

		if (geometry == Geometry::Euclidean) {
			m03 = m13 = m23 = c2 = 0;
			c1 = length;
		}
		else { // Currently hyperbolic only!
			mat[0] = glm::vec4(glm::vec3(0.0f), dx);
			mat[1] = glm::vec4(glm::vec3(0.0f), dy);
			mat[2] = glm::vec4(glm::vec3(0.0f), dz);
			mat[3] = glm::vec4(m03, m13, m23, 0.0f);
			auto matSquared = mat * mat;
			mat = c1 * mat;
			matSquared = c2 * matSquared;
			auto result = glm::mat4(1.0f) + mat + matSquared;
			return result;
		}
		AU_ASSERT(false, "We got asked to translate a vector using an unsupported geometry \"{}\"",
				(int)geometry);
		return mat;
	}
} // namespace App
