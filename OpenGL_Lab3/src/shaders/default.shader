#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 position;
uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	gl_Position = camMatrix * model * vec4(aPos, 1.0);
	position = aPos;
}

#shader fragment
#version 330 core


in vec3 position;
uniform vec3 color;
out vec4 FragColor;


void main()
{
	FragColor = new vec4(color - position, 1);
	//FragColor = new vec4(position.x, position.y, position.z, 1);
}