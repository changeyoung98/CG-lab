//#define GLEW_STATIC
//#pragma comment(lib,"glew32s.lib")

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif
#include <sys/types.h>
//#include <learnopengl/model.h>
#include "ogldev_engine_common.h"
#include "ogldev_app.h"
#include "ogldev_camera.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include <assimp/texture.h>
#include "skinning_technique.h"
#include "ogldev_glut_backend.h"
#include "ogldev_skinned_mesh.h"
#include "technique.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/camera.h>
#include <learnopengl/shader.h>
#include <learnopengl/model.h>

#include <learnopengl/SnowParticle.h>
#include <learnopengl/FountainParticle.h>
#include <iostream>
#include<time.h>
using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

class Tutorial38 : public ICallbacks, public OgldevApp
{
public:
	SkinnedMesh m_mesh;
	SkinningTechnique * m_pEffect;
	Camera* m_pGameCamera;
	DirectionalLight m_directionalLight;
	//SkinnedMesh m_mesh;
	Vector3f m_position;
	PersProjInfo m_persProjInfo;
	
	
	
	Tutorial38()
	{
		m_pGameCamera = NULL;
		m_pEffect = NULL;
		m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.55f;
		m_directionalLight.DiffuseIntensity = 0.9f;
		m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 100.0f;

		m_position = Vector3f(0.0f, 0.0f, 6.0f);
	}

	~Tutorial38()
	{
		SAFE_DELETE(m_pEffect);
		SAFE_DELETE(m_pGameCamera);
	}

	bool Init()
	{
		Vector3f Pos(0.0f, 3.0f, -1.0f);
		Vector3f Target(0.0f, 0.0f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		m_pEffect = new SkinningTechnique();

		if (!m_pEffect->Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pEffect->Enable();

		m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_pEffect->SetDirectionalLight(m_directionalLight);
		m_pEffect->SetMatSpecularIntensity(0.0f);
		m_pEffect->SetMatSpecularPower(0);

		if (!m_mesh.LoadMesh("D:/VS2017/opengl/CG/res/eagle/model.dae")) {
			printf("Mesh load failed\n");
			return false;
		}

		
		// -----------
		
		//ourModel.LoadMesh("D:/VS2017/opengl/CG/res/WuRenJi_A/WuRenJi_A.obj");


#ifndef WIN32
		if (!m_fontRenderer.InitFontRenderer()) {
			return false;
		}
#endif        	
		return true;
	}

	void Run()
	{
		GLUTBackendRun(this);
	}


	virtual void RenderSceneCB()
	{
		CalcFPS();

		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pEffect->Enable();

		vector<Matrix4f> Transforms;

		float RunningTime = GetRunningTime();

		m_mesh.BoneTransform(RunningTime, Transforms);

		for (uint i = 0; i < Transforms.size(); i++) {
			m_pEffect->SetBoneTransform(i, Transforms[i]);
		}

		m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

		Pipeline p;
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		p.Scale(0.1f, 0.1f, 0.1f);

		Vector3f Pos(m_position);
		p.WorldPos(Pos);
		p.Rotate(270.0f, 180.0f, 0.0f);
		m_pEffect->SetWVP(p.GetWVPTrans());
		m_pEffect->SetWorldMatrix(p.GetWorldTrans());

		m_mesh.Render();
		Shader aniShader(FileSystem::getPath("shaders/skinning.vs").c_str(), FileSystem::getPath("shaders/skinning.fs").c_str());
		aniShader.use();
	//	Model ourModel(FileSystem::getPath("res/WuRenJi_A/WuRenJi_A.obj"));
	//	ourModel.Draw(aniShader);
		RenderFPS();

		glutSwapBuffers();
	}


	virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State)
	{
		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;
		default:
			m_pGameCamera->OnKeyboard(OgldevKey);
		}
	}


	virtual void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}


private:

	//SkinningTechnique * m_pEffect;
	//Camera* m_pGameCamera;
	//DirectionalLight m_directionalLight;
	//SkinnedMesh m_mesh;
	//Vector3f m_position;
	//PersProjInfo m_persProjInfo;
};


int maien(int argc, char** argv)
{
	GLUTBackendInit(argc, argv, true, false);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 38")) {
		return 1;
	}

//	SRANDOM;

	Tutorial38* pApp = new Tutorial38();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}


