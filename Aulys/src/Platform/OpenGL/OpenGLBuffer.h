#pragma once

#include "Renderer/Buffer.h"

namespace Aulys 
{
	class OpenGLVertexBuffer : public VertexBuffer 
	{
	public:
		/* Creating a Vertex Buffer binds it, and leaves it bound until something else gets bound.*/
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setLayout(const BufferLayout& layout) override { mLayout = layout; }
		virtual const BufferLayout& getLayout() const override { return mLayout; }

	private:
		BufferLayout mLayout;
		uint32_t mRendererID;
	}; // class OpenGLVertexBuffer : public VertexBuffer

	class OpenGLIndexBuffer : public IndexBuffer 
	{
	public:
		/* Creating a Index Buffer binds it, and leaves it bound until something else gets bound.*/
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t getCount() const override {
			return mCount;
		}

	private:
		uint32_t mRendererID;
		uint32_t mCount;
	}; // class OpenGLIndexBuffer : public IndexBuffer
}; // namespace Aulys
