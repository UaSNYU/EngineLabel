#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Core/TimeStep.h"

#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

namespace Ciallo
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio,bool roation);

		void OnUpdata(TimeStep timestep);
		void OnEvent(Event& event);
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() { return m_ZoomLevel; }

		OrthographicCamera& GetCamera() { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

		bool rotation;
		float m_AspectRatio;
		float m_ZoomLevel=1.0f;
		float m_CameraRotation = 0.0f;
		glm::vec3 m_cameraPosition = { 0.0f,0.0f,0.0f };
		float m_cameraSpeed = 0.5f;
		float m_cameraRotationSpeed = 100.0f;

		OrthographicCamera m_Camera;
	};
}

