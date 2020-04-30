#pragma once

#include "pch.h"

#include "Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <mutex>

#include "glm/glm.hpp"

namespace Aulys {

	struct Logset {
	public:
		Logset(std::shared_ptr<spdlog::logger> header, std::shared_ptr<spdlog::logger> logger)
			: header(header), logger(logger) {};

		std::shared_ptr<spdlog::logger> header;
		std::shared_ptr<spdlog::logger> logger;
	};

	class Log
	{
	public:
		static void Init();

		static std::string indent(const std::string& msg);

		static void flush();

		static std::shared_ptr<Logset> mClientLogset;
		static std::shared_ptr<Logset> mCoreLogset;
	};
/**************************************************************************************************/
/*** Common formatters ***/
/**************************************************************************************************/

	inline std::ostream& operator<<(std::ostream& os, const glm::vec2& v) {
		return os << "{" << v.x << ", " << v.y << "}";
	}

	inline std::ostream& operator<<(std::ostream& os, const glm::vec3& v) {
		return os << "{" << v.x << ", " << v.y << ", " << v.z << "}";
	}

	inline std::ostream& operator<<(std::ostream& os, const glm::vec4& v) {
		return os << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
	}

	inline std::string print(const glm::mat4& m) {
		std::stringstream ss;
		ss << "["
			<< "{" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << "}"
			<< "{" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << "}"
			<< "{" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << "}"
			<< "{" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << "}"
			<< "]";
		return ss.str();
	}

	inline std::string print(const glm::vec4& v) {
		std::stringstream ss;
		ss << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
		return ss.str();
	}

	inline std::string print(const glm::vec3& v) {
		std::stringstream ss;
		ss << "{" << v.x << ", " << v.y << ", " << v.z << "}";
		return ss.str();
	}

	inline std::ostream& operator<<(std::ostream& os, const glm::mat4& m) {

		return os << print(m);
	}

	inline std::ostream& operator<<(std::ostream& os, const std::array<glm::mat4, 6>& m) {
		return os << "{" << print(m[0]) << ",\n" << print(m[1]) << ",\n"
			<< print(m[2]) << ",\n" << print(m[3]) << ",\n"
			<< print(m[4]) << ",\n" << print(m[5]) << "}";
	}

	inline std::ostream& operator<<(std::ostream& os, const std::array<glm::mat4, 4>& m) {
		return os << "{" << print(m[0]) << ",\n" << print(m[1]) << ",\n"
			<< print(m[2]) << ",\n" << print(m[3]) << "}";
	}


	inline std::ostream& operator<<(std::ostream& os, const std::array<glm::vec4, 3>& m) {
		return os << "{" << print(m[0]) << ",\n" << print(m[1]) << ",\n" << print(m[2]) << "}";
	}

	inline std::ostream& operator<<(std::ostream& os, const std::array<glm::vec4, 4>& m) {
		return os << "{" << print(m[0]) << ",\n" << print(m[1]) << ",\n" << print(m[2]) << ",\n"
			<< print(m[3]) << "}";
	}

	inline std::ostream& operator<<(std::ostream& os, const std::array<glm::vec4, 5>& m) {
		return os << "{" << print(m[0]) << ",\n" << print(m[1]) << ",\n" << print(m[2]) << ",\n"
			<< print(m[3]) << ",\n" << print(m[4]) << "}";
	}

	inline std::ostream& operator<<(std::ostream& os, const std::array<glm::vec3, 4>& m) {
		return os << "{" << print(m[0]) << ",\n" << print(m[1]) << ",\n" << print(m[2]) << ",\n"
			<< print(m[3]) << "}";
	};

};

template<class... Args>
inline void AU_HEADER_TRACE__(const std::string& msg, Args... args) {
	std::stringstream ss; // Create the input string.
	ss << LOGGING_CORE_HEADER_FORMAT << " " << msg;
	// Then call trace with the log args.
	::Aulys::Log::mCoreLogset->header->trace(
		ss.str(),
		args...
	);
}

template<class... Args>
inline void AU_LOG_TRACE__(const std::string& msg, Args... args) {
	::Aulys::Log::mCoreLogset->logger->trace(
		::Aulys::Log::indent(msg),
		args...
	);
}

