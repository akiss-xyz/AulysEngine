#pragma once

#include "pch.h"

#include "glad/glad.h"

#include "Renderer/Shader.h"

namespace Aulys 
{
	class OpenGLShader : public Shader 
	{
	public:
		std::string testFn(const std::string& source, const std::string& params, size_t& EOLPos, size_t tokenPos);

		OpenGLShader(Path vertexSourcePath, Path fragmentSourcePath);
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		OpenGLShader(Path sourcePath);
		OpenGLShader(const std::string& source);

		virtual ~OpenGLShader() override;

		virtual void bind() const override;
		virtual void unbind() const override;
	
		virtual std::unordered_map<std::string, Tag> getTags() const override { return tags; };
		virtual Tag getTag(const std::string& tag) const override { return tags.at(tag); };
		virtual bool setTag(const std::string& tag, const std::string& val) override {
			if (tags.count(tag)) {
				tags.at(tag).val = val;
				tags.at(tag).isDefault = false;

				if (vertexpath != nullptr) {
					if (singlePath) {
						this->createFromPath(*vertexpath);
					}
					else {
						this->createFromPaths(*vertexpath, *fragpath);
					}

					return true;
				}
				return false;
			}
			return false;
		}; 
		virtual bool setTagToDefault(const std::string& tag) override {
			if (tags.count(tag)) {
				tags.at(tag).isDefault = true;

				if (vertexpath != nullptr) {
					if (singlePath) {
						this->createFromPath(*vertexpath);
					}
					else {
						this->createFromPaths(*vertexpath, *fragpath);
					}
					return true;
				}
				return false;
			}
			return false;
		}

		// The parent class Shader also uses this function to implement an overload like this:
		// void uploadUniformFloat4(const std::string& name, const float floats[4]), so that you
		// don't have to keep turning arrays into glm::vecs.
		virtual void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) override;
		virtual void uploadUniformMat4(const std::string& name, uint32_t count,
				const glm::mat4* matrices) override;
		virtual void uploadUniformMat3(const std::string& name, const glm::mat3& matrix) override;
		virtual void uploadUniformFloat4(const std::string& name, const glm::vec4& floats) override;
		virtual void uploadUniformFloat4(const std::string& name, uint32_t count, 
				const glm::vec4* floats) override;
		virtual void uploadUniformFloat3(const std::string& name, const glm::vec3& floats) override;
		virtual void uploadUniformFloat3(const std::string& name, uint32_t count,
			   	const glm::vec3* floats) override;
		virtual void uploadUniformFloat2(const std::string& name, const glm::vec2& floats) override;
		virtual void uploadUniformFloat(const std::string& name, const float value) override;
		virtual void uploadUniformInt(const std::string& name, const int value) override;
		virtual void uploadUniformInt2(const std::string& name, const int value[2]) override;
		virtual void uploadUniformBool(const std::string& name, const bool value) override;
		virtual void uploadUniform2Bool(const std::string& name, const bool value[2]) override;
	private:
		void createFromPath(Path sourcePath);
		void createFromPaths(Path vertexSourcePath, Path fragmentSourcePath);
		bool singlePath = true; // Was I constructed from one path or two?
		Ref<Path> vertexpath = nullptr; // The vertexpath member takes priority, so if the
		Ref<Path> fragpath = nullptr;   // shader was created from one path, it's stored in 
		                                       // vertexpath.
		std::unordered_map<GLenum, std::string> preprocess(
				const std::string& maybeCombinedSource, 
				const std::string& debug_path = {"path-not-specified"}) noexcept;
		void compile(const std::unordered_map<GLenum, std::string>& preprocessedSources,
				const std::string& debug_path = {"path-not-specified"}) noexcept;
		std::string readFile(const std::string& filePath) const noexcept;
		std::unordered_map<std::string, Tag> tags{};

		uint32_t mRendererID;
	}; // class OpenGLShader : public Shader
}; // namespace Aulys
