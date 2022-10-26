#include "ParticleSystem.h"
#include "Main.h"
#include "Shader.h"
#include "MathUtils.h"
#include "Camera.h"

CShader* CParticleSystem::m_particleShader = nullptr;

CParticleSystem::CParticleSystem(unsigned int _particleCount)
{
	if (m_particleShader == nullptr) m_particleShader = new CShader("Particle.vert", "Particle.geom", "Particle.frag");
	m_position = {};

	//Create Particles
	m_particleCount = _particleCount;
	for (unsigned particleIndex = 0U; particleIndex < m_particleCount; particleIndex++)
		m_particles.push_back(CParticle(this));
	m_particlePositions.resize(m_particleCount);

	//Create Mesh Buffers
	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);

	//Assign Vertex Attributes
	glBindVertexArray(m_vertexArray); glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0 /*Position*/, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBindVertexArray(0); glBindBuffer(GL_ARRAY_BUFFER, 0);
}

CParticleSystem::~CParticleSystem()
{
	glDeleteVertexArrays(1, &m_vertexArray);
	glDeleteBuffers(1, &m_vertexBuffer);
}

void CParticleSystem::Update()
{
	//Update particles and get their position data
	for (int i = 0; i < m_particleCount; i++)
	{
		m_particles[i].Update();
		m_particlePositions[i] = m_particles[i].m_position;
	}

	//Buffer Vertex Data
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_particleCount * sizeof(glm::vec3), m_particlePositions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CParticleSystem::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glUseProgram(*m_particleShader);

	glUniform3f(glGetUniformLocation(*m_particleShader, "uni_cameraUp"),
		GetMainCamera().m_transform.Up().x, GetMainCamera().m_transform.Up().y, GetMainCamera().m_transform.Up().z);
	glUniform3f(glGetUniformLocation(*m_particleShader, "uni_cameraRight"),
		GetMainCamera().m_transform.Right().x, GetMainCamera().m_transform.Right().y, GetMainCamera().m_transform.Right().z);

	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_POINTS, 0, m_particleCount);
	glBindVertexArray(0);
	
	glUseProgram(0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

CParticle::CParticle(CParticleSystem* _particleSystem)
{
	m_particleSystem = _particleSystem;
	ResetToInitialValues();
}

void CParticle::ResetToInitialValues()
{
	m_elapsedTime = 0.0f;
	m_position = m_particleSystem->m_position;
	
	float minSpreadSpeed = 0.0f; float maxSpreadSpeed = 0.0f;
	float minRiseSpeed = 0.0f; float maxRiseSpeed = 0.0f;
	float minElapsedTime = 1.0f; float maxElapsedTime = 1.5f;

	m_velocity = glm::vec3(
		cosf(2.0f * glm::pi<float>() * glm::Lerp(minSpreadSpeed, maxSpreadSpeed, (float)rand()/(double)RAND_MAX)),
		glm::Lerp(minRiseSpeed, maxRiseSpeed, (float)rand()/(double)RAND_MAX),
		sinf(2.0f * glm::pi<float>() * glm::Lerp(minSpreadSpeed, maxSpreadSpeed, (float)rand()/(double)RAND_MAX))
	);
	
	m_elapsedTime = glm::Lerp(minElapsedTime, maxElapsedTime, (float)rand()/(double)RAND_MAX);
}

void CParticle::Update()
{
	m_velocity.y += -0.2f * e_deltatime;
	m_position += m_velocity;
	
	//Reset the particle if its elapsed time has finished
	m_elapsedTime -= e_deltatime / 100.0f;
	if (m_elapsedTime <= 0.0f) ResetToInitialValues();
}