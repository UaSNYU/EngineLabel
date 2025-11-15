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

	void SceneCamera::SetCamera(float size, float nearClip, float farClip)
	{
		m_CameraSize = size;
		m_CameraNear = nearClip;
		m_CameraFar = farClip;
	}


	void SceneCamera::SetViewPort(uint32_t width, uint32_t height)
	{
		AspectRadio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -m_CameraSize * AspectRadio * 0.5;
		float orthoRight = m_CameraSize * AspectRadio * 0.5;
		float orthoBottom = -m_CameraSize * 0.5;
		float orthoTop = m_CameraSize * 0.5;

		m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_CameraNear, m_CameraFar);
	}
}
