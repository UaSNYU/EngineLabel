#pragma once

#include "Ciallo.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Ciallo/Scene/EditorCamera.h"
#include "Panels/ContentBrowserPanel.h"

namespace Ciallo
{
	class CialloEditor2D :public Layer
	{
	public:
		CialloEditor2D();

		virtual void OnAttach()override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		void UI_Toolbar();
		void OnOverlayRender();
		void OnSceneEdit();
		void OnScenePlay();
		void OnDuplicateEntity();

		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		void NewScene();
        void OpenScene();
		void OpenScene(const std::filesystem::path& path);
        void SaveSceneAs();
		void SaveScene();
	private:
		std::filesystem::path m_CurrentPath;

		Ref<FrameBuffer> m_FrameBuffer;
		
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_GTexture;
		Ref<Texture2D> m_iconplay;
		Ref<Texture2D> m_iconedit;
		Ref<SubTexture2D> m_BlockTexture;
		
		SceneHierarchyPanel m_Panel;
		ContentBrowserPanel m_ContentPanel;

		glm::vec2 m_ViewPortSize = { 1280.0f,720.0f };
		float m_fps;
		int m_GizmoType = -1;
		bool m_viewportfocus=true;
		bool m_viewporthover = true;
		bool m_colliderbox = false;
		glm::vec2 m_ViewPortBounds[2];

		EditorCamera m_EditorCamera;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditScene;
		Entity HoverdEntity;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		SceneState m_SceneState = SceneState::Play;
	};
}

