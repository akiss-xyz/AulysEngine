#pragma once

#include "pch.h"

// This file holds a lot of the 'Buffer'-like things we deal with, so if you're searching for
// VertexBuffer or IndexBuffer and stuff, you're in the right place.

namespace Aulys
{
	enum class ShaderDataType
	{
		None = 0,
		Bool,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
	}; // class ShaderDataType

	inline std::ostream& operator<<(std::ostream& os, const ShaderDataType& type) {
		switch(type) {
			case ShaderDataType::None:
				return os << "ShaderDataType::None";
			case ShaderDataType::Bool:
				return os << "ShaderDataType::Bool";
			case ShaderDataType::Float:
				return os << "ShaderDataType::Float";
			case ShaderDataType::Float2:
				return os << "ShaderDataType::Float2";
			case ShaderDataType::Float3:
				return os << "ShaderDataType::Float3";
			case ShaderDataType::Float4:
				return os << "ShaderDataType::Float4";
			case ShaderDataType::Mat3:
				return os << "ShaderDataType::Mat3";
			case ShaderDataType::Mat4:
				return os << "ShaderDataType::Mat4";
			case ShaderDataType::Int:
				return os << "ShaderDataType::Int";
			case ShaderDataType::Int2:
				return os << "ShaderDataType::Int2";
			case ShaderDataType::Int3:
				return os << "ShaderDataType::Int3";
			case ShaderDataType::Int4:
				return os << "ShaderDataType::Int4";
			default:
				AU_CORE_ASSERT(false, "Error: Unknown value of ShaderDataType ({0})", type);
				return os << "ShaderDataType::UNKNOWN";
		}
	}

	static uint32_t shaderDataTypeSize(ShaderDataType type) {
		switch(type) {
			case ShaderDataType::None:
				return 0;
			case ShaderDataType::Bool:
				return 1;
			case ShaderDataType::Float:
				return 4;
			case ShaderDataType::Float2:
				return 8;
			case ShaderDataType::Float3:
				return 12;
			case ShaderDataType::Float4:
				return 16;
			case ShaderDataType::Mat3:
				return 36;
			case ShaderDataType::Mat4:
				return 64;
			case ShaderDataType::Int:
				return 4;
			case ShaderDataType::Int2:
				return 8;
			case ShaderDataType::Int3:
				return 12;
			case ShaderDataType::Int4:
				return 16;
		}
		AU_CORE_ASSERT(false, "Error: Unknown ShaderDataType! (Value {0})", (int)type);
		return 0;
	}

	struct BufferElement 
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalize=false)
			: name(name), type(type), size(shaderDataTypeSize(type)),
			  offset(0), normalized(normalize) {};

		BufferElement() {};

		uint32_t getComponentCount() const {
			switch (type) {
				case ShaderDataType::None:   return 0;
				case ShaderDataType::Bool:   return 1;
				case ShaderDataType::Float:  return 1;
				case ShaderDataType::Float2: return 2;
				case ShaderDataType::Float3: return 3;
				case ShaderDataType::Float4: return 4;
				case ShaderDataType::Mat3:   return 9;
				case ShaderDataType::Mat4:   return 16;
				case ShaderDataType::Int:    return 1;
				case ShaderDataType::Int2:   return 2;
				case ShaderDataType::Int3:   return 3;
				case ShaderDataType::Int4:   return 4;
			}

			AU_CORE_ASSERT(false, "Error: Unknown ShaderDataType (Value: {0})!", (int)type);
			return 0;
		}
	}; // struct BufferElement
	
	class BufferLayout 
	{
	public:
		BufferLayout() {};

		BufferLayout(const std::initializer_list<BufferElement>& elements) : mElements(elements) {
			calculateOffsetAndStride();
		};

		inline const std::vector<BufferElement>& getElements() const { return mElements; }
		inline const uint32_t getStride() const { return mStride; }

		std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
		std::vector<BufferElement>::iterator end() { return mElements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

	private:
		void calculateOffsetAndStride() {
			uint32_t offset = 0;
			for (auto& e : mElements) {
				e.offset = offset;
				offset += e.size;
				mStride += e.size;
			}
		};

		std::vector<BufferElement> mElements;
		uint32_t mStride = 0;
	}; // class BufferLayout

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void bind() const = 0; 
		virtual void unbind() const = 0;

		virtual void setLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& getLayout() const = 0;

		/* Creating a Vertex Buffer binds it, and leaves it bound until something else gets bound.*/
		static Ref<VertexBuffer> create(float* vertices, uint32_t size);
	private:
	
	}; // class VertexBuffer

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		virtual void bind() const = 0; 
		virtual void unbind() const = 0;

		virtual uint32_t getCount() const = 0;

		/* Creating a Index Buffer binds it, and leaves it bound until something else gets bound.*/
		static Ref<IndexBuffer> create(uint32_t* indices, uint32_t size);
	private:
	
	}; // class IndexBuffer
}; // namespace Aulys
