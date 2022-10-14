#include <iostream>
#include <GL/glew.h>
#include <glfw3.h>
#include <stb/stb_image.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"
#include "camera.h"
#include "Mesh.h"
#include "LightCube.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "timeManager.h"

constexpr float PI = 3.1415926535979f;


const unsigned int width = 660;
const unsigned int height = 660;


using std::cout;
using std::vector;

vector<Vertex> vertices;

vector<GLuint> indices;

vector<Vertex> lightVertices =
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

vector<GLuint> lightIndices =
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

void InitializeDependenciesAndWindow(GLFWwindow** window);
void SetCubeVertices();
void SetSphereVertices(float radius, unsigned int rings, unsigned int sectors);
void SetPyramidVertices();

Mesh* mesh;

int main()
{
	GLFWwindow* window;
	InitializeDependenciesAndWindow(&window);


	Shader* litShader = new Shader("./src/shaders/lit.shader");

	litShader->Bind();
	litShader->SetUniform1i("numOfLights", 2);
	litShader->SetUniformMat4f("model", glm::mat4(1.f));

	SetSphereVertices(0.5f, 25, 25);
	mesh = new Mesh(vertices, indices);
	Texture *texture = new Texture("./textures/pixel.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

	LightCube light1(glm::vec3(1.f, 0.f, 0.0f), 0, new Shader("./src/shaders/unlit.shader"), litShader);
	LightCube light2(glm::vec3(1.0f, 1.0f, 0.0f), 1, new Shader("./src/shaders/unlit.shader"), litShader);


	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	bool mouseIsOverMeshGui = false;
	bool mouseIsOverControlsGui = false;

	int polygonMode = GL_FILL;

	bool light1IsEnabled = true, light2IsEnabled = true;

	// Blue color change
	float blueColor = 0.f;
	float blueColorChangeSpeed = 5.f;
	bool* changeBlueChannel = new bool();

	glfwSetWindowUserPointer(window, changeBlueChannel);
	glfwSetKeyCallback(window,
		[](GLFWwindow* window, int key, int scancode, int action, int mode) {
			bool* changeBlueChannel = (bool*)glfwGetWindowUserPointer(window);
			if (key == GLFW_KEY_C && action == GLFW_PRESS)
				*changeBlueChannel = !*changeBlueChannel;
		}
	);


	// Light 2 rotation
	float rotationAngle = 0.f;
	float rotationRadius = light2.lightPosition.x;
	bool rotateLight = false;
	float rotationSpeed = 2.f;

	while (!glfwWindowShouldClose(window))
	{
		Time::SetNow();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

		if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
			light1.Move(10.f * Time::GetDeltaTime(), 0.f, 0.f);
		if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
			light1.Move(-10.f * Time::GetDeltaTime(), 0.f, 0.f);
		if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
			light1.Move(0.f, 0.f, -10.f * Time::GetDeltaTime());
		if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
			light1.Move(0.f, 0.f, 10.f * Time::GetDeltaTime());
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			light1.Move(0.f, 20.f * Time::GetDeltaTime(), 0.f);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			light1.Move(0.f, -20.f * Time::GetDeltaTime(), 0.f);

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			light1.SetIntencity(light1.GetIntencity() >= 1 ? 0.1f : light1.GetIntencity() + 0.05f);
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
			light2.SetIntencity(light2.GetIntencity() >= 1 ? 0.1f : light2.GetIntencity() + 0.05f);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			rotationSpeed += 0.1f;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			rotationSpeed -= 0.1f;

		camera.HandleInputs(window, mouseIsOverMeshGui || mouseIsOverControlsGui);
		
		if (rotateLight)
		{
			light2.Move(rotationRadius * cos(rotationAngle), light2.lightPosition.y, rotationRadius * sin(rotationAngle), false);
			rotationAngle += rotationSpeed * Time::GetDeltaTime();
		}

		mesh->Render(*litShader, camera);
		if(light1IsEnabled) light1.Render(camera);
		if(light2IsEnabled) light2.Render(camera);

#pragma region GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Controls", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize(ImVec2(width, 90));
		ImGui::SetWindowPos(ImVec2(0, height - ImGui::GetWindowSize().y));
		mouseIsOverControlsGui = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
		ImGui::Text("C - Switch color animation of light 1");
		ImGui::Text("P/Y - Change light's intencity");
		ImGui::Text("<-/-> - Change rotation speed");
		ImGui::End();
		
		ImGui::Begin("Mesh/Texture/Light");
		mouseIsOverMeshGui = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
		ImGui::Text("Mesh");
		if (ImGui::Button("Sphere"))
			SetSphereVertices(0.5f, 25, 25);
		if (ImGui::Button("Pyramid"))
			SetPyramidVertices();
		if (ImGui::Button("Cube"))
			SetCubeVertices();
		ImGui::Text("View");
		if (ImGui::Button("Switch Texture"))
			mesh->texture = mesh->texture ? nullptr : texture;
		if (ImGui::Button("Switch Polygon Mode"))
			polygonMode = polygonMode == GL_FILL ? GL_LINE : GL_FILL;
		ImGui::Text("Lights");
		if (ImGui::Button("Switch light 1"))
		{
			light1IsEnabled = !light1IsEnabled;
			light1.SetIntencity(light1IsEnabled ? 1.f : 0.f);
		}
		if (ImGui::Button("Switch light 2"))
		{
			light2IsEnabled = !light2IsEnabled;
			light2.SetIntencity(light2IsEnabled ? 1.f : 0.f);
		}
		if (ImGui::Button("Switch light rotation"))
			rotateLight = !rotateLight;
		ImGui::End();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

		if (*changeBlueChannel)
		{
			if (blueColor > 1.f || blueColor < 0.f)
			{
				blueColorChangeSpeed *= -1;
				blueColor = round(blueColor);
			}
			blueColor += blueColorChangeSpeed * Time::GetDeltaTime();
			light1.SetColor(glm::vec3(light1.color.x, light1.color.y, blueColor));
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		Time::Calculate();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void InitializeDependenciesAndWindow(GLFWwindow** window)
{
	if (!glfwInit())
		cout << "Glfw Init Error!";
	*window = glfwCreateWindow(width, height, "GL App", NULL, NULL);
	glfwMakeContextCurrent(*window);
	if (glewInit() != GLEW_OK)
		cout << "Glew Init Error!";
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(*window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void SetPyramidVertices()
{
	vertices =
	{
		// Bottom
		Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
		Vertex{glm::vec3(-0.5f, 0.0f,  -0.5f), glm::vec2(0.0f, 5.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
		Vertex{glm::vec3(0.5f, 0.0f,  -0.5f), glm::vec2(5.0f, 5.0f), glm::vec3(0.0f, -1.0f, 0.0f)},
		Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},

		// Left
		Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-0.8f, 0.5f, 0.0f)},
		Vertex{glm::vec3(-0.5f, 0.0f,  -0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(-0.8f, 0.5f, 0.0f)},
		Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(-0.8f, 0.5f, 0.0f)},

		// Back
		Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(5.0f, 0.f), glm::vec3(0.0f, 0.5f, -0.8f)},
		Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.5f, -0.8f)},
		Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(0.0f, 0.5f, -0.8f)},

		// Front
		Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.8f, 0.5f,  0.0f)},
		Vertex{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(0.8f, 0.5f,  0.0f)},
		Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(0.8f, 0.5f,  0.0f)},

		// Front
		Vertex{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(5.0f, 0.0f), glm::vec3(0.0f, 0.5f,  0.8f)},
		Vertex{glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.5f,  0.8f)},
		Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec2(2.5f, 5.0f), glm::vec3(0.0f, 0.5f,  0.8f)},
	};

	indices =
	{
		0, 1, 2,
		0, 2, 3,
		4, 6, 5,
		7, 9, 8,
		10, 12, 11,
		13, 15, 14
	};

	delete mesh;
	mesh = new Mesh(vertices, indices);
}

