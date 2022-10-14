#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.h"

enum class Projection { Ortho, Perspective };

class Camera
{
private:
	bool firstClick = true;
public:
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	int width;
	int height;

	float speed = 10.f;
	float sensitivity = 100.f;

	Camera(int width, int height, glm::vec3 position);

	void UpdateMatrix(float fovDeg, float nearPlane, float farPlane, Shader &shader, const char* uniform, Projection projection = Projection::Perspective );
	void HandleInputs(GLFWwindow *window, bool stopMouseInput = false);
};

