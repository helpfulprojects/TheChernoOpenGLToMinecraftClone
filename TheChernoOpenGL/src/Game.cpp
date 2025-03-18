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
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f 
	};
	va = new VertexArray();
	VertexBuffer vb(vertices, 36*5);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	va->AddBuffer(vb, layout);

	shader = new Shader("res/shaders/Basic.shader");
	shader->Bind();
	texture = new Texture("res/textures/wall.jpg");
	texture->Bind();
	shader->Uniform1i("u_Texture", 0);

	proj = glm::perspective(glm::radians(45.0f), (float)960.0 / (float)540.0, 0.1f, 100.0f);
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
	m_Renderer->Draw(*va, *shader);
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

