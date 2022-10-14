#pragma once
#include "Mesh.h"
#include <vector>

using std::vector;

class LightCube : public Mesh
{
private:
	vector<Vertex> m_CubeVertices =
	{
		Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
	};
	vector<GLuint> m_CubeIndices =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};
	Shader* m_LightShader;
	Shader *m_LitObjectShader;
	float m_Intencity = 1.f;
	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	int m_LightIndex;

public:
	LightCube(glm::vec3 color, int lightIndex, Shader* lightShader, Shader* litObjectShader);
	void Move(float x, float y, float z, bool addToPreviousPosition = true);
	void SetColor(glm::vec3 color);
	void SetIntencity(float m_Intencity);
	void Render(Camera& camera);

	inline float const GetIntencity() { return m_Intencity; }


	glm::vec3 color = glm::vec3(1.f, 0.f, 0.0f);
	glm::vec3 lightPosition = glm::vec3(1.5f, 0.7f, -1.5f);

};

