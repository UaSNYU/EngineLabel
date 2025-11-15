#include "clpch.h"
#include "SubTexture2D.h"

namespace Ciallo
{
	SubTexture2D::SubTexture2D(Ref<Texture2D>& texture, glm::vec2 min, glm::vec2 max)
	{
		m_Texture = texture;

		m_TextureCoords[0] = { min.x, min.y };
		m_TextureCoords[1] = { max.x, min.y };
		m_TextureCoords[2] = { max.x, max.y };
		m_TextureCoords[3] = { min.x, max.y };


	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(Ref<Texture2D>& texture, glm::vec2 coords, glm::vec2 Meshsize, glm::vec2 SpriteSize)
	{
		glm::vec2 min={ coords.x* Meshsize.x / texture->GetWidth(), coords.y* Meshsize.y / texture->GetHeight() };
		glm::vec2 max = { (coords.x + SpriteSize.x)* Meshsize.x / texture->GetWidth(), (coords.y + SpriteSize.y)* Meshsize.y / texture->GetHeight() };
	
		return CreatRef<SubTexture2D>(texture,min,max);
	}
}