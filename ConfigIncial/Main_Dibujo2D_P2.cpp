#include<iostream>

//#define GLEW_STATIC
//	Practica 2
//	Grupo: 14 de lab de compu grafica.
//	fecha de entrega: semana del 31 de agosto al 6 de septiembre, soy asdri
//	Nombre: León Ruiz Eduardo
//	No. de cuenta: 421025550
#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

//Cambiamos la ventana a cuadrada para quitar el espacio entre pixeles
const GLint WIDTH = 1000, HEIGHT = 1000;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ELR --> Practica 2", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);

	//Verificaci�n de errores de creacion  ventana
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
  
	float vertices[] = {
		-0.8f,  0.8f, 0.0f,    0.4f,0.4f,0.4f,  //0--4
		-0.7f, 0.8f, 0.0f,    0.4f,0.4f,0.4f, 
		-0.6f, 0.8f, 0.0f,  0.4f,0.4f,0.4f,  
		-0.5f,  0.8f, 0.0f,   0.4f,0.4f,0.4f,  
		-0.4f,  0.8f, 0.0f,   0.4f,0.4f,0.4f,

		-0.8f,  0.7f, 0.0f,   0.4f,0.4f,0.4f,//5-9
		-0.7f,  0.7f, 0.0f,   0.15f,0.15f,0.15f,
		-0.6f, 0.7f, 0.0f,  0.4f,0.4f,0.4f,
		-0.5f,  0.7f, 0.0f,   0.15f,0.15f,0.15f,
		-0.4f,  0.7f, 0.0f,   0.4f,0.4f,0.4f,

		-0.8f,  0.6f, 0.0f,    0.4f,0.4f,0.4f,  //10-14
		-0.7f, 0.6f, 0.0f,    0.4f,0.4f,0.4f,
		-0.6f, 0.6f, 0.0f,  0.4f,0.4f,0.4f,
		-0.5f,  0.6f, 0.0f,   0.4f,0.4f,0.4f,
		-0.4f,  0.6f, 0.0f,   0.4f,0.4f,0.4f,

		-0.9f,  0.7f, 0.0f,    1.0f,0.76f,0.8f,
		-0.9f,  0.6f, 0.0f,    1.0f,0.0f,0.5f, //15-16
		-0.3f,  0.7f, 0.0f,    1.0f,0.76f,0.8f,
		-0.3f,  0.6f, 0.0f,    1.0f,0.0f,0.5f, //15-18
		-0.7f,  0.5f, 0.0f,   0.4f,0.4f,0.4f, //15-19

		-0.6f,  0.5f, 0.0f,   1.0f,0.0f,0.5f, 
		-0.5f,  0.5f, 0.0f,   0.4f,0.4f,0.4f,
		-0.4f,  0.5f, 0.0f,   0.3f,0.3f,0.3f, //20-22

		-0.7f,  0.4f, 0.0f,   0.4f,0.4f,0.4f,
		-0.4f,  0.4f, 0.0f,   0.4f,0.4f,0.4f,
		-0.4f,  0.1f, 0.0f,   0.4f,0.4f,0.4f,
		-0.7f,  0.1f, 0.0f,   0.4f,0.4f,0.4f,//23-26

		-0.7f,  0.0f, 0.0f,   1.0f,0.76f,0.8f,
		-0.6f,  0.0f, 0.0f,   0.3f,0.3f,0.3f,
		-0.5f,  0.0f, 0.0f,   1.0f,0.76f,0.8f,
		-0.4f,  0.0f, 0.0f,   0.3f,0.3f,0.3f,
		-0.3f,  0.0f, 0.0f,   0.3f,0.3f,0.3f, //27-31

		-0.375f,  0.375f, 0.0f,   0.4f,0.4f,0.4f,
		-0.375f,  0.1f, 0.0f,   0.4f,0.4f,0.4f,
		-0.250f,  0.1f, 0.0f,   0.4f,0.4f,0.4f, //32-34
		
		-0.8f,  -0.1f, 0.0f,    0.4f,0.4f,0.4f,
		-0.2f,  -0.1f, 0.0f,    0.3f,0.3f,0.3f,
		-0.2f,  -0.3f, 0.0f,    0.4f,0.4f,0.4f,
		-0.8f,  -0.3f, 0.0f,    0.3f,0.3f,0.3f, //35-38

		-0.9f,  -0.4f, 0.0f,    1.0f,0.76f,0.8f,
		-0.8f,  -0.4f, 0.0f,    1.0f,0.76f,0.8f,
		-0.7f,  -0.4f, 0.0f,    0.4f,0.4f,0.4f,
		-0.6f,  -0.4f, 0.0f,    0.4f,0.4f,0.4f,
		-0.5f,  -0.4f, 0.0f,    1.0f,0.76f,0.8f,
		-0.4f,  -0.4f, 0.0f,    1.0f,0.76f,0.8f,
		-0.3f,  -0.4f, 0.0f,    0.4f,0.4f,0.4f,
		-0.2f,  -0.4f, 0.0f,    0.4f,0.4f,0.4f,
		-0.1f,  -0.4f, 0.0f,    0.7f,0.0f,0.3f,
		0.0f,  -0.4f, 0.0f,    0.7f,0.0f,0.3f, //39-48

		0.1f,  -0.5f, 0.0f,    0.7f,0.0f,0.3f,
		0.2f,  -0.6f, 0.0f,    0.7f,0.0f,0.3f,
		0.2f,  -0.7f, 0.0f,    0.7f,0.0f,0.3f,
		0.1f,  -0.8f, 0.0f,    0.7f,0.0f,0.3f,
		0.0f,  -0.8f, 0.0f,    0.7f,0.0f,0.3f,
		-0.1f,  -0.8f, 0.0f,    0.7f,0.0f,0.3f,
		-0.2f,  -0.8f, 0.0f,    0.7f,0.0f,0.3f,
		-0.3f,  -0.9f, 0.0f,    0.7f,0.0f,0.3f, //49-56
	};
	unsigned int indices[] = {  // note that we start from 0!
		0,1,2,3,4,
		5,6,7,8,9,
		10,11,12,13,14,
		15,16,17,18,19,
		20,21,22,
		23,24,25,26,
		27,28,29,30,31,
		32,33,34,
		35,36,37,38,
		39,40,41,42,43,44,45,46,47,48,
		49,50,51,52,53,54,55,56,
	};



	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)



	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glBindVertexArray(VAO);

		//Vertices
		glPointSize(50);
		glDrawArrays(GL_POINTS, 0, 5);
		glDrawArrays(GL_POINTS, 5, 5);
		glDrawArrays(GL_POINTS, 10, 5);
		glDrawArrays(GL_POINTS, 15, 5);
		glDrawArrays(GL_POINTS, 20, 3);
		glDrawArrays(GL_TRIANGLE_FAN, 23, 4);
		glDrawArrays(GL_POINTS, 27, 5);
		glDrawArrays(GL_TRIANGLES, 32, 3);
		glDrawArrays(GL_TRIANGLE_FAN, 35, 4);
		glDrawArrays(GL_POINTS, 39, 10);
		glDrawArrays(GL_POINTS, 49, 8);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}