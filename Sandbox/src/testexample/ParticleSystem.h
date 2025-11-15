#pragma once

#include "Ciallo.h"

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec3 BeginColor, EndColor;
	float BeginSize, EndSize, SizeVariation;
	float LiftTime = 1.0f;
};


class ParticleSystem
{
public:
	ParticleSystem();

	void Emit(const ParticleProps& particleProps);

	void OnUpdate(Ciallo::TimeStep ts);

	void OnRenderer();

	void ClearParticles();

	void CursorParticleEmit(Ciallo::OrthographicCameraController& camera);

	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec3 BeginColor, EndColor;
		float BeginSize, EndSize, SizeVariation;
		float LifeRemaining = 0.0f;
		float Rotation = 0.0f;
		float LiftTime = 1.0f;

		bool Active = false;
	};

private:
	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex = 999;
};