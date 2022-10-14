#include "LightCube.h"

LightCube::LightCube(glm::vec3 color, int lightIndex, Shader *lightShader, Shader *litObjectShader)
{
	this->color = color;
	m_LightShader = lightShader;
	m_LitObjectShader = litObjectShader;
	m_LightIndex = lightIndex;

	vertices = m_CubeVertices;
	indices = m_CubeIndices;
	SetVAO();

	// Refresh shaders
	Move(0, 0, 0);
	SetColor(color);
	SetIntencity(m_Intencity);
}

void LightCube::Move(float x, float y, float z, bool addToPreviousPosition)
{
	lightPosition = addToPreviousPosition ? lightPosition + glm::vec3(x, y, z) : glm::vec3(x, y, z);
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), lightPosition);

	m_LightShader->Bind();
	m_LightShader->SetUniformMat4f("model", m_ModelMatrix);

	m_LitObjectShader->Bind();
	m_LitObjectShader->SetUniform3f("lights[" + std::to_string(m_LightIndex) + "].position", lightPosition);
	m_LitObjectShader->Unbind();
}



void LightCube::SetColor(glm::vec3 color)
{
	this->color = color;

	m_LightShader->Bind();
	m_LightShader->SetUniform3f("lightColor", color);

	m_LitObjectShader->Bind();
	m_LitObjectShader->SetUniform3f("lights[" + std::to_string(m_LightIndex) + "].color", color);
	m_LitObjectShader->Unbind();
}


void LightCube::SetIntencity(float m_Intencity)
{
	this->m_Intencity = m_Intencity;
	m_LitObjectShader->Bind();
	m_LitObjectShader->SetUniform1f("lights[" + std::to_string(m_LightIndex) + "].intencity", m_Intencity);
	m_LitObjectShader->Unbind();
}

void LightCube::Render(Camera& camera) { Mesh::Render(*m_LightShader, camera); }