#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource {
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
};

class Shader {
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void Uniform1i(const std::string& uniformName, int value);
	void Uniform1f(const std::string& uniformName, float value);
	void Uniform4f(const std::string& uniformName, float v1, float v2, float v3, float v4);
	void UniformMatrix4fv(const std::string& uniformName, const glm::mat4& matrix);
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
	std::unordered_map<std::string, int> m_UniformLocations;

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, std::string& shaderSource);
	unsigned int CreateShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource);

	int GetUniformLocation(const std::string& uniformName);
};
