#include "src/pch.h"

#include "ConsoleSink.h"

namespace Aulys {
	ConsoleSink& ConsoleSink::operator<<(const char* msg) {
		auto lastline = (lines.size() != 0 && lines.back().message.length() != 0) ? lines.back() : LogItem("\n", spdlog::level::info, false);
		if (lastline.isRaw) {
			lines.back() = LogItem(lastline.message + msg, spdlog::level::info, true);
		} 
		else {
			lines.push_back(LogItem(msg, spdlog::level::info, true));
		}
		return *this;
	}

	ConsoleSink& ConsoleSink::operator<<(const std::string& msg) {
		auto lastline = (lines.size() != 0 && lines.back().message.length() != 0) ? lines.back() : LogItem("\n", spdlog::level::info, false);
		if (lastline.isRaw) {
			lines.back() = LogItem(lastline.message + msg, spdlog::level::info, true);
		} 
		else {
			lines.push_back(LogItem(msg, spdlog::level::info, true));
		}
		return *this;
	}

	ConsoleSink& ConsoleSink::operator<<(const LogItem& msg) {
		std::stringstream ss(msg.message);
		std::string buf;
		while(getline(ss, buf, '\n')) {
			lines.push_back(LogItem(buf, msg.lvl, msg.isRaw, msg.isCommand));
		}
		return *this;
	}

	ConsoleSink::ConsoleSink(const std::string& windowName) : mWindowName(windowName) {
		this->commands.push_back(
			CommandItem( "help", [this](std::vector<std::string> argv, ConsoleSink* sink) -> uint32_t {
				*sink << "Welcome to the Aulys Engine!\n" << "This is the help menu for the in-engine ConsoleSink command utility.\n"
					<< "The available commands are:\n\n";

				for (auto item : commands) {
					*sink << item.command << "\n";
				}

				*sink << "\n" << "Adding your own command is simple:\n"
					<< "`myConsoleSink->commands.push_back({\"COMMANDNAME\", myCommandFunction})`\n"
					<< "Where `myCommandFunction` is a `bool(std::vector<std::string> argv, void* sink)`. For example, using a lambda:\n"
					<< "`myConsoleSink->commands.push_back(\"HI\", [](std::vector<std::string> argv, void* sink) { *(ConsoleSink*)sink << \"Hello!\"; return 0; })`\n"
					<< "\nThis way you can even add captures into your lambda to create commands that modify literally anything in your game, such as how the `swapTexture` command\n"
					<< "captures UIOverlay in order to create textures on the fly.\n"
					<< "In theory, you could just make a function called `p` that sends the rest over to a python interpreter for example, allowing you to integrate scripting languages.\n"
					<< "Or you could by default send all inputs to your scripting language, with only a few lines of code to edit in ConsoleSink.h! Go ham! :)\n";

				return 0;
			} )
		);

	};

