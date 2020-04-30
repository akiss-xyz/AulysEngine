#pragma once

#include "Log/LogImpl.h"

#include "imgui.h"

#define LOG_CONSOLE_RAWCOLOR 0.8f, 0.6f, 0.6f, 1.0f
#define LOG_CONSOLE_COMMANDCOLOR 0.4f, 0.9f, 0.4f, 1.0f

namespace Aulys {
	struct LogItem {
		LogItem(const std::string& message, spdlog::level::level_enum lvl = spdlog::level::trace, bool isRaw = false, bool isCommand = false) : message(message), lvl(lvl), isRaw(isRaw), isCommand(isCommand) {};

		std::string message;
		spdlog::level::level_enum lvl;
		bool isRaw = false;
		bool isCommand = false;
	};

	class ConsoleSink : public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		struct CommandItem {
			CommandItem(const std::string& command, std::function<uint32_t(std::vector<std::string> argv, ConsoleSink* sink)> f)
				: command(command), fn{ f } {};;

			const std::string command;
			std::function<uint32_t(std::vector<std::string>, ConsoleSink*)> fn;
		};

		std::vector<CommandItem> commands;

		ConsoleSink& operator<<(const char* msg);

		ConsoleSink& operator<<(const std::string& msg);

		ConsoleSink& operator<<(const LogItem& msg);

		ConsoleSink(const std::string& windowName = "ConsoleSink");

		void draw();

		int callCommand(const char* line);

		void readReturnCode(int code);
		
		// Helper fn from 
		static int Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }

		int textEditCallback(ImGuiInputTextCallbackData* data);

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override;

		void flush_() override { };

	private:
		char inputBuf[256] = "";
		std::vector<std::string> history;
		int historyIndex = -1;
		std::vector<LogItem> lines;
		bool autoScroll = true;
		bool scrollToBottom = false;
		std::string mWindowName;

	}; // class ConsoleSink : public spdlog::sinks::base_sink<std::mutex>
};