void SetSphereVertices(float radius, unsigned int rings, unsigned int sectors)
{
	std::vector<float> vertices_;
	std::vector<float> normals;
	std::vector<float> texCoords;

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / rings;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= rings; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectors; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices_.push_back(x);
			vertices_.push_back(y);
			vertices_.push_back(z);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectors;
			t = (float)i / rings;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	int j = 0;
	vertices.clear();
	for (int i = 0; i < vertices_.size(); i += 3, j += 2)
		vertices.push_back(Vertex{
			glm::vec3(vertices_[i], vertices_[i + 1], vertices_[i + 2]),
			glm::vec2(texCoords[j], texCoords[j + 1]),
			glm::vec3(normals[i], normals[i + 1], normals[i + 2])
			});

	indices.clear();
	int k1, k2;
	for (int i = 0; i < rings; ++i)
	{
		k1 = i * (sectors + 1);     // beginning of current stack
		k2 = k1 + sectors + 1;      // beginning of next stack

		for (int j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (rings - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

		}
	}

	delete mesh;
	mesh = new Mesh(vertices, indices);
}

void SetCubeVertices()
{
	vertices =
	{
		// Left
		Vertex{glm::vec3(-1, -1	, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)},
		Vertex{glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1)},
		Vertex{glm::vec3(1, 1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1)},
		Vertex{glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)},
		// Rear
		Vertex{glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(1, 0, 0)},
		Vertex{glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0)},
		Vertex{glm::vec3(1, 1, 1), glm::vec2(1, 1), glm::vec3(1, 0, 0)},
		Vertex{glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0)},
		//Right
		Vertex{glm::vec3(1, -1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)},
		Vertex{glm::vec3(-1, -1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)},
		Vertex{glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)},
		Vertex{glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)},
		//Front
		Vertex{glm::vec3(-1, -1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0)},
		Vertex{glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0)},
		Vertex{glm::vec3(-1, 1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)},
		Vertex{glm::vec3(-1, 1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0)},
		//Top
		Vertex{glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 1, 0)},
		Vertex{glm::vec3(-1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 1, 0)},
		Vertex{glm::vec3(-1, 1, -1), glm::vec2(1, 1), glm::vec3(0, 1, 0)},
		Vertex{glm::vec3(1, 1, -1), glm::vec2(1, 0), glm::vec3(0, 1, 0)},
		//Bottom
		Vertex{glm::vec3(1, -1, 1), glm::vec2(0, 0), glm::vec3(0, -1, 0)},
		Vertex{glm::vec3(-1, -1, 1), glm::vec2(0, 1), glm::vec3(0, -1, 0)},
		Vertex{glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(0, -1, 0)},
		Vertex{glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(0, -1, 0)},
	};

	indices.clear();

	for (unsigned int i = 0; i < 36; i += 4)
	{
		indices.insert(indices.end(), { i, i + 1, i + 2 });
		indices.insert(indices.end(), { i, i + 2, i + 3 });
	}


	delete mesh;
	mesh = new Mesh(vertices, indices);
}