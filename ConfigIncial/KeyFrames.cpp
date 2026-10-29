#include <iostream>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
//	Eduardo León Ruiz
//	No. Cuenta --> 421025550
//	Previo 12
// 
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
float rotDog = 0.0f;
int dogAnim = 0;
float head = 0.0f;
float tail = 0.0f;

// Partes del perro por separado (en grados). Cada pata gira sobre el eje X de su articulacion:
//   positivo = la pata se va hacia atras ; negativo = hacia adelante y arriba
float FLegL = 0.0f;      // pata delantera izquierda
float FLegR = 0.0f;      // pata delantera derecha  (la que saluda)
float BLegL = 0.0f;      // pata trasera izquierda
float BLegR = 0.0f;      // pata trasera derecha
float bodyPitch = 0.0f;  // inclinacion del cuerpo: negativo = levanta el frente (para sentarse)

//KeyFrames
float dogPosX = 0.0f, dogPosY = 0.0f, dogPosZ = 0.0f;

#define MAX_FRAMES 24
#define NUM_PARAMS 11
enum { P_X, P_Y, P_Z, P_ROT, P_HEAD, P_TAIL, P_FLL, P_FLR, P_BLL, P_BLR, P_PITCH };

// Cada parametro animable apunta a su variable viva
float* paramVar[NUM_PARAMS] = { &dogPosX, &dogPosY, &dogPosZ, &rotDog, &head, &tail,
								&FLegL, &FLegR, &BLegL, &BLegR, &bodyPitch };

int i_max_steps = 190;      // pasos de interpolacion por defecto (a 60 FPS ~ 3 s)
int i_curr_steps = 0;
typedef struct _frame {
	float value[NUM_PARAMS];   // pose guardada
	float inc[NUM_PARAMS];     // incremento por paso hacia el siguiente fotograma
	int   steps;               // pasos del tramo que inicia en este fotograma
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);
	for (int p = 0; p < NUM_PARAMS; p++) KeyFrame[FrameIndex].value[p] = *paramVar[p];
	KeyFrame[FrameIndex].steps = i_max_steps;
	FrameIndex++;
}

void resetElements(void)
{
	for (int p = 0; p < NUM_PARAMS; p++) *paramVar[p] = KeyFrame[0].value[p];
}

void interpolation(void)
{
	int n = KeyFrame[playIndex].steps;
	for (int p = 0; p < NUM_PARAMS; p++)
		KeyFrame[playIndex].inc[p] = (KeyFrame[playIndex + 1].value[p] - KeyFrame[playIndex].value[p]) / n;
}

// Escribe en consola los fotogramas grabados, listos para pegar en LoadDefaultAnimation()
void printFrames(void)
{
	printf("\n// ---- pegar dentro de LoadDefaultAnimation() ----\n");
	for (int i = 0; i < FrameIndex; i++)
	{
		const float* v = KeyFrame[i].value;
		printf("setFrame(%d, %.4ff, %.4ff, %.4ff, %.2ff, %.2ff, %.2ff, %.2ff, %.2ff, %.2ff, %.2ff, %.2ff, %d);\n",
			i, v[P_X], v[P_Y], v[P_Z], v[P_ROT], v[P_HEAD], v[P_TAIL], v[P_FLL], v[P_FLR], v[P_BLL], v[P_BLR], v[P_PITCH],
			KeyFrame[i].steps);
	}
	printf("FrameIndex = %d;\n\n", FrameIndex);
}

void setFrame(int i, float x, float y, float z, float rot, float hd, float tl,
	float fl, float fr, float bl, float br, float pitch, int steps)
{
	float v[NUM_PARAMS] = { x, y, z, rot, hd, tl, fl, fr, bl, br, pitch };
	for (int p = 0; p < NUM_PARAMS; p++) { KeyFrame[i].value[p] = v[p]; KeyFrame[i].inc[p] = 0.0f; }
	KeyFrame[i].steps = steps;
}

// ---------------------------------------------------------------
// Geometria del perro: se leen los .obj para apoyarlo bien en el piso
// tanto de pie como sentado (asi no se hunde ni flota al cambiar de pose)
// ---------------------------------------------------------------
struct PartGeo { std::vector<glm::vec3> v; glm::vec3 off; };
PartGeo gBody, gHead, gTail, gFL, gFR, gBL, gBR;
glm::vec3 floorCenter(0.0f);
float floorY = 0.0f, floorSize = 10.0f;
float standY = 0.0f, sitY = 0.0f;

// Pose sentado: el cuerpo se inclina hacia atras, las patas traseras quedan dobladas hacia
// adelante y las delanteras verticales (angulo de pata = angulo deseado en el mundo - inclinacion)
const float sitPitch = -35.0f;
const float sitFront = 35.0f;    // delanteras: -0 (vertical) - (-35)
const float sitBack = -55.0f;    // traseras:  -90 (hacia adelante) - (-35)

