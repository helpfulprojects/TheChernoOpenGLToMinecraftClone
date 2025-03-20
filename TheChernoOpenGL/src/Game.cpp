#include <iostream>
#include "Game.h"
#include "IndexBuffer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

Game::~Game()
{
	delete shader;
	delete m_Renderer;
	delete m_World;
	delete texture;
	delete m_Camera;
}
void Game::Init()
{
	m_Camera = new Camera(glm::vec3(0.0f, 30.0f, 0.0f));
	m_Renderer = new Renderer();
	m_World = new World();
	shader = new Shader("res/shaders/Basic.shader");
	shader->Bind();
	texture = new Texture("res/textures/atlas.png");
	texture->Bind();
	shader->Uniform1i("u_Texture", 0);
	shader->Unbind();
}
void Game::Update(float deltaTime)
{
	std::cout <<"FPS:" << 1 / deltaTime << std::endl;
	m_World->UpdateChunksToRender(m_Camera->Position);
}
void Game::Render()
{
	shader->Bind();
	glm::mat4 proj = glm::perspective(glm::radians(m_Camera->Zoom), (float)m_SCR_WIDTH / (float)m_SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = m_Camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f); 
	glm::mat4 mvp = proj * view * model;
	shader->UniformMatrix4fv("u_MVP", mvp);
	m_World->UpdateVertexArrays();
	m_World->Draw(*m_Renderer);
}
void Game::ProcessInput(float deltaTime)
{

    if (m_Keys[GLFW_KEY_W])
        m_Camera->ProcessKeyboard(FORWARD, deltaTime);
    if (m_Keys[GLFW_KEY_S])
        m_Camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (m_Keys[GLFW_KEY_A])
        m_Camera->ProcessKeyboard(LEFT, deltaTime);
    if (m_Keys[GLFW_KEY_D])
        m_Camera->ProcessKeyboard(RIGHT, deltaTime);
    if (m_Keys[GLFW_KEY_E])
        m_Camera->ProcessKeyboard(UP, deltaTime);
    if (m_Keys[GLFW_KEY_Q])
        m_Camera->ProcessKeyboard(DOWN, deltaTime);
    if (m_Keys[GLFW_KEY_LEFT_SHIFT])
        m_Camera->ProcessKeyboard(SPEED_INCREASE, deltaTime);
}

void Game::ProcessMouse(float xpos, float ypos, float xoffset, float yoffset)
{
	m_Xpos = xpos;
	m_Ypos = ypos;
	m_Xoffset = xoffset;
	m_Yoffset = yoffset;
    m_Camera->ProcessMouseMovement(m_Xoffset, m_Yoffset);
}

