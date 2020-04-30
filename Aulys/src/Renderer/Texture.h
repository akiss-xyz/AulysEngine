#pragma once

#include "Core/Core.h"

namespace Aulys 
{
	class Texture 
	{
	public:
		virtual ~Texture() = default;

		virtual explicit operator int() const = 0;
		virtual int getID() const { return (int)(*this); };

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void bind(uint32_t slot = 0) const = 0;
	private:
	
	}; // class Texture

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> create(const Path& path);
		static Ref<Texture2D> create(int width, int height);
		static Ref<Texture2D> create(glm::vec2 size) {
			return Texture2D::create(size.x, size.y);
		};
	private:
	
	}; // class Texture2D : public Texture

}; // namespace Aulys
