#pragma once

#include "Aulys.h"

#include <cmath>

#include "GeometryEnum.h"

#define PI 3.14159265358979f
#define defhCWH 0.6584789485f
#define defhCWK 0.5773502692f

namespace App 
{
	constexpr float piOver(float n) {
		return n == -1 ? 0 : PI / n;
	};

	template<typename T>
	constexpr T lorentzDot(const glm::vec<3, T>& v, const glm::vec<3, T>& w) {
		return v.x * w.x + v.y * w.y - v.z * w.z;
	};

	template<typename T>
	constexpr T lorentzDot(const glm::vec<4, T>& v, const glm::vec<4, T>& w) {
		return v.x * w.x + v.y * w.y + v.z * w.z - v.w * w.w;
	};

	template<typename T>
	constexpr T sphericalDot(const glm::vec<3, T>& v, const glm::vec<3, T>& w) {
		return v.x * w.x + v.y * w.y + v.z * w.z;
	};

	template<typename T>
	constexpr T sphericalDot(const glm::vec<4, T>& v, const glm::vec<4, T>& w) {
		return v.x * w.x + v.y * w.y + v.z * w.z + v.w * w.w;
	};


	template<typename T> 
	constexpr T length(const glm::vec<3, T>& v) {
			return sqrt(lorentzDot(v,v));
	}

	template<typename T> 
	constexpr T length(const glm::vec<4, T>& v) {
			return sqrt(lorentzDot(v,v));
	}

	template<class T> 
	constexpr T direction(const T& v) {
		return v * (float)(1 / length(v));
	};

	template<typename T>
	constexpr T dotg(const glm::vec<3, T>& v, const glm::vec<3, T>& w) {
		return v.x * w.x + v.y * w.y + v.z * w.z;
	};

	template<typename T>
	constexpr T dotg(const glm::vec<4, T>& v, const glm::vec<4, T>& w) {
		return v.x * w.x + v.y * w.y + v.z * w.z + v.w * w.w;
	};

	template<typename T>
	constexpr auto dotg(Geometry::V g, const T& v, const T& w) {
		if(g == Geometry::Hyperbolic) {
			return lorentzDot(v, w);
		}
		else if(g == Geometry::Euclidean) {
			return dotg(v, w);
		}
		else if(g == Geometry::Spherical) {
			return sphericalDot(v, w);
		}
		else {
			AU_ASSERT(false, "The geometry value {0} passed in isn't recognised by this function."
					" Check your arguments.", g);
			return 0.0f;
		}
	};

	template<typename T>
	constexpr float length(Geometry::V g, const T& v) {
		return pow( fabs( dotg(g, v, v) ), 0.5f); // Check if there's a float absolute value?
	};

	template<typename T>
	constexpr auto normalize(Geometry::V g, const T& v) {
		return v / length(g, v);
	};

	glm::mat4 translateByVector(Geometry::V geometry, glm::vec3 v);
}; // namespace App
