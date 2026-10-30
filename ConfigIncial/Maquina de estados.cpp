#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
// Eduardo León Ruiz 
// Practica 11
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

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
//Anim
float rotBall = 0.0f;
bool AnimBall = false;
bool AnimDog = false;
float rotDog = 0.0f;
int dogAnim = 0;               // estado actual de la maquina de estados (ver DogState)
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
glm::vec3 dogPos(0.0f, 0.0f, 0.0f);
float dogRot = 0.0f;
bool step = false;

enum DogState { DOG_IDLE = 0, DOG_WALK = 1, DOG_STOPPING = 2, DOG_DONE = 3, DOG_TURN = 4 };

// Ruta: lista de puntos (en el mundo) que el perro visita en orden
std::vector<glm::vec3> path;
int wpIndex = 1;                     // siguiente punto a visitar (el 0 es el de partida)
float turnTarget = 0.0f;             // rumbo (grados) al que debe girar
int stateAfterTurn = DOG_WALK;       // estado al terminar de girar
const float turnSpeed = 120.0f;      // velocidad de giro (grados/seg)
const int sweeps = 4;                // numero de diagonales de la ruta (mas = mas cerrada)

// Offsets de las partes (los mismos que se usan al dibujar)
const glm::vec3 offHead(0.0f, 0.093f, 0.208f);
const glm::vec3 offTail(0.0f, 0.026f, -0.288f);
const glm::vec3 offFL(0.112f, -0.044f, 0.074f);
const glm::vec3 offFR(-0.111f, -0.055f, 0.074f);
const glm::vec3 offBL(0.082f, -0.046f, -0.218f);
const glm::vec3 offBR(-0.083f, -0.057f, -0.231f);

// Medidas leidas de los .obj
glm::vec3 floorMin(-5.0f), floorMax(5.0f);   // limites del piso
float floorSize = 10.0f, floorMinSide = 10.0f;
float floorY = 0.0f;
float dogFront = 0.3f, dogBack = -0.3f;      // extremos del perro a lo largo de su eje Z
float footY = -0.2f;                         // punto mas bajo de las patas (en coordenadas del cuerpo)
float walkSpeed = 0.25f;                     // unidades por segundo
float stopMargin = 0.5f;                     // distancia de seguridad al borde del piso

struct Bounds { glm::vec3 mn = glm::vec3(0.0f); glm::vec3 mx = glm::vec3(0.0f); };

Bounds ReadBounds(const char* path)
{
	Bounds r;
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << "No se pudo abrir " << path << std::endl;
		return r;
	}
	bool first = true;
	std::string line;
	while (std::getline(file, line))
	{
		if (line.size() > 2 && line[0] == 'v' && line[1] == ' ')
		{
			std::istringstream ss(line.substr(2));
			float x, y, z;
			if (ss >> x >> y >> z)
			{
				glm::vec3 v(x, y, z);
				if (first) { r.mn = r.mx = v; first = false; }
				else { r.mn = glm::min(r.mn, v); r.mx = glm::max(r.mx, v); }
			}
		}
	}
	return r;
}

float dogRadius = 0.3f;                      // radio que ocupa el perro al girar (para no salirse del piso)

// Construye la ruta que recorre todo el piso:
//   1) recto por el borde izquierdo hasta el final del piso
//   2) gira y avanza un paso lateral
//   3) diagonal hacia el borde de partida; gira, paso lateral, diagonal... hasta cubrir todo el piso
//   4) regresa por el borde derecho y por el borde de partida hasta el punto de inicio
void BuildPath()
{
	float r = dogRadius + stopMargin;                     // margen para que ninguna parte salga del piso
	float xL = floorMin.x + r, xR = floorMax.x - r;
	float zB = floorMin.z + r, zF = floorMax.z - r;       // B = lado de partida, F = final del piso
	float y = floorY - footY;                             // las patas apoyan en el piso
	float s = (xR - xL) / (2.0f * sweeps);                // avance lateral por tramo

	path.clear();
	path.push_back(glm::vec3(xL, y, zB));                 // 0: punto de partida
	path.push_back(glm::vec3(xL, y, zF));                 // 1: recto hasta el final del piso

	float x = xL;
	bool atFront = true;
	for (int k = 0; k < sweeps; k++)
	{
		x += s;                                           // gira y avanza (paso lateral)
		path.push_back(glm::vec3(x, y, atFront ? zF : zB));
		x += s;                                           // diagonal hacia el borde opuesto
		atFront = !atFront;
		path.push_back(glm::vec3(x, y, atFront ? zF : zB));
	}

	if (atFront) path.push_back(glm::vec3(xR, y, zB));    // baja por el borde derecho
	path.push_back(glm::vec3(xL, y, zB));                 // regresa al punto de partida
}

