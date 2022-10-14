#shader vertex
#version 330 core


layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNormal;

uniform mat4 camMatrix;
uniform mat4 model;

out vec2 texCoord;
out vec3 normal;
out vec3 currentPosition;

void main()
{
	currentPosition = vec3(model * vec4(aPos, 1.f));
	gl_Position = camMatrix * vec4(currentPosition, 1.f);
	texCoord = aTex;
	normal = aNormal;
}

#shader fragment
#version 330 core
const int MAX_LIGHTS = 5;

struct BasicLight
{
	float intencity;
	vec3 position;
	vec3 color;
};

uniform int numOfLights;
uniform BasicLight lights[MAX_LIGHTS];

uniform float hasTexture;
uniform sampler2D tex0;

in vec2 texCoord;
in vec3 normal;
in vec3 currentPosition;

out vec4 FragColor;


vec3 CalculateDiffuseLight(BasicLight light)
{
	vec3 lightDirection = normalize(light.position - currentPosition);
	float diffuse = max(dot(normalize(normal), lightDirection), 0.f);
	return light.color * diffuse * light.intencity;
}

void main()
{
	float ambientLight = 0.15f;

	vec3 diffuseLightsResult = vec3(0.f);
	for (int i = 0; i < numOfLights; i++)
		diffuseLightsResult += CalculateDiffuseLight(lights[i]);

	vec4 lightResult = vec4(diffuseLightsResult + ambientLight, 1.f);
	FragColor = (hasTexture != 0.f ? texture(tex0, texCoord) : vec4(1.f)) * lightResult;
}