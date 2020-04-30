#pragma once

#include "pch.h"

#include "Renderer/VertexArray.h"

namespace Aulys 
{
	class OpenGLVertexArray : public VertexArray 
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
		virtual void addVertexBuffer
			(const Ref<VertexBuffer>& vertexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override 
		{
			return mVertexBuffers;
		}

		virtual const Ref<IndexBuffer>& getIndexBuffer() const override { 
			return mIndexBuffer;
		}
	private:
		uint32_t mRendererID;
		std::vector<Ref<VertexBuffer>> mVertexBuffers;
		Ref<IndexBuffer> mIndexBuffer;
	
	}; // class OpenGLVertexArray : public VertexArray
}; // namespace Aulys

