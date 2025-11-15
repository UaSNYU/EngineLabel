#include "ParticleSystem.h"
#include "Random.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

static ParticleProps defaultPartiprops = {
	{ 0.0f,0.0f },
	{ 0.0f,0.0f },{0.5f,0.5f},
	{1.0f,1.0f,0.0f},{0.0f,0.0f,0.0f},
	0.2f,0.0f,0.1f,
	5.0f
};


ParticleSystem::ParticleSystem()
{
	m_ParticlePool.resize(1000);
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
	particle.BeginColor = particleProps.BeginColor;
	particle.EndColor = particleProps.EndColor;
	particle.BeginSize= particleProps.BeginSize+particleProps.SizeVariation*(Random::Float()-0.5f);
	particle.EndSize = particleProps.EndSize + particleProps.SizeVariation * (Random::Float() - 0.5f);

	particle.Rotation = Random::Float() * 360.0f;

	particle.LiftTime = particleProps.LiftTime;
	particle.LifeRemaining = particleProps.LiftTime;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();

	std::cout << "Emitting particle at index: " << m_PoolIndex << std::endl;
}

void ParticleSystem::OnUpdate(Ciallo::TimeStep ts)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active) continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}
		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;

		
	}
}

void ParticleSystem::OnRenderer()
{
	for (auto& particle : m_ParticlePool)
	{
		if (particle.Active==false) continue;

		float life = particle.LifeRemaining / particle.LiftTime;
		glm::vec3 color = glm::lerp(particle.EndColor, particle.BeginColor, life);
		glm::vec4 r_color= glm::vec4(color, life);
		float size = glm::lerp(particle.EndSize, particle.BeginSize, life);

		Ciallo::Renderer2D::DrawRotateQuad(particle.Position,particle.Rotation, glm::vec2(size), r_color);
	}
}

void ParticleSystem::ClearParticles()
{
	for (auto& particle : m_ParticlePool)
	{
		particle.Active = false;  // 确保每个粒子都标记为非活跃
	}
}

void ParticleSystem::CursorParticleEmit(Ciallo::OrthographicCameraController& camera)
{
	if (Ciallo::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
	{

		auto [x, y] = Ciallo::Input::GetMousePos();
		auto win_x = Ciallo::Application::Get().GetWindow().GetWidth();
		auto win_y = Ciallo::Application::Get().GetWindow().GetHeight();

		auto& Bounds = camera.GetBounds();

		auto worldPosX = (x / win_x) * Bounds.GetWidth() - Bounds.GetWidth() * 0.5;
		auto worldPosY = Bounds.GetHeight() * 0.5 - (y / win_y) * Bounds.GetHeight();
		auto CameraPos = camera.GetCamera().GetPosition();

		glm::vec4 postion = { worldPosX + CameraPos.x,worldPosY + CameraPos.y,0.0f,1.0f };
		postion = glm::rotate(glm::mat4(1.0f), glm::radians(camera.GetCameraRotation()), glm::vec3(0.0f, 0.0f, 1.0f)) * postion;

		defaultPartiprops.Position = postion;

		Emit(defaultPartiprops);
		
	}

}
