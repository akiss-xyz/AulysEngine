#include "src/pch.h"

#include "Event.h"

namespace Aulys 
{
	std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
}; // namespace Aulys
