#include "Shader.h"

#include <iostream>
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <string>


Shader::Shader(const std::string& filepath) : m_FilePath(filepath), ID(0)
{
    ShaderProgamSource source = ParseShader(filepath);
    ID = CreateShader(source.vertexSource, source.fragmentSource);
}


/// <summary>
/// Разделяет один файл с двумя шейдерами (vertex, fragment)
/// </summary>
ShaderProgamSource Shader::ParseShader(const std::string filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CreateShader(const std::string vertexShader, const std::string fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);


    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
        std::cout << "SHADER ERROR" << std::endl;

    return id;
}

void Shader::Bind() const { glUseProgram(ID); }

void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform4f(const std::string& name, glm::vec4 vec)
{
    glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 vec)
{
    glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}




int Shader::GetUniformLocation(const std::string& name)
{
    int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1)
        std::cout << "UNIFORM " << name << " DOES NOT EXIST!";
    return location;
}


Shader::~Shader()
{
    glDeleteProgram(ID);
}
