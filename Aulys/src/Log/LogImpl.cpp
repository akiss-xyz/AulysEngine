#include "src/pch.h"

#include "LogImpl.h"

namespace Aulys
{
	Ref<Logset> Log::mCoreLogset = std::make_shared<Logset>(
		spdlog::stdout_color_mt(LOGGING_CORE_HEADER_NAME),
		spdlog::stdout_color_mt(LOGGING_CORE_LOGGER_NAME));
	Ref<Logset> Log::mClientLogset = std::make_shared<Logset>(
		spdlog::stdout_color_mt(LOGGING_CLIENT_HEADER_NAME),
		spdlog::stdout_color_mt(LOGGING_CLIENT_LOGGER_NAME));

	void Log::Init()
	{
		spdlog::flush_every(std::chrono::seconds(5));

		auto filemtSinkLogger = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
			Path("logs/log.txt").get(), 1048576 * 5, 3);
		filemtSinkLogger->set_level(spdlog::level::trace);

		Log::mClientLogset->logger->set_level(spdlog::level::trace);
		Log::mClientLogset->logger->set_pattern(LOGGING_CLIENT_LOGGER_PATTERN);
		Log::mClientLogset->logger->sinks()[0]->set_level(spdlog::level::info);
		Log::mClientLogset->logger->sinks().push_back(filemtSinkLogger);

		Log::mClientLogset->header->set_level(spdlog::level::trace);
		Log::mClientLogset->header->set_pattern(LOGGING_CLIENT_HEADER_PATTERN);
		Log::mClientLogset->header->sinks()[0]->set_level(spdlog::level::info);
		Log::mClientLogset->header->sinks().push_back(filemtSinkLogger);

		Log::mCoreLogset->header->set_level(spdlog::level::trace);
		Log::mCoreLogset->header->set_pattern(LOGGING_CORE_HEADER_PATTERN);
		Log::mCoreLogset->header->sinks()[0]->set_level(spdlog::level::info);
		Log::mCoreLogset->header->sinks().push_back(filemtSinkLogger);

		Log::mCoreLogset->logger->set_level(spdlog::level::trace);
		Log::mCoreLogset->logger->set_pattern(LOGGING_CORE_LOGGER_PATTERN);
		Log::mCoreLogset->logger->sinks()[0]->set_level(spdlog::level::info);
		Log::mCoreLogset->logger->sinks().push_back(filemtSinkLogger);
	};

	std::string Log::indent(const std::string& msg) {
		std::istringstream buf(msg);
		std::stringstream output;
		std::string line;

		std::getline(buf, line);
#ifdef LOGGING_PREPEND_EMPTY_LINE
		output << '\n';
#endif
		output << LOGGING_INDENT << line;
		while(std::getline(buf, line)) {
			if(line.empty()) {
				output << LOGGING_EMPTY_LINE;
			} else {
				output << '\n' << LOGGING_INDENT << line ;
			}
		}
#ifdef LOGGING_POSTPEND_EMPTY_LINE
		output << '\n';
#endif
		return output.str();
	}

	void Log::flush() {
		mClientLogset->header->flush();
		mClientLogset->logger->flush();
		mCoreLogset->header->flush();
		mCoreLogset->logger->flush();
	};
}; // namespace Aulys
