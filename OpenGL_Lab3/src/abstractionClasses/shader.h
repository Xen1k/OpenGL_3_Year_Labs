#pragma once
#include <string>

#include "glm/glm.hpp"

struct ShaderProgamSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
public:
	unsigned int ID;
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	unsigned int CompileShader(unsigned int type, const std::string source);
	unsigned int CreateShader(const std::string vertexShader, const std::string fragmentShader);
	ShaderProgamSource ParseShader(const std::string filepath);
	int GetUniformLocation(const std::string& name);
};

