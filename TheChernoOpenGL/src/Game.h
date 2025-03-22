#pragma once
#include <vector>
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "World.h"
#include "ThreadPool.h"
class Game
{
public:
	Renderer* m_Renderer;
	bool m_Keys[1024];
	Game(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT) : m_SCR_WIDTH(SCR_WIDTH), m_SCR_HEIGHT(SCR_HEIGHT){}
	~Game();
	void Init();
	void Update(float deltaTime);
	void Render();
	void ProcessInput(float deltaTime);
	void ProcessMouse(float xpos, float ypos, float xoffset, float yoffset);
private:
	unsigned int vao;
	Shader* m_TerrainShader;
	Shader* m_WaterShader;
	World* m_World;
	ThreadPool* m_ThreadPool;
	Texture* texture;
	Camera* m_Camera;
	glm::vec3 translate;
	glm::mat4 proj;
	glm::mat4 view;
	float m_Xpos, m_Ypos, m_Xoffset, m_Yoffset;
	unsigned int m_SCR_WIDTH, m_SCR_HEIGHT;
};
