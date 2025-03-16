#pragma once
#include <string>
#include <unordered_map>

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

	void Uniform4f(const std::string& uniformName, float v1, float v2, float v3, float v4);
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
	std::unordered_map<std::string, int> m_UniformLocations;

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, std::string& shaderSource);
	unsigned int CreateShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource);

	int GetUniformLocation(const std::string& uniformName);
};