	void ConsoleSink::draw() {
		ImGui::Begin(mWindowName.c_str());

		const float height =
			ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text

		ImGui::BeginChild("ScrollingRegion", { 0, -height }, false, ImGuiWindowFlags_HorizontalScrollbar);
			for (int i = 0; i < lines.size(); i++) {
				if (lines[i].isRaw) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(LOG_CONSOLE_RAWCOLOR)); ImGui::TextUnformatted(lines[i].message.c_str()); ImGui::PopStyleColor(); continue; }
				if (lines[i].isCommand) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(LOG_CONSOLE_COMMANDCOLOR)); ImGui::TextUnformatted(lines[i].message.c_str()); ImGui::PopStyleColor(); continue; }
				if (this->level() <= lines[i].lvl) { ImGui::TextUnformatted(lines[i].message.c_str()); }
			}

			if (scrollToBottom || (autoScroll && (ImGui::GetScrollY() <= ImGui::GetScrollMaxY())) ) {
				ImGui::SetScrollHereY(1.0f);
			}
			scrollToBottom = false;
		ImGui::EndChild();


		ImGui::Separator();


		bool focusOnInput = false;
		if (ImGui::InputText("Input", inputBuf, sizeof(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue
							 | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory,
							 [](ImGuiInputTextCallbackData* data) {
								 return ( (ConsoleSink*)(data->UserData) )->textEditCallback(data);
							 },
							 (void*)this)) {
			char* s = inputBuf;
			std::stringstream ss; ss << " >> " << s;
			*this << LogItem(ss.str(), spdlog::level::trace, false, true);
			if (s[0]) {
				char* s = inputBuf;
				readReturnCode(callCommand(s));
#ifdef AU_PLATFORM_WINDOWS
				strcpy_s<sizeof(inputBuf)>(inputBuf, "");
#else
				strcpy(inputBuf, "");
#endif
			}
			focusOnInput = true;
		}

		ImGui::SetItemDefaultFocus();
		if (focusOnInput) {
			ImGui::SetKeyboardFocusHere(-1);
		}

		ImGui::SameLine();

		if (this->level() == spdlog::level::trace) {
			if (ImGui::Button("Trace")) { this->set_level(spdlog::level::trace); }
		}
		else {
			if (ImGui::SmallButton("Trace")) { this->set_level(spdlog::level::trace); }
		}
		ImGui::SameLine();

		if (this->level() == spdlog::level::info) {
			if (ImGui::Button("Info")) { this->set_level(spdlog::level::info); }
		}
		else {
			if (ImGui::SmallButton("Info")) { this->set_level(spdlog::level::info); }
		}
		ImGui::SameLine();

		if (this->level() == spdlog::level::warn) {
			if (ImGui::Button("Warn")) { this->set_level(spdlog::level::warn); }
		}

		else {
			if (ImGui::SmallButton("Warn")) { this->set_level(spdlog::level::warn); }
		}
		ImGui::SameLine();

		if (this->level() == spdlog::level::err) {
			if (ImGui::Button("Error")) { this->set_level(spdlog::level::err); }
		}
		else {
			if (ImGui::SmallButton("Error")) { this->set_level(spdlog::level::err); }
		}
		ImGui::SameLine();

		if (autoScroll) {
			if (ImGui::SmallButton("Autoscroll?")) { this->autoScroll = false;  }
		}
		else {
			if (ImGui::Button("Autoscroll?")) { this->autoScroll = true;  }
		}
		ImGui::SameLine();

		if (ImGui::SmallButton("Scroll to bottom")) { this->scrollToBottom = true;  }

		ImGui::End();
	}

	int ConsoleSink::callCommand(const char* line) {
		history.push_back(std::string(line));

		std::istringstream iss(line);
		std::string buf;
		std::vector<std::string> argv;

		if (getline(iss, buf, ' ')) { // get the first word of the command (the command name)
			argv.push_back(buf); // put the command name in argv[0]

			for (const auto& item : this->commands) {
				if (item.command == argv[0]) { // find the matching command
					while (getline(iss, buf, ' ')) { // read in the rest of the words
						argv.push_back(buf);
					}
					try {
						return (int)item.fn(argv, this);
					}
					catch (std::exception& e) {
						*this << "(ConsoleSink Error #-1) Command threw an exception=\"" << e.what() << "\".\n";
						return -1;
					}
				}
			}

			*this << "(ConsoleSink Error #-2) No such command: \"" << argv[0] << "\"\n";
			return -1;
		}
		*this << "(ConsoleSink Error #-3) I couldn't find the first word (space-delimited block of characters) of your command.\n";
		return -1;
	}

	void ConsoleSink::readReturnCode(int code) {
		switch (code) {
		case -1:
			// Errors already handled by callCommand.
			break;
		case 0:
			// Success!
			break;
		case 1:
			*this << "Not enough parameters provided. (Error #1)\n";
			break;
		default:
			AU_LOG_WARN("Failure in calling command."
				"The function associated with the command returned error #{0}. "
				"Please check the associated function (e.g. in ConsoleSink.h or wherever it's defined) for what this error represents.", code);
			break;
		}
	}
	
	int ConsoleSink::textEditCallback(ImGuiInputTextCallbackData* data) {
		switch (data->EventFlag) {
			case ImGuiInputTextFlags_CallbackCompletion:
			{
				// Command completion.
				const char* endOfWord = data->Buf + data->CursorPos;
				const char* startOfWord = endOfWord;
				while (startOfWord > data->Buf) {
					const char c = startOfWord[-1];
					if (c == ' ' || c == '\t' || c == ',' || c == ';') {
						break;
					}
					startOfWord--;
				}

				std::string buffer = std::string(data->Buf);
				std::string word = std::string(buffer.substr(startOfWord - data->Buf, endOfWord - startOfWord));
				std::vector<std::string> completionCandidates;
				for (int i = 0; i < this->commands.size(); i++) {
					if (commands[i].command.substr(0, endOfWord - startOfWord) == word) {
						completionCandidates.push_back(commands[i].command);
					}
				}

				if (completionCandidates.size() == 0) {
					*this << "No matches for: \"" << word << "\".\n";
				}
				else if (completionCandidates.size() == 1) {
					data->DeleteChars((int)(startOfWord - data->Buf), (int)(endOfWord - startOfWord));
					data->InsertChars(data->CursorPos, completionCandidates[0].c_str());
					data->InsertChars(data->CursorPos, " ");
				}
				else {
					int matchLen = (int)(endOfWord - startOfWord);
					while (true) {
						char c = 0;
						bool allMatchesSoFar = true;
						for (int i = 0; i < completionCandidates.size() && allMatchesSoFar; i++) {
							if (i == 0) {
								c = completionCandidates[i][matchLen];
							}
							else if (c == 0 || c != completionCandidates[i][matchLen]) {
								allMatchesSoFar = false;
							}
						}
						if (!allMatchesSoFar) {
							break;
						}
						matchLen++;
					}

					if (matchLen > 0) {
						data->DeleteChars((int)(startOfWord - data->Buf), (int)(endOfWord - startOfWord));
						data->InsertChars(data->CursorPos, completionCandidates[0].c_str(), completionCandidates[0].c_str() + matchLen);
					}

					for (const auto candidate : completionCandidates) {
						*this << candidate << "\n";
					}
				}
				break;
			}
			case ImGuiInputTextFlags_CallbackHistory:
			{
				if (data->EventKey == ImGuiKey_UpArrow) {
					historyIndex = historyIndex == -1 ? history.size() - 1 : historyIndex - 1;
				}
				else if (data->EventKey == ImGuiKey_DownArrow && historyIndex != -1) {
					historyIndex = ++historyIndex >= history.size() ? -1 : historyIndex;
				}
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, (historyIndex >= 0) ? history[historyIndex].c_str() : "");
			}
		}
		return 0;
	}

	void ConsoleSink::sink_it_(const spdlog::details::log_msg& msg) {
		spdlog::memory_buf_t formatted;
		base_sink<std::mutex>::formatter_->format(msg, formatted);
		auto a = LogItem(fmt::to_string(formatted), msg.level);
		*this << a;
	};

};
