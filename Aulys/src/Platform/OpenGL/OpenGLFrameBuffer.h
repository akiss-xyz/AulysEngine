#pragma once

#include "Renderer/FrameBuffer.h"
#include "Renderer/Texture.h"

namespace Aulys 
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer();
		~OpenGLFrameBuffer();
		virtual void bind() noexcept override;
		virtual void unbind() noexcept override;
		virtual void attachTexture(Ref<Texture2D> texture) override;

		// OpenGL ID get
		virtual explicit operator int() const noexcept override { return mRendererID; }
		virtual int getID() const noexcept override { return (int)*this; }

		// Validity checks - e.g. is the framebuffer complete?
		virtual explicit operator bool() const noexcept override;
		virtual bool isValid() const noexcept { return (bool)*this; }

	private:
		uint32_t mRendererID;
	}; // class FrameBuffer
}; // namespace Aulys
