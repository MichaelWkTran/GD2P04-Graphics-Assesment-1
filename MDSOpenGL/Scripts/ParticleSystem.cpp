#include "ParticleSystem.h"
#include <math.h>

CParticleSystem::CParticleSystem()
{
	m_particles = 4000U;
}

CParticleSystem::~CParticleSystem()
{
}

void CParticle::ResetToInitialValues()
{
	//m_elapsedTime = 0.0f;
	//m_position = origin;
	//m_velocity = glm::vec3(
	//	0.25f * cosf(this->id * .0167) + 0.25f * randomFloat() - 0.125f,
	//	1.5f + 0.25f * randomFloat() - 0.125f,
	//	0.25 * sin(this->id * .0167) + 0.25f * randomFloat() - 0.125f
	//);
	//
	//this->elapsedTime = randomFloat() + 0.125;
}