static void LoadVerts(const char* path, std::vector<glm::vec3>& out)
{
	std::ifstream file(path);
	if (!file.is_open()) { std::cout << "No se pudo abrir " << path << std::endl; return; }
	std::string line;
	while (std::getline(file, line))
	{
		if (line.size() > 2 && line[0] == 'v' && line[1] == ' ')
		{
			std::istringstream ss(line.substr(2));
			float x, y, z;
			if (ss >> x >> y >> z) out.push_back(glm::vec3(x, y, z));
		}
	}
}

// Altura minima (en Y) de todo el perro para una pose dada, con el cuerpo en el origen
static float LowestY(float pitch, float fl, float fr, float bl, float br)
{
	glm::mat4 P = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	float lo = 1e9f;
	auto test = [&](const PartGeo& g, float ang)
		{
			glm::mat4 M = P * glm::translate(glm::mat4(1.0f), g.off)
				* glm::rotate(glm::mat4(1.0f), glm::radians(ang), glm::vec3(1.0f, 0.0f, 0.0f));
			for (size_t i = 0; i < g.v.size(); i++)
				lo = std::min(lo, (M * glm::vec4(g.v[i], 1.0f)).y);
		};
	test(gBody, 0.0f); test(gHead, 0.0f); test(gTail, 0.0f);
	test(gFL, fl); test(gFR, fr); test(gBL, bl); test(gBR, br);
	return (lo > 1e8f) ? 0.0f : lo;
}

void StandPose(void)
{
	dogPosY = standY; bodyPitch = 0.0f;
	FLegL = FLegR = BLegL = BLegR = 0.0f;
	head = 0.0f; tail = 0.0f;
}

void SitPose(void)
{
	dogPosY = sitY; bodyPitch = sitPitch;
	FLegL = FLegR = sitFront; BLegL = BLegR = sitBack;
	head = 0.0f; tail = 0.0f;
}

// Animacion pregrabada: el perro se sienta y luego estira la pata derecha como saludando
void LoadDefaultAnimation(void)
{
	float x = dogPosX, z = dogPosZ, r = 0.0f;
	//         i    x  y      z  rot head tail  FLL       FLR   BLL      BLR      pitch     pasos
	setFrame(0, x, standY, z, r, 0, 0, 0, 0, 0, 0, 0, 190);  // de pie
	setFrame(1, x, sitY, z, r, 0, 0, sitFront, sitFront, sitBack, sitBack, sitPitch, 60);   // sentado
	setFrame(2, x, sitY, z, r, 0, 0, sitFront, sitFront, sitBack, sitBack, sitPitch, 120);  // pausa breve
	setFrame(3, x, sitY, z, r, 0, 20, sitFront, -75, sitBack, sitBack, sitPitch, 50);   // levanta la pata derecha
	setFrame(4, x, sitY, z, r, 0, -20, sitFront, -55, sitBack, sitBack, sitPitch, 50);   // saluda (abajo)
	setFrame(5, x, sitY, z, r, 0, 20, sitFront, -90, sitBack, sitBack, sitPitch, 50);   // saluda (arriba)
	setFrame(6, x, sitY, z, r, 0, -20, sitFront, -55, sitBack, sitBack, sitPitch, 50);
	setFrame(7, x, sitY, z, r, 0, 20, sitFront, -90, sitBack, sitBack, sitPitch, 120);
	setFrame(8, x, sitY, z, r, 0, 0, sitFront, sitFront, sitBack, sitBack, sitPitch, 0);    // baja la pata
	FrameIndex = 9;
	play = false;
}

