#include "clpch.h"
#include "OrthographicCameraController.h"

#include "Input.h"
#include "KeyCodes.h"

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
			m_cameraPosition.y += m_cameraSpeed * timestep;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
			m_cameraPosition.y -= m_cameraSpeed * timestep;
		}

		if (Input::IsKeyPressed(HZ_KEY_A))
		{
			m_cameraPosition.x -= m_cameraSpeed * timestep;
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{
			m_cameraPosition.x += m_cameraSpeed * timestep;
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

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_ZoomLevel =m_ZoomLevel> event.GetYOffset() * 0.05f ? m_ZoomLevel-event.GetYOffset()*0.05f:m_ZoomLevel;

		std::cout << m_ZoomLevel << std::endl;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}