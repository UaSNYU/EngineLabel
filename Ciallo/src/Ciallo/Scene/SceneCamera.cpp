#include "clpch.h"
#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Ciallo
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	SceneCamera::~SceneCamera()
	{
	}

	void SceneCamera::SetOrthographicCamera(float size, float nearClip, float farClip)
	{
		m_CameraType = CameraType::Orthographic;
		m_CameraOrthographicSize = size;
		m_CameraOrthographicNear = nearClip;
		m_CameraOrthographicFar = farClip;
	}

	void SceneCamera::SetPerspectiveCamera(float FOV, float nearClip, float farClip)
	{
		m_CameraType = CameraType::Perspective;
		m_CameraPerspectiveFov = FOV;
        m_CameraPerspectiveNear = nearClip;
        m_CameraPerspectiveFar = farClip;

	}

	void SceneCamera::SetViewPort(uint32_t width, uint32_t height)
	{
		AspectRadio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_CameraType == CameraType::Orthographic)
		{
			float orthoLeft = -m_CameraOrthographicSize * AspectRadio * 0.5;
			float orthoRight = m_CameraOrthographicSize * AspectRadio * 0.5;
			float orthoBottom = -m_CameraOrthographicSize * 0.5;
			float orthoTop = m_CameraOrthographicSize * 0.5;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_CameraOrthographicNear, m_CameraOrthographicFar);

		}
		else
		{
			m_Projection = glm::perspective(m_CameraPerspectiveFov, AspectRadio, m_CameraPerspectiveNear, m_CameraPerspectiveFar);
		}
	}
}
