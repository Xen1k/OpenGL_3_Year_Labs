#pragma once

#include <GL/glew.h>
#include "VBO.h"

class VAO
{
private:
	GLuint m_ID;

public:

	VAO();
	void Bind();
	void Unbind();
	void Delete();
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
};

