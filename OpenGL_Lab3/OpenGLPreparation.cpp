#pragma comment(lib, "GlU32.Lib")

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <cmath>
#include <glfw3.h>
#include <algorithm>
#include <deque>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "camera.h"
#include "timeManager.h"

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
void RecalculateIndicies(bool startZero = true);

#pragma region Flat Shapes
void SetRightTriangleVerticies()
{
	vertices = { 0.f, 0.f, 0.f, 0.5f, 0.f, 0.f, 0.f, 0.5f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}

void SetRectangleVertices()
{
	vertices = { -0.5f, 0.f, 0.f, -0.5f, 0.3f, 0.f, 0.5f, 0.3f, 0.f, 0.5f, 0.0f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}

void SetRhombusVertices()
{
	vertices = { 0.f, -0.7f, 0.f, -0.3f, 0.f, 0.f, 0.f, 0.7f, 0.f, 0.3f, 0.0f, 0.f };
	RecalculateIndicies();
	RefreshMesh();
}
#pragma endregion

float CalculateHyperboloidY(float x, float z, float a, float b, float c) { return  b * sqrt(z * z / (c * c) - x * x / (a * a) - 1); }

void SetTwoSheetedHyperboloidVertices()
{
	float a = 0.5f, b = 0.5f, c = 1.f;
	float y;
	float subdivision = 0.1f;
	float zSubdivision = 0.3f;
	vertices.clear();
	//for (float x = 0; x <= a * 2; x += subdivision)
	//{
	//	for (float z = c; z <= 2 * c; z += zSubdivision)
	//	{
	//		y = b * sqrt(z * z / (c * c) - x * x / (a * a) - 1);
	//		//y = sqrt((a * a - x * x) * b * b / (a * a));
	//		vertices.push_back(x);
	//		vertices.push_back(y);
	//		vertices.push_back(z);

	//		vertices.push_back(x);
	//		vertices.push_back(-y);
	//		vertices.push_back(z);

	//		vertices.push_back(-x);
	//		vertices.push_back(y);
	//		vertices.push_back(z);

	//		vertices.push_back(-x);
	//		vertices.push_back(-y);
	//		vertices.push_back(z);

	//	}

	//}
	float maxX = a;
	float maxZ = c + 5 * zSubdivision;

	vector<GLfloat> vertices1, vertices2, vertices3, vertices4;

	//float maxZ = c + 3 * zSubdivision;
	for (float x = 0; x < maxX; x += subdivision)
	{
		for (float z = c; z <= maxZ; z += zSubdivision)
		{
			y = CalculateHyperboloidY(x, z, a, b, c);

			if (isnan(y))
				continue;

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(-z);

		}
	}

	for (float x = maxX - subdivision; x >= subdivision; x -= subdivision)
	{
		for (float z = c; z <= maxZ; z += zSubdivision)
		{
			y = CalculateHyperboloidY(x, z, a, b, c);
			if (isnan(y))
				continue;
			vertices.push_back(x);
			vertices.push_back(-y);
			vertices.push_back(-z);
		}
	}

	for (float x = subdivision; x < maxX; x += subdivision)
	{
		for (float z = c; z <= maxZ; z += zSubdivision)
		{
			y = CalculateHyperboloidY(x, z, a, b, c);
			if (isnan(y))
				continue;
			vertices.push_back(-x);
			vertices.push_back(-y);
			vertices.push_back(-z);
		}
	}

	for (float x = maxX - subdivision; x >= subdivision; x -= subdivision)
	{
		for (float z = c; z <= maxZ; z += zSubdivision)
		{
			y = CalculateHyperboloidY(x, z, a, b, c);
			if (isnan(y))
				continue;
			vertices.push_back(-x);
			vertices.push_back(y);
			vertices.push_back(-z);
		}
	}
	indices.clear();

	for (int i = 0; i < vertices.size(); i += 3)
	{
		cout << "| " << vertices[i] << ", " << vertices[i + 1] << ", " << vertices[i + 2] << " |" << std::endl;
	}
	cout << (float)vertices.size() / 3;

	// First Layer indicies
	for (int i = 0; i < (maxX / subdivision - 1) * 4; i++)
	{
		indices.push_back(0);
		indices.push_back(1 + (maxZ - c) / zSubdivision * i);
		indices.push_back(1 + (maxZ - c) / zSubdivision * (i + 1));
	}
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(1 + (maxZ - c) / zSubdivision * (maxX / subdivision - 1) * 4);

	if (indices.size() == 0)
		for (int i = 0; i < vertices.size() / 3 - 1; i++)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
		}

	RefreshMesh();
}
//void SetTwoSheetedHyperboloidVertices()
//{
//	float a = 0.5f, b = 0.5f, c = 1.f;
//	float y;
//	float subdivision = 0.1f;
//	float zSubdivision = 0.3f;
//	vertices.clear();
//	//for (float x = 0; x <= a * 2; x += subdivision)
//	//{
//	//	for (float z = c; z <= 2 * c; z += zSubdivision)
//	//	{
//	//		y = b * sqrt(z * z / (c * c) - x * x / (a * a) - 1);
//	//		//y = sqrt((a * a - x * x) * b * b / (a * a));
//	//		vertices.push_back(x);
//	//		vertices.push_back(y);
//	//		vertices.push_back(z);
//
//	//		vertices.push_back(x);
//	//		vertices.push_back(-y);
//	//		vertices.push_back(z);
//
//	//		vertices.push_back(-x);
//	//		vertices.push_back(y);
//	//		vertices.push_back(z);
//
//	//		vertices.push_back(-x);
//	//		vertices.push_back(-y);
//	//		vertices.push_back(z);
//
//	//	}
//
//	//}
//	float maxX = a;
//	float maxZ = c + 5 * zSubdivision;
//
//	vector<GLfloat> vertices1, vertices2, vertices3, vertices4;
//
//	//float maxZ = c + 3 * zSubdivision;
//	for (float z = c; z <= maxZ; z += zSubdivision)
//	{
//		for (float x = 0; x < maxX * z; x += subdivision)
//		{
//			y = CalculateHyperboloidY(x, z, a, b, c);
//
//			if (isnan(y))
//				continue;
//
//			vertices.push_back(x);
//			vertices.push_back(y);
//			vertices.push_back(-z);
//
//		}
//	}
//
//	for (float z = c; z <= maxZ; z += zSubdivision)
//	{
//		for (float x = maxX - subdivision; x >= subdivision; x -= subdivision)
//		{
//			y = CalculateHyperboloidY(x, z, a, b, c);
//			if (isnan(y))
//				continue;
//			vertices.push_back(x);
//			vertices.push_back(-y);
//			vertices.push_back(-z);
//		}
//	}
//
//	for (float z = c; z <= maxZ; z += zSubdivision)
//	{
//		for (float x = subdivision; x < maxX; x += subdivision)
//		{
//			y = CalculateHyperboloidY(x, z, a, b, c);
//			if (isnan(y))
//				continue;
//			vertices.push_back(-x);
//			vertices.push_back(-y);
//			vertices.push_back(-z);
//		}
//	}
//
//	for (float z = c; z <= maxZ; z += zSubdivision)
//	{
//		for (float x = maxX - subdivision; x >= subdivision; x -= subdivision)
//		{
//			y = CalculateHyperboloidY(x, z, a, b, c);
//			if (isnan(y))
//				continue;
//			vertices.push_back(-x);
//			vertices.push_back(y);
//			vertices.push_back(-z);
//		}
//	}
//	indices.clear();
//
//	for (int i = 0; i < vertices.size(); i += 3)
//	{
//		cout << "| " << vertices[i] << ", " << vertices[i + 1] << ", " << vertices[i + 2] << " |" << std::endl;
//	}
//	cout << (float)vertices.size() / 3;
//
//	// First Layer indicies
//	//for (int i = 0; i < (maxX / subdivision - 1) * 4; i++)
//	//{
//	//	indices.push_back(0);
//	//	indices.push_back(1 + (maxZ - c) / zSubdivision * i);
//	//	indices.push_back(1 + (maxZ - c) / zSubdivision * (i + 1));
//	//}
//	//indices.push_back(0);
//	//indices.push_back(1);
//	//indices.push_back(1 + (maxZ - c) / zSubdivision * (maxX / subdivision - 1) * 4);
//
//	if (indices.size() == 0)
//		for (int i = 0; i < vertices.size() / 3 - 1; i++)
//		{
//			indices.push_back(i);
//			indices.push_back(i + 1);
//			indices.push_back(i + 2);
//		}
//
//	RefreshMesh();
//}

void RenderTree(int numOfCylinders)
{
	GLUquadricObj* trunk = gluNewQuadric();
	vector<GLUquadricObj*> leaves;
	for (int i = 0; i < numOfCylinders; i++)
		leaves.push_back(gluNewQuadric());

	glColor3ub(128, 77, 0);
	gluCylinder(trunk, 0.04f, 0.04f, 0.3f, 15, 10);
	for (int i = 0; i < numOfCylinders; i++)
	{
		glTranslatef(0.f, 0.0f, 0.1f);
		if(i == numOfCylinders - 1)
			glColor3ub(244, 255, 232);
		else
			glColor3ub(126, 250, 107);
		gluCylinder(leaves[i], 0.02f + 0.03f * (numOfCylinders - i), 0.f, 0.3f, 15, 10);
	}
	glTranslatef(0.f, 0.0f, -0.1f * numOfCylinders);
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

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	SetTwoSheetedHyperboloidVertices();

	Shader shader("./src/shaders/default.shader");


	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	float r = 0.f;
	float colorChangeSpeed = 0.f;

	glm::mat4 figureModelMatrix = glm::mat4(1.0f);
	bool rotateFigure = false;
	float figureScale = 1.f;
	float lastFigureScale = 1.f;

	int polygonMode = GL_POINT;
	Projection cameraProjection = Projection::Perspective;


	bool isHoveringOverFiguresGui = false;
	bool isHoveringOverEditGui = false;


	bool renderTree = false;
	// To rotate the tree
	glRotatef(-90.f, 1.f, 0.f, 0.f);

	while (!glfwWindowShouldClose(window))
	{
		Time::SetNow();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
		
		if(rotateFigure)
			figureModelMatrix = glm::rotate(figureModelMatrix, 1.4f * Time::GetDeltaTime(), glm::vec3(0.0f, 1.0f, 0.f));
		
			figureModelMatrix = glm::scale(figureModelMatrix, glm::vec3(figureScale - lastFigureScale + 1, figureScale - lastFigureScale + 1, figureScale - lastFigureScale + 1));
			if (lastFigureScale != figureScale)
				lastFigureScale = figureScale;

		if(renderTree)
			RenderTree(5);

		shader.Bind();
		VAO1->Bind();


		// Set color
		r += colorChangeSpeed * Time::GetDeltaTime();
		if (r >= 1.f)
		{
			colorChangeSpeed *= -1;
			r = 0.99f;
		}
		else if (r <= 0)
		{
			colorChangeSpeed *= -1;
			r = 0.01f;
		}

		camera.HandleInputs(window, isHoveringOverEditGui || isHoveringOverFiguresGui);
		camera.UpdateMatrix(90.0f, 0.1f, 1000.0f, shader, "camMatrix", cameraProjection);
		shader.SetUniformMat4f("model", figureModelMatrix);
		shader.SetUniform3f("color", r, 1.f - r, 1.f);

		if(!renderTree)
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		shader.Unbind();

#pragma region GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Figures");
		isHoveringOverFiguresGui = ImGui::IsWindowFocused();
		if (ImGui::Button("Right Triangle"))
			SetRightTriangleVerticies();
		else if (ImGui::Button("Rectangle"))
			SetRectangleVertices();
		else if (ImGui::Button("Rhombus"))
			SetRhombusVertices();
		else if (ImGui::Button("Hyperboloid"))
			SetTwoSheetedHyperboloidVertices();
		ImGui::Text("Tree");
		if (ImGui::Button("Render Tree"))
			renderTree = true;
		else if (ImGui::Button("Disable Tree Render"))
			renderTree = false;
		ImGui::End();

		ImGui::Begin("Edit");
		isHoveringOverEditGui = ImGui::IsWindowFocused();
		if (ImGui::Button("Switch Rotation"))
			rotateFigure = !rotateFigure;
		else if (ImGui::Button("Switch Colors"))
			colorChangeSpeed = colorChangeSpeed == 0 ? 0.5f : 0;
		else if (ImGui::Button("Set Wireframe"))
			polygonMode = GL_LINE;
		else if (ImGui::Button("Set Filled"))
			polygonMode = GL_FILL;
		ImGui::Text("Camera Projection");
		if (ImGui::Button("Perspective"))
			cameraProjection = Projection::Perspective;
		else if (ImGui::Button("Ortho"))
			cameraProjection = Projection::Ortho;
		ImGui::SliderFloat("Scale", &figureScale, 0.5f, 2.f);
		ImGui::End();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

		

		glfwSwapBuffers(window);
		glfwPollEvents();
		Time::Calculate();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void RecalculateIndicies(bool startZero)
{
	indices.clear();
	for (int i = 0; i < vertices.size() / 3 - 1; i++)
	{
		indices.push_back(startZero ? 0 : i);
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