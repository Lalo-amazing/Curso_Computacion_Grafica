#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
// León Ruiz Eduardo
// No. cuenta: 421025550
// Practica 10
// Fecha de entrega 24/10/2026
// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera: vista elevada (como en el video). Posicion, up, yaw, pitch
Camera  camera(glm::vec3(0.0f, 6.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -30.0f);
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// ---------------------------------------------------------------
// Medidas de los modelos (se leen de los .obj para apoyar y centrar todo)
// ---------------------------------------------------------------
struct ObjBounds
{
	glm::vec3 mn = glm::vec3(0.0f);
	glm::vec3 mx = glm::vec3(1.0f);
	glm::vec3 headTop = glm::vec3(0.0f, 1.0f, 0.0f);  // punto mas alto (cabeza) en coordenadas del modelo
	bool ok = false;
};

ObjBounds LoadObjBounds(const char* path)
{
	ObjBounds r;
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << "No se pudo abrir " << path << ", se usan medidas por defecto" << std::endl;
		return r;
	}

	std::vector<glm::vec3> verts;
	std::string line;
	while (std::getline(file, line))
	{
		if (line.size() > 2 && line[0] == 'v' && line[1] == ' ')
		{
			std::istringstream ss(line.substr(2));
			float x, y, z;
			if (ss >> x >> y >> z) verts.push_back(glm::vec3(x, y, z));
		}
	}
	if (verts.empty()) return r;

	r.mn = glm::vec3(1e9f);
	r.mx = glm::vec3(-1e9f);
	for (size_t i = 0; i < verts.size(); i++)
	{
		r.mn = glm::min(r.mn, verts[i]);
		r.mx = glm::max(r.mx, verts[i]);
	}

	// Cabeza = centro (x,z) de los vertices del 10% mas alto del modelo
	float limit = r.mx.y - 0.10f * (r.mx.y - r.mn.y);
	glm::vec3 sum(0.0f);
	int n = 0;
	for (size_t i = 0; i < verts.size(); i++)
	{
		if (verts[i].y >= limit) { sum += verts[i]; n++; }
	}
	r.headTop = glm::vec3(sum.x / n, r.mx.y, sum.z / n);
	r.ok = true;
	return r;
}

// Piso
glm::vec3 floorCenter(0.0f);   // centro del piso
float floorY = 0.0f;           // altura de la superficie del piso
float floorSize = 10.0f;       // lado mayor
float floorMinSide = 10.0f;    // lado menor (limita el radio del circulo)

// Perro
glm::vec3 dogMin(0.0f), dogMax(1.0f), dogHead(0.0f, 1.0f, 0.0f);
// Si la pelota no cae justo sobre la cabeza, mueve este ajuste (coordenadas del modelo del perro)
const glm::vec3 headTweak(0.0f, 0.0f, 0.0f);

// Pelota
glm::vec3 ballCenter(0.0f);    // centro de la pelota en su modelo
float ballR = 0.5f;            // radio
float ballBounceAmp = 1.5f;    // altura maxima del bote sobre la cabeza

void LoadAllBounds()
{
	ObjBounds f = LoadObjBounds("Models/perro/piso.obj");
	floorCenter = (f.mn + f.mx) * 0.5f;
	floorY = f.mx.y;
	floorCenter.y = floorY;
	floorSize = glm::max(f.mx.x - f.mn.x, f.mx.z - f.mn.z);
	floorMinSide = glm::min(f.mx.x - f.mn.x, f.mx.z - f.mn.z);

	ObjBounds d = LoadObjBounds("Models/perro/RedDog.obj");
	dogMin = d.mn;
	dogMax = d.mx;
	dogHead = d.headTop;

	ObjBounds b = LoadObjBounds("Models/perro/ball.obj");
	ballCenter = (b.mn + b.mx) * 0.5f;
	ballR = 0.5f * (b.mx.y - b.mn.y);
	ballBounceAmp = 3.0f * ballR;

	std::cout << "Perro: alto = " << (dogMax.y - dogMin.y) << "  | Pelota: radio = " << ballR
		<< "  | Piso: " << floorSize << " x " << floorMinSide << std::endl;
}

// ---------------------------------------------------------------
// Animación (N inicia, N pausa donde esta, N reanuda)
// ---------------------------------------------------------------
bool  AnimActive = false;

const float orbitPeriod = 8.0f;      // segundos por vuelta
const float orbitFactor = 0.35f;     // radio = 35% del lado menor del piso
float orbitRadius = 0.0f;            // radio actual (0 = centro del piso)
float orbitAngle = 0.0f;             // angulo actual (radianes)
float animTime = 0.0f;

