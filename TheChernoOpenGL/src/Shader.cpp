#include "Shader.h"
#include "ErrorManager.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream shaderFile(filepath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(shaderFile, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, std::string& shaderSource) {
	unsigned int id = glCreateShader(type);
	const char* source = shaderSource.c_str();
	GlCall(glShaderSource(id, 1, &source, nullptr));
	GlCall(glCompileShader(id));
	int status;
	GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &status));
	if (status == GL_FALSE) {
		int length;
		GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* msg = (char*)alloca(length * sizeof(char));
		GlCall(glGetShaderInfoLog(id, length, nullptr, msg));
		std::cout << "Error in " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << msg << std::endl;
		GlCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource) {
	unsigned int id = glCreateProgram();
	unsigned int vertexId = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentId = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	GlCall(glAttachShader(id, vertexId));
	GlCall(glAttachShader(id, fragmentId));
	GlCall(glLinkProgram(id));
	int status;
	GlCall(glGetProgramiv(id, GL_LINK_STATUS, &status));
	if (status == GL_FALSE) {
		std::cout << "LINKING ERROR" << std::endl;
		char msg[512];
		glGetProgramInfoLog(id, 512, nullptr, msg);
		std::cout << msg << std::endl;
	}
	GlCall(glValidateProgram(id));
	GlCall(glDeleteShader(vertexId));
	GlCall(glDeleteShader(fragmentId));
	return id;
}

int Shader::GetUniformLocation(const std::string& uniformName)
{
	if (m_UniformLocations.find(uniformName) != m_UniformLocations.end())
		return m_UniformLocations[uniformName];

	GlCall(int location = glGetUniformLocation(m_RendererID, uniformName.c_str()));
	if (location == -1)
		std::cout << "Couldn't find uniform location for: " << uniformName << std::endl;
	m_UniformLocations[uniformName] = location;
	return location;
}

Shader::Shader(const std::string& filepath): m_Filepath(filepath), m_RendererID(0)
{
	ShaderProgramSource shaderSource = ParseShader(filepath);
	m_RendererID = CreateShaderProgram(shaderSource.vertexShaderSource, shaderSource.fragmentShaderSource);
}

Shader::~Shader()
{
	GlCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GlCall(glUseProgram(0));
}

void Shader::Uniform1i(const std::string& uniformName, int value)
{
	GlCall(glUniform1i(GetUniformLocation(uniformName), value));
}

void Shader::Uniform1f(const std::string& uniformName, float value)
{
	GlCall(glUniform1f(GetUniformLocation(uniformName), value));
}

void Shader::Uniform4f(const std::string& uniformName, float v1, float v2, float v3, float v4)
{
	GlCall(glUniform4f(GetUniformLocation(uniformName), v1, v2, v3, v4));
}
