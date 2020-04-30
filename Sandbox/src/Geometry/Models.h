#pragma once

#include "Aulys.h"
#include "Maths.h"

#include <complex>

namespace App 
{
	class Mobius 
	{
	public:
		Mobius(std::complex<float> a, std::complex<float> b, std::complex<float> c, 
				std::complex<float> d) : a(a), b(b), c(c), d(d) {};

		template<typename T>
		Mobius scaleComponents(T n);

		Mobius normalize();

		[[nodiscard]] Mobius inverse() const;

		template<typename T>
		T operator*(T z);

		static Mobius PoincareToUHS();

		static Mobius UHSToPoincare();
	private:
		std::complex<float> a;
		std::complex<float> b;
		std::complex<float> c; 
		std::complex<float> d;
	}; // class Mobius

	glm::vec3 cartesianToSpherical(glm::vec3 v);

	glm::vec3 sphericalToCartesian(glm::vec3 v);

	glm::vec3 transformHelper(glm::vec3 v, Mobius m);

	glm::vec3 UHSToPoincare(glm::vec3 vUHS);
}; // namespace App
