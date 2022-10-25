#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "UpdatedObject.h"
#include "Mesh.h"

class CParticle
{
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	float m_elapsedTime;

	CParticle();
	~CParticle();
	void ResetToInitialValues();
};

class CParticleSystem : public CUpdatedObject
{
private:
	unsigned int m_particleCount;

public:
	std::vector<CParticle> m_particles;
	CMesh<> m_particleMesh;

	CParticleSystem();
	~CParticleSystem();


	virtual void Draw() override;
};

