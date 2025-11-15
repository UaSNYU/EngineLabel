#pragma once

#include "Texture.h"
#include "glm/glm.hpp"
#include "Ciallo/Core/Core.h"

namespace Ciallo
{
	class SubTexture2D
	{
	public:
		SubTexture2D(Ref<Texture2D>& texture,glm::vec2 min, glm::vec2 max);

		Ref<Texture2D>& GetTexture()  { return m_Texture; }
		glm::vec2* GetTexCoords() { return m_TextureCoords; }
		static Ref<SubTexture2D> CreateFromCoords(Ref<Texture2D>& texture,glm::vec2 coords,glm::vec2 Meshsize,glm::vec2 SpriteSize);

	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TextureCoords[4];
	};

}