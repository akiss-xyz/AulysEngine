#include "Models.h"

namespace App 
{

	template<typename T>
	Mobius Mobius::scaleComponents(T n) {
		this->a *= n;
		this->b *= n;
		this->c *= n;
		this->d *= n;
		return *this;
	}

	Mobius Mobius::normalize() {
		return this->scaleComponents( 1.0f / sqrt(a*d - b*c) );
	}

	[[nodiscard]] Mobius Mobius::inverse() const {
		return Mobius(d, -b, -c, a).normalize();
	}

	template<typename T>
	T Mobius::operator*(T z) {
		return (a * z + b) / (c * z + d);
	}

	Mobius Mobius::PoincareToUHS() {
		float t = sqrt(2.0f)/2.0f;
		return Mobius({t, 0}, {0, t}, {0, t}, {t, 0});
	}

	Mobius Mobius::UHSToPoincare() {
		return Mobius::PoincareToUHS().inverse();
	}

	glm::vec3 cartesianToSpherical(glm::vec3 v) {
		return { length(v), acos( v.z / length(v) ), atan2(v.y, v.x) };
	}

	glm::vec3 sphericalToCartesian(glm::vec3 v) {
		return { v.x * sin(v.y) * cos(v.z), v.x * sin(v.y) * sin(v.z), v.x * cos(v.y) };
	}

	glm::vec3 transformHelper(glm::vec3 v, Mobius m) {
		auto spherical = cartesianToSpherical(v);
		auto c1 = std::polar(spherical.x, PI / 2 - spherical.y);
		auto c2 = m * c1;
		auto s2 = glm::vec3(abs(c2), PI / 2 - arg(c2), spherical.z);
		return sphericalToCartesian(s2);
	}

	glm::vec3 UHSToPoincare(glm::vec3 vUHS) {
		return transformHelper(vUHS, Mobius::UHSToPoincare());
	}
}; // namespace App
