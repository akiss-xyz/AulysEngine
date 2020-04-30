#include "src/pch.h"

#include "Core.h"

namespace Aulys 
{
	std::string Path::replaceLinuxSlashToWinSlash(const std::string& s) {
		std::stringstream ss;
		for (uint32_t i = 0; i < s.length(); i++) {
			if(s[i] == '/')
			{
				ss << '\\';
			}
			else {
				ss << s[i];
			}
		}
		return ss.str();
	};

	// This is a bit defensive programming-y, but this function is useless. 
	// You should only pass Linux-style paths to the engine.
	std::string Path::replaceWinSlashToLinuxSlash(const std::string& s) {
		std::stringstream ss;
		bool foundWinSlash = false;
		for (uint32_t i = 0; i < s.length(); i++) {
			if(s[i] == '\\')
			{
				foundWinSlash = true;
				ss << '/';
			}
			else {
				ss << s[i];
			}
		}

		if (foundWinSlash) {
			LWL("In handling path \"{0}\", we found some Windows style slashes. Aulys uses"
				" Linux style slashes for file paths - we've corrected the path automatical"
				"ly for you but please use the proper path.", s);
		}
		return ss.str();
	};
}; // namespace Aulys
