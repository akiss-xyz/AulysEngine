#include "src/pch.h"

#include "OpenGLTexture.h"

#include "glad/glad.h"
#include "stb/stb_image.h"

namespace Aulys 
{
	OpenGLTexture2D::OpenGLTexture2D(const Path& path) : mPath(path) {
		int width, height, channels;
		stbi_uc* data = stbi_load(path.get(), &width,  &height, &channels, 0);

		AU_CORE_ASSERT(data, 
				"[OpenGLTexture2D::OpenGLTexture2D] Failed to load image at path \"{0}\"!", path.get());

		mWidth = width; mHeight = height;

		GLenum internalStorageFormat = 0, dataFormat = 0; 
			// internalStorageFormat is how we upload the data, passed to glTextureStorage2D.
			// dataFormat is how the data is processed by OpenGL, passed to glTextureSubImage2D.
		if (channels == 4) {
			internalStorageFormat = GL_RGBA8;   // If we have 4 channels, assume 
			                                    // 8 bits per channel, RGBA format.
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1); /* See if this only applies to 3 channels
				Some pngs are packed differently so this setting is here to stop access
				violations because the width of every line isn't what OpenGL expects.
			    https://www.khronos.org/opengl/wiki/Common_Mistakes#Texture_upload_and_pixel_reads
				*/
			
			internalStorageFormat = GL_RGB8; // If we have 3 channels, assume it is RGB format.
			dataFormat = GL_RGB;
		}
		else if (channels == 1) {
			internalStorageFormat = GL_RGB8;
			dataFormat = GL_RED;
		}
		else {
			AU_CORE_ASSERT(false, 
			"[OpenGLTexture2D] The image at path \"{0}\" has unsupported number of channels ({1}).", 
				path.get(), channels);
			internalStorageFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}


		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);

		glTextureStorage2D(mRendererID, 1, internalStorageFormat, mWidth, mHeight);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// "FS SIMD16 shader: 14 inst, 0 loops, 322 cycles, 0:0 spills:fills,"
		// "Promoted 0 constants, compacted 224 to 176 bytes."
		// From source GL_DEBUG_SOURCE_SHADER_COMPILER, with type GL_DEBUG_TYPE_OTHER.
		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, dataFormat, 
				GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(int width, int height) : mPath("0"), mWidth(width), mHeight(height) {
		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glBindTexture(GL_TEXTURE_2D, mRendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	};

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &mRendererID);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTextureUnit(slot, mRendererID);
	}
}; // namespace Aulysthe

