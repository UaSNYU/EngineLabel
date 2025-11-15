#pragma once

#include "OrthographicCamera.h"
#include "Ciallo/Core/TimeStep.h"

#include "Ciallo/Events/MouseEvent.h"
#include "Ciallo/Events/ApplicationEvent.h"

namespace Ciallo
{
	struct OrthographicCameraBounds
	{
		float left, right;
		float bottom, top;

		float GetWidth() { return right - left; }
		float GetHeight() { return top - bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio,bool roation);

		void OnUpdata(TimeStep timestep);
		void OnEvent(Event& event);
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateBounds(); }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; CalculateBounds(); }
		void CalculateBounds();
		float GetZoomLevel() { return m_ZoomLevel; }
		float GetCameraRotation() { return m_CameraRotation; }

		OrthographicCamera& GetCamera() { return m_Camera; }
		OrthographicCameraBounds& GetBounds() { return m_bounds; }
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
		OrthographicCameraBounds m_bounds;

	};
}

