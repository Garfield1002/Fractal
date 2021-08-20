#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define GLAD_DLL
#include <glad/glad.h>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"

// contants
constexpr int width{ 1024 }, height{ 1024 };
constexpr float movement_speed{ 1.0f };

// global variables
glm::vec2 g_center{ -1.0f, 0.0f };
float g_zoom{ 1.0f };
float g_secondes_per_frame{ 1.0f };


constexpr GLfloat vertices[] = {
	 1.0f,  1.0f, 0.0f,  // top right
	-1.0f,  1.0f, 0.0f,  // top left
	-1.0f, -1.0f, 0.0f,  // bottom left
	 1.0f, -1.0f, 0.0f,  // bottom right
};

constexpr GLuint indices[] = {
	0, 1, 3,
	1, 2, 3
};


void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
	// glfw: set a callback for errors
	glfwSetErrorCallback(error_callback);

	// glfw: initialize and configure
	// -------------------------------------
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);		// anti-aliasing

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(width, height, "Fractal Viewer", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1);

	// glfw window icon
	// ----------------
	GLFWimage images[1];
	images[0].pixels = stbi_load("res/icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	Shader shader("res/vertex.gls", "res/fragment.gls");
	shader.setVec2("viewportDimensions", width, height);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	GLuint VBO, VAO, EBO; // vertex buffer object, vertex array object, element buffer objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer( 	// set the vertex attributes pointers
		0,                  // attribute. Must match the layout in the shader (location = 0).
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// general OpenGL configuration
	// ----------------------------
	glViewport(0, 0, width, height);

	double prev_time_value{ glfwGetTime() };

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		double time_value{ glfwGetTime() };
		g_secondes_per_frame = time_value - prev_time_value;

		// render
		// ------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		shader.setVec2("viewportDimensions", width, height);
		shader.setVec2("center", g_center);
		shader.setFloat("zoom", g_zoom);

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		prev_time_value = time_value;
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	shader.deleteProgram();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_UP:
			g_center += glm::vec2(0, movement_speed / g_zoom * g_secondes_per_frame);
			break;
		case GLFW_KEY_DOWN:
			g_center -= glm::vec2(0, movement_speed / g_zoom * g_secondes_per_frame);
			break;
		case GLFW_KEY_RIGHT:
			g_center += glm::vec2(movement_speed / g_zoom * g_secondes_per_frame, 0);
			break;
		case GLFW_KEY_LEFT:
			g_center -= glm::vec2(movement_speed / g_zoom * g_secondes_per_frame, 0);
			break;
		case GLFW_KEY_W:
			g_zoom += movement_speed * g_zoom * 2 * g_secondes_per_frame;
			break;
		case GLFW_KEY_S:
			g_zoom -= movement_speed * g_zoom * 2 * g_secondes_per_frame;
			break;
		default:
			break;
		}

	}
}