/*#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/camera.h>
#include <learnopengl/shader.h>
#include <learnopengl/model.h>

#include <learnopengl/SnowParticle.h>
#include <learnopengl/FountainParticle.h>
#include <iostream>
*/


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, int key, int scancode, int action, int mods);
void autoUpdate(GLFWwindow *window);
void cameraMove(GLFWwindow *window);
void rand_move();
void anti_update();
void fkr_update();
void fkr_move();
void avoid();
void renderCube();

unsigned int loadTexture(const char *path);

void renderScene(const Shader &shader,vector<Model> models);

// skybox
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;



// car's position
glm::vec3 car_position = glm::vec3(-3.72f, 1.80f, 6.17f);

glm::vec3 car_forward = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 fkr_forward = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 anti_forward = glm::vec3(0.0f, 0.0f, -1.0f);
float car_forward_speed = 0.1f;
float car_forward_acceleration = -0.01f;
float car_rotation_ratio = 273.1f;
float car_rotation_acceleration = 0.0f;
float car_tilt_ratio = 0.0f;
float car_up_speed = 0.0f;
float car_up_acceleration = 0.0f;

float fkr_forward_speed = 2.0f;
float fkr_forward_acceleration = -0.01f;
float fkr_rotation_ratio = 273.1f;
float fkr_rotation_acceleration = 0.0f;
float fkr_tilt_ratio = 0.0f;
float fkr_up_speed = 0.0f;
float fkr_up_acceleration = 0.0f;

float anti_forward_speed = 1.0f;
float anti_forward_acceleration = -0.01f;
float anti_rotation_ratio = 273.1f;
float anti_rotation_accelaration = 0.0f; 
float anti_tilt_ratio = 0.0f;
float anti_up_speed = 0.0f;
float anti_up_acceleration = 0.0f;

int  fkrtake = 0;

