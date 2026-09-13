#include<iostream>
//Práctica 4
// León Ruiz Eduardo
// fecha de entrega: 12 de septiembre de 2026
// No. Cuenta: 421025550
//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
//Var de tipo flotante, manipular la vista para manipular el entorno, sin entrar ni salir del programa
float movX=0.0f;
float movY=0.0f;
float movZ=-3.2f;
float rot = -30.0f;
int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4 - Eduardo León", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificaci�n de errores de creacion  ventana
	if (nullptr == window)
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


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	float vertices[] = {
		// Posición            // Color base (Blanco)
		-0.1f, -0.1f,  0.1f,   1.0f, 1.0f, 1.0f, // Frente
		 0.1f, -0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f,  0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f,  0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f,  0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f, -0.1f,  0.1f,   1.0f, 1.0f, 1.0f,

		-0.1f, -0.1f, -0.1f,   1.0f, 1.0f, 1.0f, // Atrás
		 0.1f, -0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f,  0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f,  0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f,  0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f, -0.1f, -0.1f,   1.0f, 1.0f, 1.0f,

		 0.1f, -0.1f,  0.1f,   1.0f, 1.0f, 1.0f, // Derecha
		 0.1f, -0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f,  0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f,  0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f,  0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f, -0.1f,  0.1f,   1.0f, 1.0f, 1.0f,

		-0.1f,  0.1f,  0.1f,   1.0f, 1.0f, 1.0f, // Izquierda
		-0.1f,  0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f, -0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f, -0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f, -0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f,  0.1f,  0.1f,   1.0f, 1.0f, 1.0f,

		-0.1f, -0.1f, -0.1f,   1.0f, 1.0f, 1.0f, // Abajo
		 0.1f, -0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f, -0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f, -0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f, -0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f, -0.1f, -0.1f,   1.0f, 1.0f, 1.0f,

		-0.1f,  0.1f, -0.1f,   1.0f, 1.0f, 1.0f, // Arriba
		 0.1f,  0.1f, -0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f,  0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		 0.1f,  0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f,  0.1f,  0.1f,   1.0f, 1.0f, 1.0f,
		-0.1f,  0.1f, -0.1f,   1.0f, 1.0f, 1.0f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

	//projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view = glm::mat4(1);

		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint colorLoc = glGetUniformLocation(ourShader.Program, "objectColor");

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAO);

		// Definición de Colores de la referencia
		glm::vec3 cAzul = glm::vec3(0.12f, 0.65f, 0.95f);
		glm::vec3 cCrema = glm::vec3(0.96f, 0.82f, 0.72f);
		glm::vec3 cNegro = glm::vec3(0.05f, 0.05f, 0.05f);
		glm::vec3 cBlanco = glm::vec3(0.92f, 0.92f, 0.92f);
		glm::vec3 cCafe = glm::vec3(0.48f, 0.28f, 0.15f);

		// --- 1. PATAS (AZUL) ---
		// Pata Izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.25f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.5f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (colorLoc != -1) glUniform3f(colorLoc, cAzul.r, cAzul.g, cAzul.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata Derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.25f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.5f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (colorLoc != -1) glUniform3f(colorLoc, cAzul.r, cAzul.g, cAzul.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// --- 2. PECHO / TORSO (CREMA) ---
		// Bloque único sólido para el abdomen
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.05f, 0.02f));
		model = glm::scale(model, glm::vec3(3.2f, 2.0f, 1.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (colorLoc != -1) glUniform3f(colorLoc, cCrema.r, cCrema.g, cCrema.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// --- 3. BRAZOS (AZUL) ---
		// Brazo Izquierdo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.42f, -0.05f, 0.05f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (colorLoc != -1) glUniform3f(colorLoc, cAzul.r, cAzul.g, cAzul.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Brazo Derecho
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.42f, -0.05f, 0.05f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (colorLoc != -1) glUniform3f(colorLoc, cAzul.r, cAzul.g, cAzul.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// --- 4. CAPARAZÓN ESPALDA ---
		// Borde Blanco
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.02f, -0.18f));
		model = glm::scale(model, glm::vec3(3.4f, 2.2f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (colorLoc != -1) glUniform3f(colorLoc, cBlanco.r, cBlanco.g, cBlanco.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Centro Café
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.02f, -0.28f));
		model = glm::scale(model, glm::vec3(2.8f, 1.8f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (colorLoc != -1) glUniform3f(colorLoc, cCafe.r, cCafe.g, cCafe.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// --- 5. CABEZA (AZUL) ---
		// Bloque principal de la cabeza
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.42f, 0.0f));
		model = glm::scale(model, glm::vec3(3.4f, 2.8f, 2.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (colorLoc != -1) glUniform3f(colorLoc, cAzul.r, cAzul.g, cAzul.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// --- 6. OJOS (NEGRO) ---
		// Ojo Izquierdo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.21f, 0.42f, 0.25f));
		model = glm::scale(model, glm::vec3(0.9f, 1.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (colorLoc != -1) glUniform3f(colorLoc, cNegro.r, cNegro.g, cNegro.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Ojo Derecho
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.21f, 0.42f, 0.25f));
		model = glm::scale(model, glm::vec3(0.9f, 1.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (colorLoc != -1) glUniform3f(colorLoc, cNegro.r, cNegro.g, cNegro.b);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.008f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.008f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		 movY += 0.008f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		 movY -= 0.008f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.008f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.008f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.04f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.04f;
 }


