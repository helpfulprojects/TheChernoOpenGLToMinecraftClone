#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
		float vertices[] = {
			-0.5,-0.5,
			0.5,-0.5,
			0.5,0.5,
			-0.5,0.5,
		};
		VertexArray vao;
		VertexBuffer vbo(vertices, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		vao.AddBuffer(vbo,layout);

		unsigned int indices[] = {
			0, 1 , 2,
			0, 2 , 3,
		};
		IndexBuffer ibo(indices, 6);
		vao.Unbind();
		Shader shaderProgram("res/shaders/Basic.shader");
		shaderProgram.Bind();
		shaderProgram.Uniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);

		vao.Unbind();
		shaderProgram.Unbind();
		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GlCall(glClear(GL_COLOR_BUFFER_BIT));

			shaderProgram.Bind();
			shaderProgram.Uniform4f("u_Color", r, 0.0f, 0.0f, 1.0f);
			vao.Bind();
			GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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