// camera
ACamera camera(glm::vec3(0.0f, 0.5f, 2.3f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
glm::vec3 fkr_position = glm::vec3(30.72f, 1.80f, -30.17f);
glm::vec3 camera_position = glm::vec3(fkr_position.x, fkr_position.y + 20.0f, fkr_position.z + 5.0f);
glm::vec3 anti_position = glm::vec3(car_position.x, car_position.y + 5.0f, car_position.z+5.0f );

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// meshes
unsigned int planeVAO;


// status
static enum STATUS
{
	NONE,           // 0
	MOVE
};
static STATUS status = STATUS::MOVE;

int main(int argc, char** argv)
{

	/*GLUTBackendInit(argc, argv, true, false);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 38")) {
		return 1;
	}

	//	SRANDOM;

	Tutorial38* pApp = new Tutorial38();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;
//	return 0;*/
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, processInput);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------

	glEnable(GL_DEPTH_TEST);
	static const uint MAX_BONES = 100;
	DirectionalLight m_directionalLight;
	m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.55f;
	m_directionalLight.DiffuseIntensity = 0.9f;
	m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);
/*	SkinningTechnique * m_pEffect;
    m_pEffect = new SkinningTechnique();

	if (!m_pEffect->Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pEffect->Enable();

	m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	m_pEffect->SetDirectionalLight(m_directionalLight);
	m_pEffect->SetMatSpecularIntensity(0.0f);
	m_pEffect->SetMatSpecularPower(0);*/
	
	// build and compile shaders
	// -------------------------
	Shader ourShader(FileSystem::getPath("shaders/model_loading.vs").c_str(), FileSystem::getPath("shaders/model_loading.fs").c_str());
	Shader skyboxShader(FileSystem::getPath("shaders/skybox.vs").c_str(), FileSystem::getPath("shaders/skybox.fs").c_str());
	Shader shadowShader(FileSystem::getPath("shaders/shadow_mapping.vs").c_str(), FileSystem::getPath("shaders/shadow_mapping.fs").c_str());
//	Shader aniShader(FileSystem::getPath("shaders/skinning.vs").c_str(), FileSystem::getPath("shaders/skinning.fs").c_str());
//	Shader shadowShader(FileSystem::getPath("shaders/3.1.3.shadow_mapping.vs").c_str(), FileSystem::getPath("shaders/3.1.3.shadow_mapping.fs").c_str());
	Shader simpleDepthShader(FileSystem::getPath("shaders/3.1.3.shadow_mapping_depth.vs").c_str(), FileSystem::getPath("shaders/3.1.3.shadow_mapping_depth.fs").c_str());
	Shader debugDepthQuad(FileSystem::getPath("shaders/3.1.3.debug_quad.vs").c_str(), FileSystem::getPath("shaders/3.1.3.debug_quad_depth.fs").c_str());
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float skyboxVertices[] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//skybox faces
	vector<std::string> faces
	{
		FileSystem::getPath("res/skybox/right.jpg"),
		FileSystem::getPath("res/skybox/left.jpg"),
		FileSystem::getPath("res/skybox/top.jpg"),
		FileSystem::getPath("res/skybox/bottom.jpg"),
		FileSystem::getPath("res/skybox/front.jpg"),
		FileSystem::getPath("res/skybox/back.jpg")
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// plane
	float planeVertices[] = {
		// positions            // normals         // texcoords
		25.0f, 0.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, 0.0f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, 0.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		25.0f, 0.0f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, 0.0f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		25.0f, 0.0f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};
	// plane VAO
	
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	// load textures
	unsigned int Texture = loadTexture(FileSystem::getPath("res/textures/texture.jpg").c_str());

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	shadowShader.use();
    shadowShader.setInt("diffuseTexture", 0);
	shadowShader.setInt("shadowMap", 1);
	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);
	int state = 0;
	
	vector<Model> models;
	// load models
	// -----------
	Model ourModel(FileSystem::getPath("res/WuRenJi_A/WuRenJi_A.obj"));
	models.push_back(ourModel);
	Model target(FileSystem::getPath("res/eagle/model.dae"));
	models.push_back(target);
	Model model_merge(FileSystem::getPath("res/sence_model/model_m1.obj"));
	models.push_back(model_merge);
	//Model model_merge(FileSystem::getPath("res/scene/_______________1_.obj"));
	Model anti(FileSystem::getPath("res/antiFKR/antiFKR.obj"));
	models.push_back(anti);
	//	Model model_m(FileSystem::getPath("res/1/1.obj"));
	Model fence(FileSystem::getPath("res/obstacle/fence.obj"));
	models.push_back(fence);
	// Model myWay("res/test/myway.obj");


	/*SkinnedMesh mesh;
	aniShader.use();
	mesh.LoadMesh("D:/VS2017/opengl/CG/Content/boblampclean.md5mesh");
	GLuint m_boneLocation[MAX_BONES];
	GLuint m_prevBoneLocation[MAX_BONES];*/
	
	// light position
	glm::vec3 lightPos(0.0f, 4.0f, -1.0f);

	// model view projection matries
	glm::mat4 model, view, projection;

	// Particles init
//	SnowParticle::SnowParticle mSnow;
	FountainParticle::FountainParticle mFountain;


	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// camearaMove
		if (status == STATUS::NONE) {
			cameraMove(window);
			// std::cout << "car position : " << car_position.x << ' ' << car_position.y << ' ' << car_position.z << " rotation : " << car_rotation_ratio << std::endl;
		}


		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//cout << lastFrame << endl;
		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// don't forget to enable shader before setting uniforms

	/*	vector<Matrix4f> Transforms;
		//if (state <1000) {
		//aniShader.use();
		mesh.BoneTransform(currentFrame, Transforms);

		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++) {
			char Name[128];
			memset(Name, 0, sizeof(Name));
			SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
			m_boneLocation[i] = glGetUniformLocation(aniShader.program, Name);
			//	cout << m_boneLocation[i] << Name << endl;
			if (m_boneLocation[i] == INVALID_UNIFORM_LOCATION) {
				cout << "Warning! Unable to get the location of uniform '%s'\n" << Name << endl;
			}
			SNPRINTF(Name, sizeof(Name), "gPrevBones[%d]", i);
			m_prevBoneLocation[i] = glGetUniformLocation(aniShader.program, Name);
		}

		for (uint i = 0; i < Transforms.size(); i++) {
			glUniformMatrix4fv(m_boneLocation[i], 1, GL_TRUE, (const GLfloat*)Transforms[i]);
		}
		//		state++;
		//	}
		//state++;
		glm::mat4 model_ani;
	//	model_ani = glm::translate(model_ani, glm::vec3(5.05f, 0.05f, 0.05f));
		model_ani = glm::rotate(model_ani, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model_ani = glm::scale(model_ani, glm::vec3(0.03f, 0.03f, 0.03f));
		aniShader.setMat4("model", model_ani);
		//mesh.Draw(ourShader);
		mesh.Render();*/

		
		//	model_col = glm::translate(model_col, glm::vec3(-30.0f, 0.0f, 30.0f));
		// render the model
	/*	glm::mat4 model_col;

		ourShader.setMat4("model", model_col);
		model_merge.Draw(ourShader);
	
		///////
		
		

	//	vector<Matrix4f> Transforms;
		//if (state <1000) {
			aniShader.use();
			mesh.BoneTransform(currentFrame, Transforms);

			for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++) {
				char Name[128];
				memset(Name, 0, sizeof(Name));
				SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
				m_boneLocation[i] = glGetUniformLocation(aniShader.program, Name);
			//	cout << m_boneLocation[i] << Name << endl;
				if (m_boneLocation[i] == INVALID_UNIFORM_LOCATION) {
					cout << "Warning! Unable to get the location of uniform '%s'\n" << Name << endl;
				}
				SNPRINTF(Name, sizeof(Name), "gPrevBones[%d]", i);
				m_prevBoneLocation[i] = glGetUniformLocation(aniShader.program, Name);
			}

			for (uint i = 0; i < Transforms.size(); i++) {
				glUniformMatrix4fv(m_boneLocation[i], 1, GL_TRUE, (const GLfloat*)Transforms[i]);
			}
	//		state++;
	//	}
		//state++;
	

		

	///////////	// render the man
		//	glm::mat4 model_man;
		//model_man = glm::translate(model_man, glm::vec3(-4.0f, 0.8f, -0.5f));
		///model_man = glm::scale(model_man, glm::vec3(0.05f, 0.05f, 0.05f));
		//ourShader.setMat4("model", model_man);
		//man.Draw(ourShader);*/

		

		


		// render the plane
		
		ourShader.use();
		
		if (status == STATUS::NONE) {
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			view = camera.GetViewMatrix();
		}
		else {
			projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			if (lastFrame > 30 || lastFrame<20) {
				view = glm::lookAt(camera_position, fkr_position, glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else {
				view = glm::lookAt(camera_position, car_position, glm::vec3(0.0f, 1.0f, 0.0f));
			}
			
		}
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		
		if (lastFrame <= 20) {
			rand_move();
		}
		//rand_move();
			autoUpdate(window);
			anti_update();
			fkr_update();
			int dis = glm::distance2(fkr_position, car_position);
			int disAnti = glm::distance2(fkr_position, anti_position);
			
		// render the obj model
		glm::mat4 model;
		model = glm::translate(model, fkr_position);
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		// model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, glm::radians(fkr_rotation_ratio + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-fkr_tilt_ratio), glm::vec3(0.0f, 0.0f, 1.0f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);


		// render the target
		glm::mat4 model_tar;
	
		model_tar = glm::translate(model_tar, car_position);
		model_tar = glm::scale(model_tar, glm::vec3(0.005f, 0.005f, 0.005f));
		model_tar = glm::rotate(model_tar, glm::radians(car_rotation_ratio + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model_tar);
		target.Draw(ourShader);

		// render the model
		glm::mat4 model_col;

		ourShader.setMat4("model", model_col);
		model_merge.Draw(ourShader);

		//anti
		glm::mat4 modela;
		modela = glm::translate(modela, anti_position);
		modela = glm::scale(modela, glm::vec3(0.06f, 0.06f, 0.06f));
		// model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		modela = glm::rotate(modela, glm::radians(car_rotation_ratio + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", modela);
		anti.Draw(ourShader);


		// render the fence
		glm::mat4 model_fence;
		ourShader.setMat4("model", model_fence);
		fence.Draw(ourShader);
		//renderScene(ourShader, models);

		// 
		if (dis == 0) {
			cout << "Target Killed!" << endl;
			Sleep(150);
			break;
		}
		if (disAnti == 0) {
			glm::mat4 model_fountain;
			model_fountain = glm::translate(model_fountain, glm::vec3(fkr_position.x, fkr_position.y, fkr_position.z));
			model_fountain = glm::scale(model_fountain, glm::vec3(0.05, 0.05, 0.05));
			mFountain.Render(deltaTime, model_fountain, view, projection);
			cout << "Avoid Success!" << endl;
			//Sleep(4000);
			//break;
		}
		
		

		// light
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		//ourShader.use();
		//ourShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		//1.
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//	glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		renderScene(simpleDepthShader, models);
		//	renderScene(ourShader, models);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadowShader.use();

		// set light uniforms
		if (status == STATUS::NONE) {
			shadowShader.setVec3("viewPos", camera.Position);
		}
		else {
			shadowShader.setVec3("viewPos", camera_position);
		}
		// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadowShader.setMat4("projection", projection);
		shadowShader.setMat4("view", view);
		shadowShader.setVec3("lightPos", lightPos);
		shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderScene(shadowShader,models);


		// Snow
		//     glm::mat4 model_snow(1.0f);
		    //  glm::mat4 projection_snow = glm::ortho(-15.0, 15.0, 0.0, 15.0, -3.0, 100.0);
		//	  model_snow = glm::translate(model_snow, anti_position);
		//	  model_snow = glm::scale(model_snow, glm::vec3(0.001, 0.001, 0.001));
		//      mSnow.Render(deltaTime, model_snow, view, projection);

		
			 // render Depth map to quad for visual debugging
			 // ---------------------------------------------
	/*	debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", near_plane);
		debugDepthQuad.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//renderQuad();
		*/

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		if (status == STATUS::NONE) {
			view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		}
		else {
			if (lastFrame > 30 || lastFrame<20) {
				view = glm::mat4(glm::mat3(glm::lookAt(camera_position, fkr_position, glm::vec3(0.0f, 1.0f, 0.0f))));
			}
			else {
				view = glm::mat4(glm::mat3(glm::lookAt(camera_position, car_position, glm::vec3(0.0f, 1.0f, 0.0f)))); // remove translation from the view matrix
			}
		}
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default*/


							  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
							  // -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously alloobjed GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void cameraMove(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void autoUpdate(GLFWwindow *window) {
	
	
	car_forward_speed += car_forward_acceleration * deltaTime;	
	if ((car_position.x >= -3.65f) && (car_position.x <= 4.0f) && (car_position.z <= 5.0f) && (car_position.z >= 3.0f)) {
		car_forward_speed = car_forward_speed < 5.0f ? car_forward_speed : 5.0f;
	}
	else {
		car_forward_speed = car_forward_speed < 5.0f ? car_forward_speed : 5.0f;
		car_forward_speed = car_forward_speed > 0.0f ? car_forward_speed : 0.0f;
	}
	
	car_rotation_ratio += car_rotation_acceleration * deltaTime;
	if (car_rotation_acceleration!=0.0f && abs(car_tilt_ratio) <= 20.0f) {
		car_tilt_ratio += car_rotation_acceleration*0.3f * deltaTime;
	}
	else if (car_rotation_acceleration== 0.0f && car_tilt_ratio != 0.0f) {
		float new_car_tilt_ratio = car_tilt_ratio- car_tilt_ratio * 0.4f * deltaTime;
		car_tilt_ratio = (new_car_tilt_ratio * car_tilt_ratio) < 0 ? 0.0f : new_car_tilt_ratio;
	}
	car_up_speed += car_up_acceleration * deltaTime;
	car_up_speed = car_up_speed > 1.0f ? 1.0f : car_up_speed;
	car_up_speed = car_up_speed < -1.0f ? -1.0f : car_up_speed;
	if (car_rotation_ratio != 0.0f) {
		car_forward.z = (-1)*sin(glm::radians(car_rotation_ratio + 90.0f));
		car_forward.x = cos(glm::radians(car_rotation_ratio + 90.0f));
	}
	int dis = glm::distance2(fkr_position, car_position);
	if (dis == 0) {
		car_forward_speed = 0;
	//	cout << "Target Killed~" << endl;
	}
	car_position.z += car_forward.z*car_forward_speed*deltaTime;
	car_position.x += car_forward.x*car_forward_speed*deltaTime;
	car_position.y += car_up_speed * deltaTime;

	/*if (car_forward_speed == 0.0f) {
		fkr_position.y = 2 + 0.05*sin(glm::radians(50*pi*lastFrame));
		anti_rotation_ratio += 0.2f;

	}*/
	if (car_position.y < 0.15) {
		car_position.y = 0.15;
	}
//	car_position.x += sin(glm::radians(lastFrame));
	if (20<=lastFrame <= 30) {
		/*camera_position.x = anti_position.x - anti_forward.x;
		camera_position.z = anti_position.z - anti_forward.z;
		camera_position.y = anti_position.y + 0.4f;*/
		camera_position.x = car_position.x - car_forward.x;
		camera_position.z = car_position.z - car_forward.z;
		camera_position.y = car_position.y + 0.4f;
	}
	

	/*if ((car_position.x >= 1.3f) && (car_position.x <= 3.0f) && (car_position.z <= 5.0f) && (car_position.z >= 3.0f)) {
		car_forward_speed = 0.0f;
		cout << "Target killed!" << endl;
		glfwSetWindowShouldClose(window, true);
	}*/
	
	//	cout << car_position.x <<" "<< car_position.y <<" "<< car_position.z<< '\n';
}

void anti_update() {

	//anti_forward_speed += anti_forward_acceleration * deltaTime;
	if (car_forward_speed==0.0f) {
		anti_rotation_ratio += 0.2f;
	}
	int dis = glm::distance2(fkr_position, anti_position);
	int disT = glm::distance2(fkr_position, car_position);
	//cout << "avoid:"<< dis << endl;
	if ((dis < 30) && (disT <50)) {
		anti_forward = glm::normalize(fkr_position - anti_position);
		anti_forward_speed = 1.0f;
		anti_position.z += anti_forward.z*anti_forward_speed*deltaTime;
		anti_position.x += anti_forward.x*anti_forward_speed*deltaTime;
		anti_position.y += anti_up_speed * deltaTime;
	}
	
	else {
		
		anti_position.x = car_position.x;
		anti_position.z = car_position.z + car_forward.z;
		anti_position.y = car_position.y + 0.2f;
	}
	
	
}
void fkr_update() {
	
	//rand_move();
	fkr_forward_speed += fkr_forward_acceleration * deltaTime;

	fkr_forward_speed = fkr_forward_speed < 3.0f ? fkr_forward_speed : 3.0f;
	fkr_forward_speed = fkr_forward_speed > 0.0f ? fkr_forward_speed : 0.0f;
	
	fkr_rotation_ratio += fkr_rotation_acceleration * deltaTime;
	if (fkr_rotation_acceleration!=0.0f && abs(fkr_tilt_ratio) <= 20.0f) {
		fkr_tilt_ratio += fkr_rotation_acceleration*0.3f * deltaTime;
	}
	else if (fkr_rotation_acceleration== 0.0f && fkr_tilt_ratio != 0.0f) {
		float new_fkr_tilt_ratio = fkr_tilt_ratio- fkr_tilt_ratio * 0.4f * deltaTime;
		fkr_tilt_ratio = (new_fkr_tilt_ratio * fkr_tilt_ratio) < 0 ? 0.0f : new_fkr_tilt_ratio;
	}
	fkr_up_speed += fkr_up_acceleration * deltaTime;
	fkr_up_speed = fkr_up_speed > 1.0f ? 1.0f : fkr_up_speed;
	fkr_up_speed = fkr_up_speed < -1.0f ? -1.0f : fkr_up_speed;
	if (fkr_rotation_ratio != 0.0f) {
		fkr_forward.z = (-1)*sin(glm::radians(fkr_rotation_ratio + 90.0f));
		fkr_forward.x = cos(glm::radians(fkr_rotation_ratio + 90.0f));
	}
	fkr_forward += 1.2f*glm::normalize(glm::vec3(0, 0, 0) - fkr_position);
	int dis = glm::distance2(fkr_position, car_position);
	glm::vec3 fkr_forwardT = glm::normalize(car_position - fkr_position);
	glm::vec3 fkr_forwardA = glm::normalize(fkr_position - anti_position);
	if (lastFrame>30){
		int disAnti= glm::distance2(fkr_position, anti_position);
	//	cout << "anti" << disAnti << endl;	
		glm::vec3 fkr_rotate = glm::vec3(0, 0, 0);
		fkr_rotate.x = -fkr_forwardA.z;
		fkr_rotate.y = fkr_forwardA.y;
		fkr_rotate.z = fkr_forwardA.x;
		if ( 0< disAnti<60) {
			if (dis < disAnti|| lastFrame>60) {
				fkr_forwardA = fkr_rotate;
				fkr_forward = glm::normalize(fkr_forwardA + fkr_forwardT);
				fkr_forward = fkr_forwardT;
			}
			
			else {
				
				if (disAnti < 30) {
					fkr_forward.x = fkr_forwardA.x*cos(glm::radians(3 * float(disAnti)))- fkr_forwardA.z*sin(glm::radians(3 * float(disAnti)));
					fkr_forward.y = fkr_forwardA.y;
					fkr_forward.z = fkr_forwardA.x*sin(glm::radians(3 *float( disAnti))) + fkr_forwardA.z*cos(glm::radians(3 * float(disAnti)));
					fkr_forward = fkr_rotate;
				}
				else {
				    fkr_forwardA = fkr_rotate;
					fkr_forward = glm::normalize(fkr_forwardA + fkr_forwardT);
				}
			}
			
		}
		else {
			fkr_forward = fkr_forwardT;
		}
		if (disAnti == 0) {
			fkr_forward_speed = 0.0f;
			
		}
		else {
			fkr_forward_speed = 2.0f;
		}
		
	}
		

	fkr_position.z += fkr_forward.z*fkr_forward_speed*deltaTime;
	fkr_position.x += fkr_forward.x*fkr_forward_speed*deltaTime;
	fkr_position.y += fkr_up_speed * deltaTime;

	if (fkr_forward_speed == 0.0f) {
		fkr_position.y = 2 + 0.05*sin(glm::radians(50*pi*lastFrame));	

	}
	if (fkr_position.y < 0.15) {
		fkr_position.y = 0.15;
	}
	
	/*fkr_position.x = car_position.x;
	fkr_position.z = car_position.z + car_forward.z;
	fkr_position.y = car_position.y + 0.2f;*/
	if (lastFrame > 30 || lastFrame<20) {
		camera_position.x = fkr_position.x - fkr_forwardT.x;
		camera_position.z = fkr_position.z - fkr_forwardT.z;
		camera_position.y = fkr_position.y + 0.4f;
	}
}
void fkr_move() {
	fkr_rotation_acceleration = 10.0f;
	if (lastFrame > 9) {
		fkr_rotation_acceleration = 0;
	}
	
}
void rand_move() {
//	fkr_move();
	if (int(lastFrame) % 3 == 0) {
		fkrtake = rand() % 8;
	}
	if (fkrtake == 0)
		fkr_rotation_acceleration = 0.0f;
	    fkr_forward_acceleration = 1.0f;
	if (fkrtake == 1)
		fkr_rotation_acceleration = -5.0f;
	if (fkrtake == 2) {
		fkr_rotation_acceleration = 0.0f;
		fkr_forward_acceleration = 0.4f;
	}
	if (fkrtake == 3) {
		fkr_rotation_acceleration = 0.0f;
		fkr_forward_acceleration = -0.8f;
	}
	if (fkrtake == 4) {
		fkr_rotation_acceleration = 5.0f;
		
	}
	if (fkrtake == 5)
		fkr_rotation_acceleration = 10.0f;
	if (fkrtake == 6) {
		fkr_rotation_acceleration = 0.0f;
		fkr_forward_acceleration = 0.8f;
	}
	if (fkrtake == 7) {
		fkr_rotation_acceleration = 0.0f;
		fkr_forward_acceleration = 0.0f;
	}

}
void avoid() {
	
	if ((car_position.x >= 3.6f) && (car_position.z >= 4.1f)) {
		car_rotation_acceleration = 30.0f;
		return;
	}
	if ((car_position.x >= 3.6f) && (car_position.z <= 4.1f) && (car_position.z >= -4.0f)) {
		car_rotation_acceleration = 0.0f;
		car_forward_acceleration = 0.4f;
		return;
	}
	if ((car_position.x >= 3.6f) && (car_position.z <= -3.3f)) {
		car_rotation_acceleration = 30.0f;
		return;
	}
	if ((car_position.x <= 3.6f) && (car_position.z <= -3.3f) && (car_position.x >= -3.2f)) {
		car_rotation_acceleration = 0.0f;
		car_forward_acceleration = 0.4f;
		return;
	}
	if ((car_position.x <= -3.2f) && (car_position.z <= -4.1f)) {
		car_rotation_acceleration = 30.0f;
		return;
	}
	if ((car_position.x <= -3.2f) && (car_position.z >= -4.1f) && (car_position.z <= -2.0f)) {
		car_rotation_acceleration = 0.0f;
		car_forward_acceleration = 0.4f;
		return;
	}
	if ((car_position.x <= -3.2f) && (car_position.z >= -2.0f) && (car_position.z <= -1.0f)) {
		car_rotation_acceleration = -20.0f;

		return;
	}
	if ((car_position.x <= -3.2f) && (car_position.z >= -1.0f) && (car_position.z <= 0.0f)) {
		car_rotation_acceleration = 30.0f;
		return;
	}
	if ((car_position.x <= -3.2f) && (car_position.z >= 0.0f) && (car_position.z <= 2.4f)) {
		car_forward_acceleration = 0.4f;
		car_rotation_acceleration = 0.0f;

		return;
	}
	if ((car_position.x <= -3.65f) && (car_position.z >= 2.5f) && (car_position.z <= 3.7f)) {
		car_rotation_acceleration = 50.0f;
		return;
	}
	if ((car_position.x >= -3.65f) && (car_position.x <= 4.0f) && (car_position.z <= 5.0f) && (car_position.z >= 3.0f)) {
		car_rotation_acceleration = 0.0f;
		car_forward_acceleration = 1.0f;
		car_up_acceleration = -0.2f;
		return;
	}
}
void processInput(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
	if (action == GLFW_PRESS && key == GLFW_KEY_SPACE) {
		if (status == STATUS::NONE) {
			status = STATUS::MOVE;
			std::cout << "Switch status from NONE to MOVE" << std::endl;
		}
		else {
			status = STATUS::NONE;
			std::cout << "Switch status from MOVE to NONE" << std::endl;
		}
	}
	if (status == STATUS::MOVE) {
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_A)
				car_rotation_acceleration = 30.0f;
			if (key == GLFW_KEY_D)
				car_rotation_acceleration = -30.0f;
			if (key == GLFW_KEY_Z)
				car_up_acceleration = 0.4f;
			if (key == GLFW_KEY_X)
				car_up_acceleration = -0.8f;
			if (key == GLFW_KEY_W)
				car_forward_acceleration = 0.8f;
			if (key == GLFW_KEY_S)
				car_forward_acceleration = -0.8f;

		}

		if (action == GLFW_RELEASE) {
			if (key == GLFW_KEY_A || key == GLFW_KEY_D)
				car_rotation_acceleration = 0.0f;
			if (key == GLFW_KEY_W || key == GLFW_KEY_S)
				car_forward_acceleration = -0.1f;
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// renders the scene
void renderScene(const Shader &shader, vector<Model> models )
{
	// floor
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(10.0, 10.0, 10.0));
	shader.setMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.setMat4("model", model);
	renderCube();

	// render the obj model
/*	model = glm::mat4();
	model = glm::translate(model, fkr_position);
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	// model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(fkr_rotation_ratio + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-fkr_tilt_ratio), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.setMat4("model", model);
	models[0].Draw(shader);

	// render the target
	model = glm::mat4();
	model = glm::translate(model, car_position);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(car_rotation_ratio + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	models[1].Draw(shader);*/

	model = glm::mat4();
	// render the model
	shader.setMat4("model", model);
	models[2].Draw(shader);

	/*model = glm::mat4();
	model = glm::translate(model, anti_position);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	// model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(car_rotation_ratio + anti_rotation_ratio + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	models[3].Draw(shader);*/
//	Model fence(FileSystem::getPath("res/obstacle/fence.obj"));
	// render the fence
	model = glm::mat4();
	shader.setMat4("model", model);
	//fence.Draw(shader);
	models[4].Draw(shader);
}


// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
																  // front face
																  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
																  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
																  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
																  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
																  -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
																  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
																														// left face
																														-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
																														-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
																														-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
																														-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
																														-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
																														-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
																																											  // right face
																																											  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
																																											  1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
																																											  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
																																											  1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
																																											  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
																																											  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
																																																								   // bottom face
																																																								   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
																																																								   1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
																																																								   1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
																																																								   1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
																																																								   -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
																																																								   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
																																																																						 // top face
																																																																						 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
																																																																						 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
																																																																						 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
																																																																						 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
																																																																						 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
																																																																						 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
