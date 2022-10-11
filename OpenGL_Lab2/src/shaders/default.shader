#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 position;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	position = aPos;
}

#shader fragment
#version 330 core


in vec3 position;

out vec4 FragColor;


void main()
{
	FragColor = vec4(1 - position.x, 1 - position.y, 1 - position.y, 1);
}