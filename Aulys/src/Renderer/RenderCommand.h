#pragma once

#include "Renderer/RendererAPI.h"

namespace Aulys 
{
	class RenderCommand 
	{
	public:
		inline static void init() {
			sRendererAPI->init();
		}

		inline static void setClearColor(const glm::vec4& color) {
			sRendererAPI->setClearColor(color);
		}
		inline static void clear() {
			sRendererAPI->clear();
		}
		inline static void drawIndexed(const Ref<VertexArray>& vertexArray) {
			sRendererAPI->drawIndexed(vertexArray);
		}
	private:
		static RendererAPI* sRendererAPI;
	}; // class RenderCommand
}; // namespace Aulys
