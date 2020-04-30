#pragma once

#include "Renderer/Texture.h"

namespace Aulys 
{
	class FrameBuffer 
	{
	public:
		static Ref<FrameBuffer> create();
		virtual void attachTexture(Ref<Texture2D> texture) = 0;
		virtual void bind() noexcept = 0;
		virtual void unbind() noexcept = 0;

		// Renderer API ID get
		virtual explicit operator int() const noexcept = 0;
		virtual int getID() const noexcept = 0;

		// Validity checks
		virtual explicit operator bool() const noexcept = 0;
		virtual bool isValid() const noexcept = 0;

	private:
	
	}; // class FrameBuffer
}; // namespace Aulys
