#pragma once

#include "Aulys.h"

#include "Maths.h"
#include "GeometryEnum.h"

namespace App 
{
	Geometry::V getGeometry(int p, int q, int r);
	Geometry::V getGeometry2D(uint32_t p, uint32_t q);
}; // namespace App
