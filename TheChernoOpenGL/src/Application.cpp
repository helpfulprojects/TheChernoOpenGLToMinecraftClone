#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "ErrorManager.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "The Cherno OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK)
		std::cout << "GLEW Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		GlCall(glEnable(GL_BLEND));
		GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		Renderer renderer;
		float vertices[] = {
			100.0,100.0,0.0,0.0,
			200.0,100.0,1.0,0.0,
			200.0,200.0,1.0,1.0,
			100.0,200.0,0.0,1.0
		};
		VertexArray va;
		VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb,layout);

		unsigned int indices[] = {
			0, 1 , 2,
			0, 2 , 3,
		};
		IndexBuffer ib(indices, 6);
		va.Unbind();

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.Uniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
		
		Texture texture("res/textures/thecherno.png");
		texture.Bind();
		shader.Uniform1i("u_Texture", 0);

		glm::mat4 proj = glm::ortho(0.0, 960.0, 0.0, 540.0,-1.0,1.0);
		glm::mat4 view = glm::translate(glm::mat4(1.0), {-100.0,0.0,0.0});
		glm::mat4 model = glm::translate(glm::mat4(1.0), {0.0,100.0,0.0});
		glm::mat4 mvp = proj * view * model;
		shader.UniformMatrix4fv("u_MVP", mvp);

		va.Unbind();
		shader.Unbind();
		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.Uniform4f("u_Color", r, 0.0f, 0.0f, 1.0f);

			renderer.Draw(va, ib, shader);

			if (r < 0.0f)
				increment = 0.05f;
			else if (r > 1.0f)
				increment = -0.05f;

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}
