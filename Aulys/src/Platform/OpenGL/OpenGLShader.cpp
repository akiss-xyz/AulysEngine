#include "src/pch.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>

#include "Core/Core.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Aulys 
{
	void OpenGLShader::createFromPath(Path sourcePath) {
		this->vertexpath = std::make_shared<Path>(sourcePath);
		std::string source = readFile(*sourcePath.gets());
		// This could be a combined shader file (consisting of vertex, fragment/pixel, geometry,
		// etc. shaders, as denoted by `#type vertex` for example.)
		auto shaderSources = preprocess(source, *sourcePath.gets());
		compile(shaderSources, *sourcePath.gets());
	}

	void OpenGLShader::createFromPaths(Path vertexSourcePath, Path fragmentSourcePath) {
		this->vertexpath = std::make_shared<Path>(vertexSourcePath);
		this->fragpath = std::make_shared<Path>(fragmentSourcePath);
		this->singlePath = false;
		std::string source = readFile(*vertexSourcePath.gets());
		auto vertexShaderSources = preprocess(source, *vertexSourcePath.gets());
		source = readFile(*fragmentSourcePath.gets());

		auto fragmentShaderSources = preprocess(source, *fragmentSourcePath.gets());
		vertexShaderSources.insert(fragmentShaderSources.begin(), fragmentShaderSources.end()); // Combine them.
		compile(vertexShaderSources, "Vertex source: \"" + *vertexSourcePath.gets() + "\" & Fragment source: \"" + *fragmentSourcePath.gets());
	}

	OpenGLShader::OpenGLShader(Path sourcePath) {
		createFromPath(sourcePath);
	}

	OpenGLShader::OpenGLShader(const std::string& source) {
		// This could be a combined shader file (consisting of vertex, fragment/pixel, geometry,
		// etc. shaders, as denoted by `#type vertex` for example.)
		auto shaderSources = preprocess(source, "passed-as-string");
		compile(shaderSources, "passed-as-string");
	}

	OpenGLShader::OpenGLShader(Path vertexSourcePath, Path fragmentSourcePath) {
		createFromPaths(vertexSourcePath, fragmentSourcePath);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		std::unordered_map<GLenum, std::string> sources;
		std::unordered_map<GLenum, std::string> buf = preprocess(vertexSource, "passed-as-strings");
		sources.insert(buf.begin(), buf.end());
		buf = preprocess(fragmentSource, "passed-as-strings");
		sources.insert(buf.begin(), buf.end());
		compile(sources, "passed-as-strings");
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(mRendererID);
	}

	std::string OpenGLShader::readFile(const std::string& filePath) const noexcept {
		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg()); // Make the result string the size of the file.
			in.seekg(0 ,std::ios::beg);
			in.read(&result[0], result.size()); // Read in the whole file into our string.
				// The amount we're reading in is result.size() because we resized it to the length
				// of the file.
			in.close();
		}
		else {
			AU_LOG_ERROR("[OpenGLShader::readFile]: Error opening file at path"
				" \"{0}\"! Returning empty string.", filePath);
		}
		return result;
	}

	GLenum shaderTypeFromString(const std::string& typeName) {
		if (typeName == "vertex") {
			return GL_VERTEX_SHADER;
		}
		else if (typeName == "fragment" || typeName == "pixel") {
			return GL_FRAGMENT_SHADER;
		}

		AU_LOG_WARN("[OpenGLShader::preprocess => shaderTysourcepeFromString]: Unrecognised typeName"
				" \"{0}\". Make sure your type name (following the token `#type `) is correct,"
				" and that all shader types you want to recognise are implemented in"
				" shaderTypeFromString. Returning 0.", typeName);
		return 0;
	}

	std::string readParametersAndCall(const char* token, const std::string& source,
		std::function<std::string(const std::string& source, const std::string& params, size_t& EOLPos, size_t tokenPos)> f,
		const std::string& debug_path) {
		std::string buffer = source;
		size_t lengthOfToken = strlen(token);
		size_t pos = buffer.find(token, 0);
		while (pos != std::string::npos) {
			size_t EOLPos = buffer.find_first_of(PLATFORM_EOL_CHAR, pos);
			AU_CORE_ASSERT(EOLPos != std::string::npos, "[Aulys shader preprocessing]: Syntax error"
				" in file \"{0}\":"
				"No EOL (currently '" PLATFORM_EOL_CHAR_PRINT_FRIENDLY "') following token '{1}' was found.", debug_path, token);
			size_t startOfParams = pos + strlen(token) + 1;
			std::string params = buffer.substr(startOfParams, EOLPos-startOfParams);

			buffer = f(buffer, params, EOLPos, pos);

			size_t nextLinePos = buffer.find_first_not_of(PLATFORM_EOL_CHAR, EOLPos);
			pos = buffer.find(token, nextLinePos); // Find next token.
		}
		return buffer;
	}

	std::string OpenGLShader::testFn(const std::string& source, const std::string& params, size_t& EOLPos, size_t tokenPos) {
		auto space = params.find_first_of(' ');
		auto tag = params.substr(0, space);
		std::string value{};
		if (tags.count(tag)) {
			if (tags[tag].isDefault) {
				value = tags[tag].defaultValue;
			}
			else {
				value = tags[tag].val;
			}
		}
		else {
			// Add to tags, and replace with #include
			auto defaultValue = params.substr(space+1, params.size() - space - 1);
			Tag t{defaultValue, defaultValue, true };
			auto tagName = params.substr(0, space);
			tags.insert({tagName, t});
			value = t.val;
		}

		const auto rv = source.substr(0, tokenPos) + "#include " + value + source.substr(EOLPos, source.size() - EOLPos);
		EOLPos = tokenPos;
		return rv;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preprocess(
			const std::string& source, const std::string& debug_path) noexcept {

		auto buffer = readParametersAndCall("#tag", source, [this](const std::string& source, const std::string& params, size_t& EOLPos, size_t tokenPos) {
			return this->testFn(source, params, EOLPos, tokenPos);
		}, debug_path);

		//auto buffer = readParametersAndCall("#tag", source,
		//	[this](const std::string& source, const std::string& params, size_t EOLPos, size_t tokenPos) {
		//		auto space = params.find_first_of(' ');
		//		auto tag = params.substr(0, space);
		//		std::string value{};
		//		if (this->tags->count(tag)) {
		//			// Replace with #include that represents that.
		//			value = *tags[tag].val;
		//		}
		//		else {
		//			// Add to tags, and replace with #include
		//			auto defaultValue = params.substr(space, params.size() - space);
		//			Tag t{defaultValue, defaultValue, true };
		//			auto tagName = params.substr(0, space);
		//			this->tags.insert({tagName, t});
		//			value = t.val;
		//		}

		//		return source.substr(0, tokenPos) + "#include " + value + source.substr(EOLPos, source.size() - EOLPos);

		//		//const std::string file = readFile(*Path(params.substr(1, params.size() - 2)).gets());
		//		//return source.substr(0, tokenPos) + file + source.substr(EOLPos, source.size() - EOLPos);
		//	},
		//	debug_path
		//);

		buffer = readParametersAndCall("#include", buffer,
			[this](const std::string& source, const std::string& params, size_t& EOLPos, size_t tokenPos) {
				const std::string file = readFile(*Path(params.substr(1, params.size() - 2)).gets());
				const auto rv = source.substr(0, tokenPos) + file + source.substr(EOLPos, source.size() - EOLPos);
				EOLPos = tokenPos;
				return rv;
			},
			debug_path
		);

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type"; // The token to search for to specify shader type.
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = buffer.find(typeToken, 0);

		while (pos != std::string::npos) { // While we're still finding token `#type`
			size_t eolPos = buffer.find_first_of(PLATFORM_EOL_CHAR, pos);
			AU_CORE_ASSERT(eolPos != std::string::npos, "[OpenGLShader::preprocess] Syntax error"
				" in file \"{0}\":" 
				" No EOL ('" PLATFORM_EOL_CHAR_PRINT_FRIENDLY "') found.", debug_path);

			size_t beginOfTypeName = pos + typeTokenLen + 1;

			std::string type = buffer.substr(beginOfTypeName, eolPos-beginOfTypeName);
				// Extract the typename e.g. "vertex", from the beginning to the eol.

			// If we recognise the shader type,
			if (auto typeEnum = shaderTypeFromString(type); (bool)typeEnum) {
				// TOUCH: TEST: Changed to PLATFORM-EOL-CHAR from "\r\n"
				size_t nextLinePos = buffer.find_first_not_of(PLATFORM_EOL_CHAR, eolPos);
				pos = buffer.find(typeToken, nextLinePos); // Find next type token.

				// I tried to make this look nice, but uhh... Well, you can see I tried.
				shaderSources[typeEnum] = 
					buffer.substr(nextLinePos, 
						pos - (nextLinePos == std::string::npos ? 
							buffer.size() - 1 
							: 
							nextLinePos));
			}
			else {
				// The type substring wasn't something we recognise.
				AU_LOG_ERROR("[OpenGLShader::preprocess] Syntax error in file \"{0}\":" 
					" Shader type specifier \"{1}\" (expecting e.g. 'vertex', 'pixel')"
					" after token '#type' was invalid. Returning map as it is now.", 
					debug_path, type);
				return shaderSources;
			}
		}

		Log::mCoreLogset->header->trace("Shader (\"" + debug_path + "\") was preprocessed to:\n");
		for (auto i : shaderSources) {
			std::istringstream iss(i.second); uint32_t lineNumber = 1;
			for (std::string line; std::getline(iss, line);) {
				Log::mCoreLogset->header->trace(std::to_string(lineNumber) + std::string(" | ") + line);
				lineNumber++;
			}
			Log::mCoreLogset->header->trace("\n-------------------------------------------------------------------");
		}

		return shaderSources;
	}

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& preprocessedSources,
			const std::string& debug_path) noexcept {

		GLuint program = glCreateProgram();
			// We create a program first, so that if it fails we didn't set mRendererID to a useless
			// value - that's set at the end once we know all is legit. But we need this here so we
			// can glAttachShader as we go through each shader we compile.

		// If we don't put the .size()-2 here, we'll end up with a stray zero at the end, which'll 
		// rightly annoy OpenGL when it comes time to delete shaders and stuff. See the end of this
		// fn for more detail, where we check if this has a zero.
		std::vector<GLenum> glShaderIDs;
			// Oh. We don't need a size. The comment above is a relic of history!
			// The reason we have zeroes is because of push_back(). Crap.
			// That was kinda funny, honestly.

		for (auto& [shaderType, sourceStr] : preprocessedSources) {
			//// Compile it.
			GLuint shader = glCreateShader(shaderType);

			const GLchar* source = sourceStr.c_str();
			glShaderSource(shader, 1, &source, nullptr);
			// nullptr just tells OpenGL to assume all strings are null-terminated.

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			
			if (isCompiled == GL_FALSE) {
				/// We failed to compile! Let's extract the log message.
				GLint maxLength = 0; // Length of the message, incl. terminating null character.
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength); // Put it into here ^

				std::vector<GLchar> infoLog(maxLength); // Create a vector of GLchars to hold it.
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
					// Put the info log, of this shader, at the address of our vector of chars.
				
				// Get rid of the shader!
				glDeleteShader(shader);

				// Output all we need!
				AU_LOG_ERROR("[OpenGLShader::compile]: Compiling shader at path \"{0}\" (type {1})"
					" failed! Here's the log message:", debug_path, shaderType);
				AU_LOG_ERROR("{0}", infoLog.data());
				break;
			}
			glAttachShader(program, shader);

			glShaderIDs.push_back(shader);
				// Store each shader ID so that we can later glDeleteProgram and stuff.
		}

		glLinkProgram(program);
		// Using the complete shader that mismashes the hypVR shaders together doesn't link, on 
		// this line.

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE) {
			/// We failed to link! Again, let's get a log message out.
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
				// Let's slap the info log into that vec.

			// Get rid of our unlinked program.
			glDeleteProgram(program);

			// Go through each shader and delete 'em - this is why we kept the IDs.
			for (auto shaderID : glShaderIDs) {
				glDeleteShader(shaderID);
			}

			// And let's log our stuff.
			AU_LOG_ERROR("[OpenGLShader::compile]: Linking shader at path \"{0}\" failed!"
				" Here's the log message:", debug_path);
			AU_LOG_ERROR("{0}", infoLog.data());
		}

		// Go through each shader and detach it!
		for (auto const& shaderID : glShaderIDs) {

			if (shaderID == 0) {
				AU_LOG_WARN(
					"[OpenGLShader::compile]: Warning from shader source at \"{0}\"."
						" This shouldn't really ever happen, but here we are. :>\n"
					"I've linked the shaders in that file into a program (id {1}), but "
					"now that I'm trying to delete the shaders, I found one with ID 0. This might:"
					"\n"
					" -- Indicate an error in creating the shader.\n"
					"    If so, OpenGL will tell you,"
					   " provided you have ENABLE_GL_DEBUG_OUTPUT defined in AulysConf.h."
					   " Read the message by OpenGL to fix this, a zero somehow snuck into the IDs."
					"\n"
					" -- Or, it could indicate that we're storing the shader IDs wrong.\n"
					"    Check that \"std::vector<GLenum> glShaderIDs\" isn't one too big, "
					    "or if it's somehow null-terminated.\n"
					"You'll likely be hit with a \"GL_INVALID_VALUE in glDetachShader\" because of "
					"this - because I'm still going to try and delete it, just thought I'd warn ya."
					, debug_path, program); 
			}
			glDetachShader(program, shaderID);
		}

		// Success!
		mRendererID = program;
		glUseProgram(mRendererID);
	}

	void OpenGLShader::bind() const
	{
		glUseProgram(mRendererID);
	}

	void OpenGLShader::unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	};
	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		// f means it's floats
		// v means it's an array
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	};

	void OpenGLShader::uploadUniformMat4(const std::string& name, uint32_t count,
			const glm::mat4* matrices) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(*matrices));
	};

	// [Material Systems]
	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& floats) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform4f(location, floats.x, floats.y, floats.z, floats.w);
	};

	void OpenGLShader::uploadUniformFloat4(const std::string& name, uint32_t count,
			const glm::vec4* floats) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform4fv(location, count, glm::value_ptr(*floats));
	};

	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& floats) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform3f(location, floats.x, floats.y, floats.z);
	}

	void OpenGLShader::uploadUniformFloat3(const std::string& name, uint32_t count,
			const glm::vec3* floats) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform3fv(location, count, glm::value_ptr(*floats));
	};


	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& floats) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform2f(location, floats.x, floats.y);
	}

	void OpenGLShader::uploadUniformFloat(const std::string& name, const float value) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, const int value) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::uploadUniformInt2(const std::string& name, const int value[2]) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform2i(location, value[0], value[1]);
	}

	void OpenGLShader::uploadUniformBool(const std::string& name, const bool value) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1i(location, (int)value);
	}

	void OpenGLShader::uploadUniform2Bool(const std::string& name, const bool value[2]) {
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1iv(location, 2, (const int*)value);
	}

}; // namespace Aulys