// Coloca al perro en el punto de partida y deja todo en reposo
void ResetDog()
{
	if (!path.empty()) dogPos = path[0];
	dogRot = 0.0f;
	FLegs = RLegs = head = tail = 0.0f;
	step = false;
	wpIndex = 1;
	dogAnim = DOG_IDLE;
}

void LoadDogLimits()
{
	Bounds f = ReadBounds("Models/perro/piso.obj");
	floorMin = f.mn;
	floorMax = f.mx;
	floorY = f.mx.y;
	floorSize = glm::max(f.mx.x - f.mn.x, f.mx.z - f.mn.z);
	floorMinSide = glm::min(f.mx.x - f.mn.x, f.mx.z - f.mn.z);

	Bounds body = ReadBounds("Models/perro/DogBody.obj");
	Bounds hd = ReadBounds("Models/perro/HeadDog.obj");
	Bounds tl = ReadBounds("Models/perro/TailDog.obj");
	Bounds fl = ReadBounds("Models/perro/F_LeftLegDog.obj");
	Bounds fr = ReadBounds("Models/perro/F_RightLegDog.obj");
	Bounds bl = ReadBounds("Models/perro/B_LeftLegDog.obj");
	Bounds br = ReadBounds("Models/perro/B_RightLegDog.obj");

	dogFront = glm::max(body.mx.z, offHead.z + hd.mx.z);
	dogBack = glm::min(body.mn.z, offTail.z + tl.mn.z);
	footY = glm::min(glm::min(offFL.y + fl.mn.y, offFR.y + fr.mn.y),
		glm::min(offBL.y + bl.mn.y, offBR.y + br.mn.y));

	float dogLen = dogFront - dogBack;
	float halfWidth = glm::max(glm::abs(offFL.x) + fl.mx.x, glm::abs(offFR.x) + glm::abs(fr.mn.x));
	dogRadius = glm::max(0.5f * dogLen, halfWidth);
	walkSpeed = 0.8f * dogLen;                // largos de perro por segundo
	stopMargin = 0.04f * floorMinSide;

	std::cout << "Piso: " << (f.mx.x - f.mn.x) << " x " << (f.mx.z - f.mn.z)
		<< " | Perro: largo = " << dogLen << std::endl;

	BuildPath();
	float total = 0.0f;
	for (size_t i = 1; i < path.size(); i++) total += glm::length(path[i] - path[i - 1]);
	std::cout << "Ruta: " << path.size() << " puntos, " << total << " unidades (~" << total / walkSpeed << " s)" << std::endl;

	ResetDog();
}


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 11 - Eduardo León", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	//models
	Model DogBody((char*)"Models/perro/DogBody.obj");
	Model HeadDog((char*)"Models/perro/HeadDog.obj");
	Model DogTail((char*)"Models/perro/TailDog.obj");
	Model F_RightLeg((char*)"Models/perro/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/perro/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/perro/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/perro/B_LeftLegDog.obj");
	Model Piso((char*)"Models/perro/piso.obj");
	Model Ball((char*)"Models/perro/ball.obj");

	// Lee las medidas del piso y del perro; coloca al perro al inicio del recorrido
	LoadDogLimits();

	// Camara elevada encuadrando el piso completo
	{
		glm::vec3 c = (floorMin + floorMax) * 0.5f;
		float h = floorSize * 0.9f, d = floorSize * 1.3f;
		float pitch = -glm::degrees(atan2(h, d));
		camera = Camera(glm::vec3(c.x, floorY + h, c.z + d), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, pitch);
	}



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, glm::max(100.0f, floorSize * 20.0f));

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

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


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Body
		modelTemp = model = glm::translate(model, dogPos);
		modelTemp = model = glm::rotate(model, glm::radians(dogRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);
		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
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

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
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

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}

	// N: inicia / pausa / reanuda la animacion (perro y pelota). Si el perro ya termino, reinicia el recorrido.
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		if (dogAnim == DOG_DONE) ResetDog();
		AnimDog = !AnimDog;
		AnimBall = AnimDog;
	}

}