// Lee piso y perro: centra el perro en el piso, calcula alturas de pie / sentado y encuadra la camara
void LoadDogGeometry(void)
{
	std::vector<glm::vec3> fv;
	LoadVerts("Models/perro/piso.obj", fv);
	if (!fv.empty())
	{
		glm::vec3 mn = fv[0], mx = fv[0];
		for (size_t i = 0; i < fv.size(); i++) { mn = glm::min(mn, fv[i]); mx = glm::max(mx, fv[i]); }
		floorCenter = (mn + mx) * 0.5f;
		floorY = mx.y;
		floorSize = std::max(mx.x - mn.x, mx.z - mn.z);
	}

	LoadVerts("Models/perro/DogBody.obj", gBody.v);          gBody.off = glm::vec3(0.0f);
	LoadVerts("Models/perro/HeadDog.obj", gHead.v);          gHead.off = glm::vec3(0.0f, 0.093f, 0.208f);
	LoadVerts("Models/perro/TailDog.obj", gTail.v);          gTail.off = glm::vec3(0.0f, 0.026f, -0.288f);
	LoadVerts("Models/perro/F_LeftLegDog.obj", gFL.v);       gFL.off = glm::vec3(0.112f, -0.044f, 0.074f);
	LoadVerts("Models/perro/F_RightLegDog.obj", gFR.v);      gFR.off = glm::vec3(-0.111f, -0.055f, 0.074f);
	LoadVerts("Models/perro/B_LeftLegDog.obj", gBL.v);       gBL.off = glm::vec3(0.082f, -0.046f, -0.218f);
	LoadVerts("Models/perro/B_RightLegDog.obj", gBR.v);      gBR.off = glm::vec3(-0.083f, -0.057f, -0.231f);

	standY = floorY - LowestY(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	sitY = floorY - LowestY(sitPitch, sitFront, sitFront, sitBack, sitBack);

	dogPosX = floorCenter.x;
	dogPosZ = floorCenter.z;
	dogPosY = standY;

	// Largo del perro -> distancia de la camara (de frente, para ver el saludo)
	float dogLen = 0.55f;
	if (!gBody.v.empty())
	{
		float zmin = 1e9f, zmax = -1e9f;
		for (size_t i = 0; i < gBody.v.size(); i++) { zmin = std::min(zmin, gBody.v[i].z); zmax = std::max(zmax, gBody.v[i].z); }
		dogLen = zmax - zmin;
	}
	float camH = 0.6f * dogLen, camD = 2.2f * dogLen, target = 0.25f * dogLen;
	float pitch = -glm::degrees(atan2(camH - target, camD));
	camera = Camera(glm::vec3(dogPosX, standY + camH, dogPosZ + camD), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, pitch);

	std::cout << "Perro: largo = " << dogLen << " | altura de pie Y = " << standY << " | sentado Y = " << sitY << std::endl;
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Previo 12 - Eduardo León", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);   // VSync: la animacion por pasos dura lo mismo en cualquier equipo

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


	// Geometria del perro, camara, animacion pregrabada y pose inicial
	LoadDogGeometry();
	LoadDefaultAnimation();
	StandPose();


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
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
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::rotate(model, glm::radians(bodyPitch), glm::vec3(1.0f, 0.0f, 0.0f));   // inclinacion (sentarse)
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
		model = glm::rotate(model, glm::radians(FLegL), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegR), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(BLegL), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(BLegR), glm::vec3(1.0f, 0.0f, 0.0f));
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
	//Dog Controls
	// --- Partes por separado (60 grados/seg; la altura 0.2 unidades/seg) ---
	float rate = 60.0f * deltaTime;
	if (keys[GLFW_KEY_Q]) FLegL -= rate;   // pata delantera izquierda
	if (keys[GLFW_KEY_E]) FLegL += rate;
	if (keys[GLFW_KEY_Z]) FLegR -= rate;   // pata delantera derecha
	if (keys[GLFW_KEY_C]) FLegR += rate;
	if (keys[GLFW_KEY_R]) BLegL -= rate;   // pata trasera izquierda
	if (keys[GLFW_KEY_F]) BLegL += rate;
	if (keys[GLFW_KEY_V]) BLegR -= rate;   // pata trasera derecha
	if (keys[GLFW_KEY_B]) BLegR += rate;
	if (keys[GLFW_KEY_N]) tail -= rate;    // cola
	if (keys[GLFW_KEY_M]) tail += rate;
	if (keys[GLFW_KEY_I]) bodyPitch -= rate;   // inclinacion del cuerpo
	if (keys[GLFW_KEY_O]) bodyPitch += rate;
	if (keys[GLFW_KEY_8]) dogPosY -= 0.2f * deltaTime;   // altura
	if (keys[GLFW_KEY_9]) dogPosY += 0.2f * deltaTime;

	if (keys[GLFW_KEY_5])
	{

		head -= 0.01f;

	}
	if (keys[GLFW_KEY_4])
	{

		head += 0.01f;

	}
	if (keys[GLFW_KEY_2])
	{

		rotDog += 0.1f;

	}

	if (keys[GLFW_KEY_3])
	{

		rotDog -= 0.1f;

	}

	if (keys[GLFW_KEY_H])
	{
		dogPosZ += 0.001;
	}

	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= 0.001;
	}

	if (keys[GLFW_KEY_G])
	{
		dogPosX -= 0.001;
	}

	if (keys[GLFW_KEY_J])
	{
		dogPosX += 0.001;
	}

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

	// L: reproduce / detiene la animacion
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			//First Interpolation
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
	}

	// K: guarda la pose actual como fotograma
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
		else
		{
			printf("memoria de fotogramas llena (%d)\n", MAX_FRAMES);
		}
	}

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_P) printFrames();                                  // imprime los fotogramas como codigo
		if (key == GLFW_KEY_X) { FrameIndex = 0; play = false; printf("fotogramas borrados\n"); }
		if (key == GLFW_KEY_1) { play = false; LoadDefaultAnimation(); StandPose(); printf("animacion pregrabada cargada\n"); }
		if (key == GLFW_KEY_0) { play = false; StandPose(); }                  // pose de pie
		if (key == GLFW_KEY_7) { play = false; SitPose(); }                    // pose sentado
	}

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


}
void Animation() {

	if (play)
	{
		if (i_curr_steps >= KeyFrame[playIndex].steps) //end of animation between frames?
		{
			// Ajusta exactamente a la pose del fotograma alcanzado (evita acumular error)
			for (int p = 0; p < NUM_PARAMS; p++) *paramVar[p] = KeyFrame[playIndex + 1].value[p];

			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			for (int p = 0; p < NUM_PARAMS; p++) *paramVar[p] += KeyFrame[playIndex].inc[p];

			i_curr_steps++;
		}
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