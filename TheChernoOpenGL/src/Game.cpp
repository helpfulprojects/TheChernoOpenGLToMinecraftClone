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
	delete va;
	delete ib;
	delete texture;
	delete camera;
}
void Game::Init()
{
	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	m_Renderer = new Renderer();
	float vertices[] = {
			 //FRONT
			 0.0, 0.0, 1.0,  Texture::AtlasBlockX(3),					Texture::AtlasBlockY(0),
			 1.0, 0.0, 1.0,  Texture::AtlasBlockX(3)+Texture::m_Offset, Texture::AtlasBlockY(0),
			 1.0, 1.0, 1.0,  Texture::AtlasBlockX(3)+Texture::m_Offset, Texture::AtlasBlockY(0)+Texture::m_Offset,
			 0.0, 1.0, 1.0,  Texture::AtlasBlockX(3),					Texture::AtlasBlockY(0)+Texture::m_Offset,
			 //RIGHT	
			 1.0, 0.0, 1.0,  Texture::AtlasBlockX(3),					Texture::AtlasBlockY(0),
			 1.0, 0.0, 0.0,  Texture::AtlasBlockX(3)+Texture::m_Offset, Texture::AtlasBlockY(0),
			 1.0, 1.0, 0.0,  Texture::AtlasBlockX(3)+Texture::m_Offset, Texture::AtlasBlockY(0) + Texture::m_Offset,
			 1.0, 1.0, 1.0,  Texture::AtlasBlockX(3),					Texture::AtlasBlockY(0) + Texture::m_Offset,
			 //BACK
			 0.0, 0.0, 0.0,  Texture::AtlasBlockX(3)+Texture::m_Offset, Texture::AtlasBlockY(0),
			 1.0, 0.0, 0.0,  Texture::AtlasBlockX(3),					Texture::AtlasBlockY(0),
			 1.0, 1.0, 0.0,  Texture::AtlasBlockX(3),					Texture::AtlasBlockY(0) + Texture::m_Offset,
			 0.0, 1.0, 0.0,  Texture::AtlasBlockX(3)+Texture::m_Offset, Texture::AtlasBlockY(0) + Texture::m_Offset,
			 //LEFT	
			 0.0, 0.0, 1.0,  Texture::AtlasBlockX(3)+Texture::m_Offset, Texture::AtlasBlockY(0),
			 0.0, 0.0, 0.0,  Texture::AtlasBlockX(3),					Texture::AtlasBlockY(0),
			 0.0, 1.0, 0.0,  Texture::AtlasBlockX(3),					Texture::AtlasBlockY(0) + Texture::m_Offset,
			 0.0, 1.0, 1.0,  Texture::AtlasBlockX(3)+Texture::m_Offset, Texture::AtlasBlockY(0) + Texture::m_Offset,
			 //TOP	
			 0.0, 1.0, 1.0,  Texture::AtlasBlockX(0),					Texture::AtlasBlockY(0),
			 1.0, 1.0, 1.0,  Texture::AtlasBlockX(0)+Texture::m_Offset, Texture::AtlasBlockY(0),
			 1.0, 1.0, 0.0,  Texture::AtlasBlockX(0)+Texture::m_Offset, Texture::AtlasBlockY(0) + Texture::m_Offset,
			 0.0, 1.0, 0.0,  Texture::AtlasBlockX(0),					Texture::AtlasBlockY(0) + Texture::m_Offset,
			 //BOTTOM	
			 0.0, 0.0, 1.0,  Texture::AtlasBlockX(2),					Texture::AtlasBlockY(0) + Texture::m_Offset,
			 1.0, 0.0, 1.0,  Texture::AtlasBlockX(2)+Texture::m_Offset, Texture::AtlasBlockY(0) + Texture::m_Offset,
			 1.0, 0.0, 0.0,  Texture::AtlasBlockX(2)+Texture::m_Offset, Texture::AtlasBlockY(0),
			 0.0, 0.0, 0.0,  Texture::AtlasBlockX(2),					Texture::AtlasBlockY(0),
	};
	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	};
	va = new VertexArray();
	VertexBuffer vb(vertices, 6*4*5);
	vb.Bind();

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	va->AddBuffer(vb, layout);

	ib = new IndexBuffer(indices, 6*6);
	ib->Bind();

	shader = new Shader("res/shaders/Basic.shader");
	shader->Bind();
	texture = new Texture("res/textures/atlas.png");
	texture->Bind();
	shader->Uniform1i("u_Texture", 0);

	proj = glm::perspective(glm::radians(45.0f), (float)m_SCR_WIDTH / (float)m_SCR_HEIGHT, 0.1f, 100.0f);
	translate = { 0.0,0.0,0.0 };

	va->Unbind();
	shader->Unbind();

}
void Game::Update(float deltaTime)
{
}
void Game::Render()
{
	shader->Bind();
	glm::mat4 proj = glm::perspective(glm::radians(camera->Zoom), (float)m_SCR_WIDTH / (float)m_SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f); 
	glm::mat4 mvp = proj * view * model;
	shader->UniformMatrix4fv("u_MVP", mvp);
	m_Renderer->Draw(*va, *ib, *shader);
}
void Game::ProcessInput(float deltaTime)
{

    if (m_Keys[GLFW_KEY_W])
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (m_Keys[GLFW_KEY_S])
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (m_Keys[GLFW_KEY_A])
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (m_Keys[GLFW_KEY_D])
        camera->ProcessKeyboard(RIGHT, deltaTime);
    if (m_Keys[GLFW_KEY_SPACE])
        camera->ProcessKeyboard(UP, deltaTime);
    if (m_Keys[GLFW_KEY_LEFT_SHIFT])
        camera->ProcessKeyboard(DOWN, deltaTime);
}

void Game::ProcessMouse(float xpos, float ypos, float xoffset, float yoffset)
{
	m_Xpos = xpos;
	m_Ypos = ypos;
	m_Xoffset = xoffset;
	m_Yoffset = yoffset;
    camera->ProcessMouseMovement(m_Xoffset, m_Yoffset);
}

