/*
Shader stuff:

Better to define shaders in files so you don't need to write it as a string
Using GLSLv330 (OpenGL shading language); core means it won't let us use deprecated functions
Colors are floats between 0-1 in graphics programming (RGBA), A = alpha

 For CreateShader function:
 1. Provide OpenGL with both shaders in string form
 2. Compile into one shader
 3. Return buffer index/identifier to be bound and used
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
};

int main(void) {

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Rendering Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {

            //-0.5f, -0.5f, 0.0f, 0.0f, // 0
            // 0.0f,  0.5f, 0.5f, 1.0f, // 2
            // 0.5f, -0.5f, 1.0f, 0.0f  // 3

            //-1.0f, -1.0f,
            // 0.0f,  1.0f,
            // 1.0f, -1.0f

			//-1.0f, -1.0f,
			// 1.0f, -1.0f,
			// 1.0f,  1.0f,
			//-1.0f,  1.0f

			-0.5f, -0.5f,  0.0f,  0.0f, // 1
			 0.5f, -0.5f,  1.0f,  0.0f, // 2
			 0.5f,  0.5f,  1.0f,  1.0f, // 3
			-0.5f,  0.5f,  0.0f,  1.0f  // 4

        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        /* Blending */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

		IndexBuffer ib(indicies, 6);

		Shader shader("res/shaders/Shaders.shader");
		shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/wall.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        // Unbind everything
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.5f;
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