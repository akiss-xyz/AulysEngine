#pragma once

#include "Renderer/Texture.h"

namespace Aulys
{
	class OpenGLTexture2D : public Texture2D 
	{
	public:
		OpenGLTexture2D(const Path& path);
		OpenGLTexture2D(int width, int height);
		virtual ~OpenGLTexture2D();

		virtual explicit operator int() const override { return mRendererID; };
		virtual uint32_t getWidth() const override { return mWidth; };
		virtual uint32_t getHeight() const override { return mHeight; };

		virtual void bind(uint32_t slot = 0) const override;
	private:
		Path mPath;  // Might be useful for debugging, and hot reloading textures
		             // Though it might later move into a resource manager, is iffy here.
		uint32_t mWidth, mHeight;
		uint32_t mRendererID = 0;
	}; // class OpenGLTexture2D : public Texture2D

}; // namespace Aulys
