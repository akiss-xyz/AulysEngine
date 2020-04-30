#pragma once

#include "Renderer/Buffer.h"

namespace Aulys 
{
	class VertexArray 
	{
	public:
		virtual ~VertexArray() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
		virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

		static Ref<VertexArray> create();
	private:
	
	}; // class VertexArray
}; // namespace Aulys
