#include "src/pch.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "glad/glad.h"

#include <memory>

namespace Aulys 
{
	static GLenum ShaderDataTypeToOpenGL(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::None:   return 0;
			case ShaderDataType::Bool:   return GL_BOOL;
			case ShaderDataType::Float:  return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Mat3:   return GL_FLOAT;
			case ShaderDataType::Mat4:   return GL_FLOAT;
			case ShaderDataType::Int:    return GL_INT;
			case ShaderDataType::Int2:   return GL_INT;
			case ShaderDataType::Int3:   return GL_INT;
			case ShaderDataType::Int4:   return GL_INT;
	}

		AU_CORE_ASSERT(false, "Error: [Aulys::ShaderDataTypeToOpenGL]: Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &mRendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &mRendererID);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(mRendererID);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		AU_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(),
			"OpenGLVertexArray::addVertexBuffer: No elements in vertexBuffer->getLayout.")

		glBindVertexArray(mRendererID);
		vertexBuffer->bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& e : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer( // This binds the bound VA to the bound VB.
				index,
				// ^ In a vertex, what index within that are you talking about? This specifies to
				// OpenGL that, when I'm on the shader side and I ask for vertex[1], that 1
				// corresponds to, I dunno, say, normals.
				e.getComponentCount(),
				// ^ This is the count, it just means how many of the data type (specified in the 
				// next arg) are we passing in. e.g how many floats in one of these things? So if
				// you created an Int3, this'll be 3.
				ShaderDataTypeToOpenGL(e.type),
				// ^ Here, we're telling OpenGL what data type this is. Our BufferElement class 
				// stores this information, we just convert it using our utility function here.
				// e.g. if you created an Int3, this'll be GL_INT.
				( e.normalized ? GL_TRUE : GL_FALSE ),
				// ^ Whether this is normalized or not. If you have an int as a data type, this
				// determines whether or not this int gets mapped to [-1, 1] or [0, 1]. The square
				// brackets here, as usual, mean that we include the end values.
				layout.getStride(),
				// ^ The byte offset between each vertex. How many bytes do I have to step before
				// I'm on to the next vertex? To be honest, this is just the size of each vertex. So
				// if I start at the beginning of the VertexBuffer, and I step this many bytes in,
				// then I should be at the start of the second vertex.
				reinterpret_cast<const void*>(e.offset)
				// ^ Look within a single vertex, ignore everything else. How many bytes 'into' a
				// vertex does this actually begin? How many bytes do I have to jump over before I
				// get to what we're talking about?
			);
			index++;
		}
		mVertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(mRendererID);
		indexBuffer->bind();
		mIndexBuffer = indexBuffer;
	}

}; // namespace Aulys
