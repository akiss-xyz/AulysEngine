#include "src/pch.h"

#include "OpenGLFrameBuffer.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"

namespace Aulys 
{
	OpenGLFrameBuffer::OpenGLFrameBuffer() {
		glGenFramebuffers(1, &mRendererID);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &mRendererID);
	}

	void OpenGLFrameBuffer::bind() noexcept {
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
	}

	void OpenGLFrameBuffer::unbind() noexcept {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFrameBuffer::operator bool() const noexcept {
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	void OpenGLFrameBuffer::attachTexture(Ref<Texture2D> texture) {
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getID(), 0);

		// You could add depth and stencil stuff here.
	}
}; // namespace Aulys
