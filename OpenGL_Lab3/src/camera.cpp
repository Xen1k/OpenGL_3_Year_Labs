#include "camera.h"
#include "timeManager.h"
#include "imgui.h"


Camera::Camera(int width, int height, glm::vec3 position)
{
	this->width = width;
	this->height = height;
	this->position = position;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform, Projection projection)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(position, position + orientation, up);
	//// Adds perspective to the scene
	if(projection == Projection::Ortho)
		projectionMatrix = glm::ortho(-5.f, 5.f, -5.f, 5.f, nearPlane, farPlane);
	else
		projectionMatrix = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Exports the camera matrix to the Vertex Shader
	shader.SetUniformMat4f(uniform, projectionMatrix * view);
}



void Camera::HandleInputs(GLFWwindow* window, bool stopMouseWindow)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * orientation;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * -glm::normalize(glm::cross(orientation, up));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * -orientation;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * glm::normalize(glm::cross(orientation, up));
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * up;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		position += Time::GetDeltaTime() * speed * -up;
	

	if (stopMouseWindow)
		return;

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			orientation = newOrientation;

		// Rotates the Orientation left and right
		orientation = glm::rotate(orientation, glm::radians(-rotY), up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}