#pragma once

#include "pch.h"

namespace Aulys 
{
	class Timestep 
	{
	public:
		Timestep(float time = 0.0f) : mTime(time) {
			
		}

		float getSeconds() const { return mTime; }
		float getMilliSeconds() const { return mTime * 1000.0f; }
	private:
		float mTime; // in seconds.
	}; // class Timestep

	inline std::ostream& operator<<(std::ostream& os, const Timestep& t) {
		return os << "Timestep(" << t.getSeconds() << ")";
	}
}; // namespace Aulys
