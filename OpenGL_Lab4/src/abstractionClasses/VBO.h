#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texUV;
	glm::vec3 normal;
};

class VBO
{
private:
	GLuint m_ID;
public:
	VBO(std::vector<Vertex>& vertices);
	void Bind();
	void Unbind();
	void Delete();
};

