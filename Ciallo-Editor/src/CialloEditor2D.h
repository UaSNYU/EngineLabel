#pragma once

#include "Ciallo.h"

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
		Ref<FrameBuffer> m_FrameBuffer;
		
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_GTexture;
		Ref<SubTexture2D> m_BlockTexture;

		glm::vec2 m_ViewPortSize = { 1280.0f,720.0f };
		float m_fps;
		bool m_viewportfocus=true;
		bool m_viewporthover = true;

		OrthographicCameraController m_CarmeraController;

		Ref<Scene> m_Scene;
		Entity m_SquareEntity;
		Entity m_Camera;
		Entity m_SecondCamera;
	};
}

