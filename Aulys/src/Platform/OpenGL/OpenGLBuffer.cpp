#include "src/pch.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#include "glad/glad.h"

namespace Aulys 
{
	/* Vertex Buffer Impl */

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size){
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		// Creating a VertexBuffer binds that VertexBuffer, and leaves it bound.
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &mRendererID);
		// We give this a pointer because glDeleteBuffers can delete a list.
	}

	void OpenGLVertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID); // This is an issue here.
	}

	void OpenGLVertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Bind nothing
	}

	/* Index Buffer Impl */
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : mCount(count) {
		AU_CORE_ASSERT(sizeof(uint32_t) == sizeof(GLuint), "For some reason, on your platform, the"
		    " size of the unsigned ints OpenGL is using is different to uint32_t. This'll"
			" definitely mess up the Index Buffers' sizes.");
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer(){
		glDeleteBuffers(1, &mRendererID);
	}

	void OpenGLIndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	}

	void OpenGLIndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}; // namespace Aulys
