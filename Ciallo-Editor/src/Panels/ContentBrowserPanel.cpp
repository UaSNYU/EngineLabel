#include "ContentBrowserPanel.h"
#include "imgui.h"
#include <iostream>

namespace Ciallo
{
	static const std::filesystem::path s_AssertPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		:m_CurrentPath(s_AssertPath)
	{
		m_DirectoryIcon = Texture2D::Create("assets/Icons/directory.png");
		m_FileIcon = Texture2D::Create("assets/Icons/file.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentPath != std::filesystem::path(s_AssertPath))
		{
			if (ImGui::Button("<< BACK"))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
			}
		}

		static ImVec2 ButtonSize = {128.0f,128.0f };
		int ColumnCount = ImGui::GetContentRegionAvailWidth()/ButtonSize.x;
		if (ColumnCount < 1) ColumnCount = 1;


		ImGui::Columns(ColumnCount,0,false);
		for (auto& p : std::filesystem::directory_iterator(m_CurrentPath))
		{
			const auto& path = p.path();
			auto relativePath = std::filesystem::relative(p.path(),s_AssertPath);
			std::string relativePathString = relativePath.filename().string();


			ImGui::PushID(relativePathString.c_str());
			Ref<Texture> icon = p.is_directory() ? m_DirectoryIcon : m_FileIcon;
			if (p.is_directory())
			{

				if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { 128.0f,128.0f }, { 0,1 }, { 1,0 }))
				{
					m_CurrentPath /= p.path().filename();
					std::cout << m_CurrentPath;
				}
			}
			else
			{
				ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { 128.0f,128.0f }, { 0,1 }, { 1,0 });

				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* path = p.path().c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM",path,(wcslen(path)+1)*sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}
			}
			ImGui::TextWrapped(relativePathString.c_str());


			ImGui::PopID();
			ImGui::NextColumn();
		}


		ImGui::End();
	}

}