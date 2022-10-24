#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <cmath>
#include <glfw3.h>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const unsigned int width = 960;
const unsigned int height = 960;

using std::cout;
using std::vector;

vector<GLfloat> vertices;
vector<GLuint> indices;

VBO *VBO1;
VAO* VAO1;
EBO *EBO1;

void RefreshMesh();
void RecalculateIndicies();

#pragma region Shapes
void SetRightTriangleVerticies()
{
	vertices = { 0.f, 0.f, 0.f, 0.5f, 0.f, 0.f, 0.f, 0.5f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}

void SetIsoscelesTriangleVertices()
{
	vertices = { -0.5f, 0.f, 0.f, 0.5f, 0.f, 0.f, 0.f, 0.5f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}

void SetIsoscelesTrapezoidVertices()
{
	vertices = { -0.5f, 0.f, 0.f, -0.3f, 0.5f, 0.f, 0.3f, 0.5f, 0.f, 0.5f, 0.0f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}

void SetRightTrapezoidVertices()
{
	vertices = { -0.5f, 0.f, 0.f, -0.5f, 0.5f, 0.f, 0.3f, 0.5f, 0.f, 0.5f, 0.0f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}

void SetParallelogramVertices()
{
	vertices = { -0.5f, 0.f, 0.f, -0.3f, 0.5f, 0.f, 0.8f, 0.5f, 0.f, 0.5f, 0.0f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}

void SetRectangleVertices()
{
	vertices = { -0.5f, 0.f, 0.f, -0.5f, 0.3f, 0.f, 0.5f, 0.3f, 0.f, 0.5f, 0.0f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}

void SetDeltoidVertices()
{
	vertices = { 0.f, -0.5f, 0.f, -0.3f, 0.f, 0.f, 0.f, 0.3f, 0.f, 0.3f, 0.0f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}

void SetRhombusVertices()
{
	vertices = { 0.f, -0.7f, 0.f, -0.3f, 0.f, 0.f, 0.f, 0.7f, 0.f, 0.3f, 0.0f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}

void GenerateRegularPolygon(float x0, float y0, float radius, int n)
{
	vertices.clear();
	for (int i = 0; i < n; i++)
	{
		vertices.push_back(x0 + radius * cos(2.f * 3.14f * i / n));
		vertices.push_back(y0 + radius * sin(2.f * 3.14f * i / n));
		vertices.push_back(0);
	}
	RecalculateIndicies();
	RefreshMesh();
}
#pragma endregion

void ShowRandomPoints(int n)
{
	vertices.clear();
	for (int i = 0; i < n; i++)
	{
		vertices.push_back(static_cast<float>(rand() % 100) / 50 - 1);
		vertices.push_back(static_cast<float>(rand() % 100) / 50 - 1);
		vertices.push_back(0);
	}
	RecalculateIndicies();
	RefreshMesh();
}



int main()
{
	if (!glfwInit())
		return -1;
	srand(time(0));

	GLFWwindow* window = glfwCreateWindow(width, height, "GL App", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		cout << "Glew Init Error!";
		return -1;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	Shader shader("./src/shaders/default.shader");

	int polygonPointsCount = 3;
	GenerateRegularPolygon(0.f, 0.f, 0.3f, polygonPointsCount);
	RefreshMesh();

	int frontPolygonsMode = GL_FILL;
	int backPolygonsMode = GL_FILL;

	int randomPointsCount = 15;

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader.Bind();
		VAO1->Bind();

		// Draw back
		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, backPolygonsMode);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		// Draw front
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, frontPolygonsMode);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		shader.Unbind();

#pragma region GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Polygon mode");
		if (ImGui::Button("GL_FILL"))
			frontPolygonsMode = backPolygonsMode = GL_FILL;
		else if (ImGui::Button("GL_LINE"))
			frontPolygonsMode = backPolygonsMode = GL_LINE;
		else if (ImGui::Button("GL_POINT"))
			frontPolygonsMode = backPolygonsMode = GL_POINT;
		ImGui::End();

		ImGui::Begin("Shape");
		ImGui::SliderInt("Points", &polygonPointsCount, 3, 10);
		if (ImGui::Button("Show polygon"))
			GenerateRegularPolygon(0.f, 0.f, 0.3f, polygonPointsCount);
		ImGui::Text("Triangle");
		if (ImGui::Button("Right"))
			SetRightTriangleVerticies();
		else if (ImGui::Button("Isosceles"))
			SetIsoscelesTriangleVertices();
		ImGui::Text("Trapezoid");
		if (ImGui::Button("Isosceles Trapezoid"))
			SetIsoscelesTrapezoidVertices();
		else if (ImGui::Button("Right Trapezoid"))
			SetRightTrapezoidVertices();
		ImGui::Text("Other");
		if (ImGui::Button("Parallelogram"))
			SetParallelogramVertices();
		else if (ImGui::Button("Rectangle"))
			SetRectangleVertices();
		else if (ImGui::Button("Deltoid"))
			SetDeltoidVertices();
		else if (ImGui::Button("Rhombus"))
			SetRhombusVertices();;
		ImGui::End();

		ImGui::Begin("Face culling");
		if (ImGui::Button("Enable culling"))
		{
			glEnable(GL_CULL_FACE);
		}
		else if (ImGui::Button("Disable culling"))
		{
			glDisable(GL_CULL_FACE);
		}
		else if (ImGui::Button("Show front as points"))
		{
			frontPolygonsMode = GL_POINT;
		}
		else if (ImGui::Button("Show front filled and back as lines"))
		{
			backPolygonsMode = GL_LINE;
			frontPolygonsMode = GL_FILL;
		}
		else if (ImGui::Button("Show wireframe"))
		{
			backPolygonsMode = frontPolygonsMode = GL_LINE;
		}
		ImGui::End();

		ImGui::Begin("Random Generation");
		ImGui::SliderInt("Number of points", &randomPointsCount, 5, 30);
		if (ImGui::Button("Generate"))
			ShowRandomPoints(randomPointsCount);
		ImGui::End();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void RecalculateIndicies()
{
	indices.clear();
	for (int i = 0; i < vertices.size() / 3 - 2; i++)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
	}
}

void RefreshMesh()
{
	VBO1 = new VBO(&vertices[0], sizeof(GLfloat) * vertices.size());

	VAO1 = new VAO();
	VAO1->Bind();

	EBO1 = new EBO(&indices[0], sizeof(GLuint) * indices.size());

	VAO1->LinkAttrib(*VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	VAO1->Unbind();
	VBO1->Unbind();
	EBO1->Unbind();
}