#pragma once

#include "Ciallo/Renderer/Texture.h"
#include "Ciallo/Core/Core.h"
#include <filesystem>


namespace Ciallo
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentPath;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};

}