template<class... Args>
inline void AU_HEADER_INFO__(const std::string& msg, Args... args) {
	std::stringstream ss; // Create the input string.
	ss << LOGGING_CORE_HEADER_FORMAT << " " << msg;
	// Then call INFO with the log args.
	::Aulys::Log::mCoreLogset->header->info(
		ss.str(),
		args...
	);
}

template<class... Args> 
inline void AU_LOG_INFO__(const std::string& msg, Args... args) {
	::Aulys::Log::mCoreLogset->logger->info(
		::Aulys::Log::indent(msg),
		args...
	);
}

template<class... Args>
inline void AU_HEADER_WARN__(const std::string& msg, Args... args) {
	std::stringstream ss; // Create the input string.
	ss << LOGGING_CORE_HEADER_FORMAT << " " << msg;
	// Then call warn with the log args.
	::Aulys::Log::mCoreLogset->header->warn(
		ss.str(),
		args...
	);
}

template<class... Args>
inline void AU_LOG_WARN__(const std::string& msg, Args... args) { 
	::Aulys::Log::mCoreLogset->logger->warn(
		::Aulys::Log::indent(msg),
		args...
	);
}

template<class... Args>
inline void AU_HEADER_ERROR__(const std::string& msg, Args... args) {
	std::stringstream ss; // Create the input string.
	ss << LOGGING_CORE_HEADER_FORMAT << " " << msg;
	// Then call error with the log args.
	::Aulys::Log::mCoreLogset->header->error(
		ss.str(),
		args...
	);
}

template<class... Args>
inline void AU_LOG_ERROR__(const std::string& msg, Args... args) {
	::Aulys::Log::mCoreLogset->logger->error(
		::Aulys::Log::indent(msg),
		args...
	);
}


// Aulys logs
template<class... Args>
inline void HEADER_TRACE__(const std::string& msg, Args... args) {
	std::stringstream ss; // Create the input string.

	ss << LOGGING_CORE_HEADER_FORMAT << " " << msg;
	// Then call trace with the log args.
	::Aulys::Log::mClientLogset->header->trace(
		ss.str(),
		args...
	);
}

template<class... Args>
inline void LOG_TRACE__(const std::string& msg, Args... args) {
	::Aulys::Log::mClientLogset->logger->trace(
		::Aulys::Log::indent(msg),
		args...
	);
}

template<class... Args>
inline void HEADER_INFO__(const std::string& msg, Args... args) {
	std::stringstream ss; // Create the input string.
	ss << LOGGING_CORE_HEADER_FORMAT << " " << msg;
	// Then call INFO with the log args.
	::Aulys::Log::mClientLogset->header->info(
		ss.str(),
		args...
	);
}

template<class... Args>
inline void LOG_INFO__(const std::string& msg, Args... args) {
	::Aulys::Log::mClientLogset->logger->info(
		::Aulys::Log::indent(msg),
		args...
	);
}

template<class... Args>
inline void HEADER_WARN__(const std::string& msg, Args... args) {
	std::stringstream ss; // Create the input string.
	ss << LOGGING_CORE_HEADER_FORMAT << " " << msg;
	// Then call warn with the log args.
	::Aulys::Log::mClientLogset->header->warn(
		ss.str(),
		args...
	);
}

template<class... Args>
inline void LOG_WARN__(const std::string& msg, Args... args) {
	::Aulys::Log::mClientLogset->logger->warn(
		::Aulys::Log::indent(msg),
		args...
	);
}
 
template<class... Args>
inline void HEADER_ERROR__(const std::string& msg, Args... args) {
	std::stringstream ss; // Create the input string.
	ss << LOGGING_CORE_HEADER_FORMAT << " " << msg;
	// Then call error with the log args.
	::Aulys::Log::mClientLogset->header->error(
		ss.str(),

		args...
	);
}

template<class... Args>
inline void LOG_ERROR__(const std::string& msg, Args... args) {
	::Aulys::Log::mClientLogset->logger->error(
		::Aulys::Log::indent(msg),
		args...
	);
}
