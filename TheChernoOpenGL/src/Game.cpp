#include "Game.h"
#include "IndexBuffer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

Game::~Game()
{
	delete shader;
	delete m_Renderer;
	delete va;
	delete ib;
	delete texture;
}
void Game::Init()
{
	m_Renderer = new Renderer();
	float vertices[] = {
		100.0,100.0,0.0,0.0,
		200.0,100.0,1.0,0.0,
		200.0,200.0,1.0,1.0,
		100.0,200.0,0.0,1.0
	};
	va = new VertexArray();
	VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va->AddBuffer(vb, layout);

	unsigned int indices[] = {
		0, 1 , 2,
		0, 2 , 3,
	};
	ib = new IndexBuffer(indices, 6);
	va->Unbind();

	shader = new Shader("res/shaders/Basic.shader");
	shader->Bind();
	texture = new Texture("res/textures/wall.jpg");
	texture->Bind();
	shader->Uniform1i("u_Texture", 0);

	proj = glm::ortho(0.0, 960.0, 0.0, 540.0, -1.0, 1.0);
	view = glm::translate(glm::mat4(1.0), { -100.0,0.0,0.0 });
	translate = { 0.0,0.0,0.0 };

	va->Unbind();
	shader->Unbind();

}
void Game::Update(float deltaTime)
{
}
void Game::Render()
{
	ImGui_ImplGlfwGL3_NewFrame();
	shader->Bind();
	glm::mat4 model = glm::translate(glm::mat4(1.0), translate);
	glm::mat4 mvp = proj * view * model;
	shader->UniformMatrix4fv("u_MVP", mvp);
	m_Renderer->Draw(*va, *ib, *shader);
	{
		ImGui::SliderFloat2("float", &translate.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
void Game::ProcessInput(float deltaTime)
{
}