static void Approach(float& v, float target, float maxDelta)
{
	if (v < target) v = glm::min(v + maxDelta, target);
	else            v = glm::max(v - maxDelta, target);
}

// Lleva un angulo al rango (-180, 180]
static float WrapAngle(float a)
{
	while (a > 180.0f)   a -= 360.0f;
	while (a <= -180.0f) a += 360.0f;
	return a;
}

// Balanceo de patas (step indica el sentido); cabeza y cola siguen el ritmo
static void SwingLimbs()
{
	const float legSwing = 20.0f;     // angulo maximo de las patas (grados)
	const float legSpeed = 90.0f;     // velocidad de balanceo (grados/seg)
	float d = step ? 1.0f : -1.0f;
	FLegs += d * legSpeed * deltaTime;
	if (FLegs >= legSwing) { FLegs = legSwing;  step = false; }
	if (FLegs <= -legSwing) { FLegs = -legSwing; step = true; }
	RLegs = FLegs;
	head = 0.5f * FLegs;
	tail = 1.25f * FLegs;
}

// Prepara el giro hacia el siguiente punto de la ruta (o hacia el rumbo inicial si ya no quedan)
static void StartLeg()
{
	if (wpIndex < (int)path.size())
	{
		glm::vec3 d = path[wpIndex] - dogPos;
		turnTarget = glm::degrees(atan2(d.x, d.z));      // 0 = +Z, 90 = +X
		stateAfterTurn = DOG_WALK;
	}
	else
	{
		turnTarget = 0.0f;                               // termina mirando como al inicio
		stateAfterTurn = DOG_STOPPING;
	}
	dogAnim = DOG_TURN;
}

void Animation() {
	// Pelota: giro continuo (por tiempo, no por cuadro)
	if (AnimBall)
	{
		rotBall += 25.0f * deltaTime;
	}

	if (!AnimDog) return;

	const float legSpeed = 90.0f;     // velocidad de balanceo (grados/seg)

	switch (dogAnim)
	{
	case DOG_IDLE:
		StartLeg();
		break;

	case DOG_TURN:
	{
		// Gira por el camino mas corto hacia el rumbo objetivo, moviendo las patas en el lugar
		float diff = WrapAngle(turnTarget - dogRot);
		float maxStep = turnSpeed * deltaTime;
		if (fabs(diff) <= maxStep)
		{
			dogRot = turnTarget;
			dogAnim = stateAfterTurn;
		}
		else
		{
			dogRot += (diff > 0.0f ? maxStep : -maxStep);
			SwingLimbs();
		}
		break;
	}

	case DOG_WALK:
	{
		glm::vec3 d = path[wpIndex] - dogPos;
		d.y = 0.0f;
		float dist = glm::length(d);
		float stepLen = walkSpeed * deltaTime;

		if (dist <= stepLen)
		{
			// Llego al punto: se coloca exacto en el y decide hacia donde girar
			dogPos.x = path[wpIndex].x;
			dogPos.z = path[wpIndex].z;
			wpIndex++;
			StartLeg();
			break;
		}

		SwingLimbs();
		dogPos.x += d.x / dist * stepLen;
		dogPos.z += d.z / dist * stepLen;
		break;
	}

	case DOG_STOPPING:
		// Frena: patas, cabeza y cola regresan suavemente a su posicion neutra
		Approach(FLegs, 0.0f, legSpeed * deltaTime);
		RLegs = FLegs;
		head = 0.5f * FLegs;
		tail = 1.25f * FLegs;
		if (FLegs == 0.0f)
		{
			dogAnim = DOG_DONE;
			AnimDog = false;
			AnimBall = false;
		}
		break;

	case DOG_DONE:
	default:
		break;
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