#pragma once

#include "Ciallo.h"
#include "ParticleSystem.h"

class Sandbox2D:public Ciallo::Layer
{
public:
	Sandbox2D();

	virtual void OnAttach()override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Ciallo::TimeStep timestep) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Ciallo::Event& event) override;

private:
	Ciallo::Ref<Ciallo::Shader> m_Shader;
	Ciallo::Ref<ParticleSystem> m_ParticleSystem;

	Ciallo::Ref<Ciallo::Texture2D> m_Texture;
	Ciallo::Ref<Ciallo::Texture2D> m_GTexture;
	Ciallo::Ref<Ciallo::SubTexture2D> m_BlockTexture;


	float m_fps;

	Ciallo::OrthographicCameraController m_CarmeraController;
};