#pragma once
#include <vector>
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
class Game
{
public:
	Renderer* m_Renderer;
	bool m_Keys[1024];
	~Game();
	void Init();
	void Update(float deltaTime);
	void Render();
	void ProcessInput(float deltaTime);
private:
	unsigned int vao;
	Shader* shader;
	VertexArray* va;
	IndexBuffer* ib;
	Texture* texture;
	glm::vec3 translate;
	glm::mat4 proj;
	glm::mat4 view;
};
