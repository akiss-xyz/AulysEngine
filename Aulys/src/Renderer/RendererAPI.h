#pragma once

#include "pch.h"

#include "glm/glm.hpp"

#include "Renderer/VertexArray.h"

namespace Aulys 
{
	class RendererAPI 
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
		virtual void init() = 0;

		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		static inline API getAPI() { return sAPI; };
	private:
		static API sAPI;
	}; // class RendererAPI
}; // namespace Aulys
