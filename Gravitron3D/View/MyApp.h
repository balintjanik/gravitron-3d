#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Utils
#include "GLUtils.hpp"
#include "Camera.h"
#include "CameraManipulator.h"

#include "../Model/particle.h"
#include "../Model/octree.h"
#include <thread>

struct SUpdateInfo
{
	float ElapsedTimeInSec = 0.0f; // Program indulása óta eltelt idő
	float DeltaTimeInSec   = 0.0f; // Előző Update óta eltelt idő
};

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init();
	void Clean();

	void Update( const SUpdateInfo& );
	void Render();
	void RenderGUI();

	void KeyboardDown(const SDL_KeyboardEvent&);
	void KeyboardUp(const SDL_KeyboardEvent&);
	void MouseMove(const SDL_MouseMotionEvent&);
	void MouseDown(const SDL_MouseButtonEvent&);
	void MouseUp(const SDL_MouseButtonEvent&);
	void MouseWheel(const SDL_MouseWheelEvent&);
	void Resize(int, int);

	void OtherEvent( const SDL_Event& );
protected:
	void SetupDebugCallback();

	//
	// Adat változók
	//

	float m_ElapsedTimeInSec = 0.0f;
	float m_DeltaTimeInSec = 0.0f;

	bool m_IsLampOn = true;
	bool m_animatePointLight = false;

	// Sphere params

	static constexpr glm::vec3 SPHERE_POS = glm::vec3( -2.0f, 1.0f, -2.0f );

	// Kamera
	Camera m_camera;
	CameraManipulator m_cameraManipulator;

	// Fényforrások

	// static constexpr glm::vec3 BUG_COLOR = glm::vec3( 0.53f, 1.0f, 0.3f );

	//
	// OpenGL-es dolgok
	//

	// shaderekhez szükséges változók
	GLuint m_programID = 0; // shaderek programja

	// Fényforrás- ...
	glm::vec4 m_lightPos = glm::vec4( 0.0f, -1.0f, 0.0f, 0.0f );

	float m_lightConstantAttenuation    = 0.0;
	float m_lightLinearAttenuation      = 1.0;
	float m_lightQuadraticAttenuation   = 0.0;

	// Shaderek inicializálása, és törtlése
	void InitShaders();
	void CleanShaders();

	// Geometriával kapcsolatos változók
	float scale_factor = 0.005f;
	OGLObject m_sphereGPU = {};
	GLuint  instanceVBO = 0;

	unsigned int fps = 0;
	unsigned int frames;
	float time_since_last_sec = 0.0f;

	std::vector<glm::vec4> particlePositions;

	size_t num_threads = std::thread::hardware_concurrency(); // Get the number of available threads
	float speed = 1.0;
	const float theta = 1.8;
	const float epsilon = 1.0;
	const int n = 50000;
	std::vector<Particle> particles;
	Octree octree;

	// Geometria inicializálása, és törtlése
	void InitGeometry();
	void CleanGeometry();

	// Textúrázás, és változói
    GLuint m_SamplerID = 0;
	GLuint m_sphereTextureID = 0;

	void InitTextures();
	void CleanTextures();
};

