#pragma once

#include "Camera.h"

namespace Ciallo
{
	enum class CameraType
	{
		Perspective=0,
		Orthographic=1
	};

	class SceneCamera :public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera();

		void SetOrthographicCamera(float size,float nearClip,float farClip);
		void SetPerspectiveCamera(float FOV, float nearClip, float farClip);

        glm::mat4 GetView() const;

		void SetViewPort(uint32_t width, uint32_t height);

		void RecalculateProjection();

		CameraType GetCameraType(){return m_CameraType;}
		void SetCameraType(CameraType type){m_CameraType = type;RecalculateProjection();}

		///////////////////Orthographic
		float& GetOrthographicCameraSize() { return m_CameraOrthographicSize; }
		void SetOrthographicCameraSize(float size) { m_CameraOrthographicSize = size; RecalculateProjection(); }

		float& GetOrthographicNearClip(){return m_CameraOrthographicNear;}
		void SetOrthographicNearClip(float nearClip){m_CameraOrthographicNear = nearClip;RecalculateProjection();}

		float& GetOrthographicFarClip(){return m_CameraOrthographicFar;}
		void SetOrthographicFarClip(float farClip){m_CameraOrthographicFar = farClip;RecalculateProjection();}

		///////////////////Perspective
		float& GetPerpectiveCameraFov() { return m_CameraPerspectiveFov; }
		void SetPerpectiveCameraFov(float size) { m_CameraPerspectiveFov = size; RecalculateProjection(); }

		float& GetPerpectiveNearClip() { return m_CameraPerspectiveNear; }
		void SetPerpectiveNearClip(float nearClip) { m_CameraPerspectiveNear = nearClip; RecalculateProjection(); }

		float& GetPerpectiveFarClip() { return m_CameraPerspectiveFar; }
		void SetPerpectiveFarClip(float farClip) { m_CameraPerspectiveFar = farClip; RecalculateProjection(); }

	private:
		CameraType m_CameraType= CameraType::Orthographic;

		float m_CameraPerspectiveFov=45.0f;
		float m_CameraPerspectiveNear=0.01f, m_CameraPerspectiveFar=1000.0f;

		float m_CameraOrthographicSize=10.0f;
		float m_CameraOrthographicNear=-1.0f, m_CameraOrthographicFar=1.0f;
		float AspectRadio = 16.0f / 9.0f;
	};
}