#pragma once

#include <string>

#include "glm/glm.hpp"

namespace Aulys
{
	class Shader
	{
	public:
		struct Tag {
			std::string val;
			const std::string defaultValue;
			bool isDefault = true;
		};

		virtual ~Shader() = default;

		static Ref<Shader> create(Path vertexPath, Path fragmentPath);
		static Ref<Shader> create(const std::string& vertexSource, const std::string& fragmentSource);
		static Ref<Shader> create(Path sourcePath);
		static Ref<Shader> create(const std::string& source);

		// This is the equivalent of calling glUseProgram(ID)
		// This installs a program object as a part of the current rendering state. Not sure why
		// this isn't required.
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual std::unordered_map<std::string, Tag> getTags() const = 0;
		virtual Tag getTag(const std::string& tag) const = 0;
		virtual bool setTag(const std::string& tag, const std::string& val) = 0;
		virtual bool setTagToDefault(const std::string& tag) = 0;

		/* [Shader Abstraction and Uniforms] THIS'LL ALL GO, but I'm keeping this around until
		 * the material system takes better shape. The reason we're not going to use this is that 
		 * once the material system is in place, you can just upload buffers. So for now, I'm 
		 * keeping them around, so I don't have to cast to OpenGLShader, but this'll be removed. */

		// Note: these functions do not guarantee that the shader is bound. You have to take care of
		// that for now.

		virtual void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;
		virtual void uploadUniformMat4(const std::string& name, uint32_t count,
				const glm::mat4* matrices) = 0;
		virtual void uploadUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
		// Used for uploading vectors to uniforms [Material Systems]
		virtual void uploadUniformFloat4(const std::string& name, const glm::vec4& floats) = 0;
		virtual void uploadUniformFloat4(const std::string& name, uint32_t count,
				const glm::vec4* floats) = 0;
		virtual void uploadUniformFloat4(const std::string& name, const float floats[4]) {
			this->uploadUniformFloat4(name, {floats[0], floats[1], floats[2], floats[3]});
		};
		virtual void uploadUniformFloat3(const std::string& name, const glm::vec3& floats) = 0;
		virtual void uploadUniformFloat3(const std::string& name, uint32_t count,
				const glm::vec3* floats) = 0;
		virtual void uploadUniformFloat3(const std::string& name, const float floats[3]) {
			this->uploadUniformFloat3(name, {floats[0], floats[1], floats[2]});
		}
		virtual void uploadUniformFloat2(const std::string& name, const glm::vec2& floats) = 0;
		virtual void uploadUniformFloat2(const std::string& name, const float floats[2]) {
			this->uploadUniformFloat2(name, {floats[0], floats[1]});
		}
		virtual void uploadUniformFloat(const std::string& name, const float value) = 0;
		virtual void uploadUniformInt(const std::string& name, const int value) = 0;
		virtual void uploadUniformInt2(const std::string& name, const int value[2]) = 0;
		
		virtual void uploadUniformBool(const std::string& name, const bool value) = 0;
		virtual void uploadUniform2Bool(const std::string& name, const bool value[2]) = 0;
	private:

	}; // class Shader
}; // namespace Aulys

