#pragma once

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"


class Mesh
{
private:
	VAO m_VAO;
protected:
	void SetVAO();
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Texture* texture;

	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, Texture* texture = nullptr);
	Mesh() {};

	void Render(Shader& shader, Camera& camera);
};

