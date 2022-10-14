#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Texture *texture)
{
	this->vertices = vertices;
	this->indices = indices;
	if(texture)
		this->texture = texture;

	SetVAO();
}

void Mesh::SetVAO()
{
	m_VAO.Bind();
	VBO VBO(vertices);
	EBO EBO(indices);
	m_VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	m_VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));

	m_VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Render(Shader& shader, Camera& camera)
{
	shader.Bind();
	m_VAO.Bind();

	if (texture)
	{
		shader.SetUniform1f("hasTexture", 1.f);
		texture->texUnit(shader, "tex0", 0);
		texture->Bind();
	}
	else if (glGetUniformLocation(shader.ID, "hasTexture") != -1)
	{
		shader.SetUniform1f("hasTexture", 0.f);
	}

	camera.UpdateMatrix(shader, "camMatrix");
	
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}