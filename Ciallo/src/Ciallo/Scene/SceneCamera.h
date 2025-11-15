#pragma once

#include "Camera.h"

namespace Ciallo
{
	class SceneCamera :public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera();

		void SetCamera(float size,float nearClip,float farClip);

		void RecalculateProjection();

		void SetViewPort(uint32_t width, uint32_t height);

	private:
		float m_CameraSize=10.0f;
		float m_CameraNear=-1.0f, m_CameraFar=1.0f;
		float AspectRadio = 16.0f / 9.0f;
	};
}