#include "clpch.h"
#include "OrthographicCameraController.h"

#include "Ciallo/Core/Input.h"
#include "Ciallo/Core/KeyCodes.h"

namespace Ciallo
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool roation)
		:m_Camera(-aspectRatio*m_ZoomLevel, aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),rotation(roation),m_AspectRatio(aspectRatio)
	{

	}

	void OrthographicCameraController::OnUpdata(TimeStep timestep)
	{
		if (Input::IsKeyPressed(HZ_KEY_W))
		{
			m_cameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_cameraSpeed * timestep;
			m_cameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_cameraSpeed * timestep;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
			m_cameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_cameraSpeed * timestep;
			m_cameraPosition.x -= -sin(glm::radians(m_CameraRotation))*m_cameraSpeed * timestep;
		}

		if (Input::IsKeyPressed(HZ_KEY_A))
		{
			m_cameraPosition.y += -sin(glm::radians(m_CameraRotation)) * m_cameraSpeed * timestep;
			m_cameraPosition.x += -cos(glm::radians(m_CameraRotation)) * m_cameraSpeed * timestep;
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{
			m_cameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_cameraSpeed * timestep;
			m_cameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_cameraSpeed * timestep;
		}

		if (rotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_Q))
			{
				m_CameraRotation+= m_cameraRotationSpeed * timestep;
			}
			else if (Input::IsKeyPressed(HZ_KEY_E))
			{
				m_CameraRotation -= m_cameraRotationSpeed * timestep;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_cameraPosition);
	}


	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::CalculateBounds()
	{
		m_bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_ZoomLevel =m_ZoomLevel> event.GetYOffset() * 0.05f ? m_ZoomLevel-event.GetYOffset()*0.05f:m_ZoomLevel;
		CalculateBounds();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		CalculateBounds();
		return false;
	}

}