// Perro
float dogBounce = 0.0f;              // pequeño rebote al caminar
const float dogHopHeight = 0.15f;      // altura del salto (fraccion del alto del perro)
const float dogHopPeriod = 0.5f;       // segundos por salto
float dogLook = 0.0f;                // pequeño vaivén del cuerpo al caminar
glm::vec3 dogPos(0.0f);
float dogYaw = 0.0f;                 // giro sobre Y (grados)
float dogTilt = 0.0f;
const float dogYawOffset = 0.0f;     // ajusta si el perro no mira hacia donde camina

// Pelota
float rotBall = 0.0f;
float ballPhase = 0.0f;              // fase del bote (cada 1.0 = un bote completo)
const float ballBouncePeriod = 0.8f; // segundos por bote
glm::vec3 posBall(0.0f);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 10 - Eduardo León", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// Models
	Model Dog((char*)"Models/perro/RedDog.obj");
	Model Piso((char*)"Models/perro/piso.obj");
	Model Ball((char*)"Models/perro/ball.obj");

	// Medidas del piso y camara encuadrada sobre su centro
	LoadAllBounds();
	{
		float h = floorSize * 0.9f;   // altura de la camara
		float d = floorSize * 1.3f;   // distancia hacia atras
		float pitch = -glm::degrees(atan2(h, d));
		camera = Camera(floorCenter + glm::vec3(0.0f, h, d), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, pitch);
	}

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, glm::max(100.0f, floorSize * 20.0f));

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();
		Animation();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		lightingShader.Use();
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		// Material
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Matrices de camara
		glm::mat4 view = camera.GetViewMatrix();
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1.0f);

		// ---------------- Piso ----------------
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		// ---------------- Perro ----------------
		// Trasladar -> girar sobre Y -> inclinar (salto)
		model = glm::mat4(1.0f);
		model = glm::translate(model, dogPos + glm::vec3(0.0f, dogBounce, 0.0f));
		model = glm::rotate(model, glm::radians(dogYaw + dogLook), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dogTilt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Dog.Draw(lightingShader);

		// ---------------- Pelota ----------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, posBall);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, -ballCenter);   // gira sobre su propio centro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);

		glBindVertexArray(0);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// ---------------------------------------------------------------
// Animación: se llama una vez por cuadro
// ---------------------------------------------------------------
void Animation()
{
	float dogH = dogMax.y - dogMin.y;

	// Solo avanza si esta activa; al pausar (N) todo queda congelado donde esta
	if (AnimActive)
	{
		// el radio crece suavemente desde el centro hasta la orbita
		float targetRadius = orbitFactor * floorMinSide;
		orbitRadius += (targetRadius - orbitRadius) * glm::min(1.0f, 1.5f * deltaTime);

		animTime += deltaTime;
		orbitAngle += (2.0f * 3.14159265f / orbitPeriod) * deltaTime;

		// Perro: camino circular, mirando en la direccion de avance
		dogYaw = glm::degrees(atan2(-sin(orbitAngle), cos(orbitAngle))) + dogYawOffset;
		// Saltitos al caminar: media onda |sin| (los pies tocan el piso entre salto y salto)
		dogBounce = dogHopHeight * dogH * fabs(sin(3.14159265f * animTime / dogHopPeriod));
		dogLook = 5.0f * sin(animTime * 3.0f);

		// Pelota: gira y avanza la fase del bote
		rotBall += 90.0f * deltaTime;
		ballPhase += deltaTime / ballBouncePeriod;
		if (ballPhase > 1000.0f) ballPhase -= 1000.0f;
	}

	// Posicion del perro sobre el circulo centrado en el piso.
	// Y = floorY - dogMin.y  ->  los pies (parte mas baja del modelo) quedan sobre el piso
	dogPos = glm::vec3(floorCenter.x + orbitRadius * cos(orbitAngle),
		floorY - dogMin.y,
		floorCenter.z + orbitRadius * sin(orbitAngle));

	// Cabeza del perro en el mundo (rota junto con el perro)
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(dogYaw + dogLook), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 headWorld = dogPos + glm::vec3(0.0f, dogBounce, 0.0f)
		+ glm::vec3(rot * glm::vec4(dogHead + headTweak, 1.0f));

	// Bote en media onda: |sin| -> la pelota toca la cabeza (0) y sube hasta ballBounceAmp
	float bounceH = ballBounceAmp * fabs(sin(3.14159265f * ballPhase));
	posBall = glm::vec3(headWorld.x, headWorld.y + ballR + bounceH, headWorld.z);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	// N -> activa / desactiva la animacion (perro y pelota)
	if (GLFW_KEY_N == key && GLFW_PRESS == action)
	{
		AnimActive = !AnimActive;
	}
}
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}