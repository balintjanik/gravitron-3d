#include "SimulationView.h"
#include "SDL_GLDebugMessageCallback.h"
#include "ObjParser.h"

#include <imgui.h>
#include <iostream>
#include <filesystem>

SimulationView::SimulationView()
{
}

SimulationView::~SimulationView()
{
}

void SimulationView::SetupDebugCallback()
{
	GLint context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(SDL_GLDebugMessageCallback, nullptr);
	}
}

void SimulationView::InitShaders()
{
	m_programID = glCreateProgram();
	AttachShader( m_programID, GL_VERTEX_SHADER, "Shaders/shader.vert" );
	AttachShader( m_programID, GL_FRAGMENT_SHADER, "Shaders/shader.frag" );
	LinkProgram( m_programID );
}

void SimulationView::CleanShaders()
{
	glDeleteProgram( m_programID );
}

void SimulationView::InitGeometry()
{

	const std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
	{
		{ 0, offsetof( Vertex, position ), 3, GL_FLOAT },
		{ 1, offsetof( Vertex, normal   ), 3, GL_FLOAT },
		{ 2, offsetof( Vertex, texcoord ), 2, GL_FLOAT },
	};

	// Sphere
	MeshObject<Vertex> sphereMeshCPU = ObjParser::parse("Assets/rockICO.obj");

	m_sphereGPU = CreateGLObjectFromMesh( sphereMeshCPU, vertexAttribList );

	glBindVertexArray(m_sphereGPU.vaoID);

	// 3. Instance VBO (particlePositions contains the instance positions, i.e., per-particle positions)
	glCreateBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, particlePositions.size() * sizeof(glm::vec4), particlePositions.data(), GL_DYNAMIC_DRAW);

	// Setup instance position attribute (location 3, four-component vector)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glVertexAttribDivisor(3, 1);  // Make this attribute update once per instance

	glBindVertexArray(0);
}

void SimulationView::CleanGeometry()
{
	CleanOGLObject( m_sphereGPU );
}

void SimulationView::InitTextures()
{
	glCreateSamplers( 1, &m_SamplerID );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glSamplerParameteri( m_SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	ImageRGBA sphereImage = ImageFromFile( "Assets/rock.jpg" );

	glCreateTextures( GL_TEXTURE_2D, 1, &m_sphereTextureID );
	glTextureStorage2D( m_sphereTextureID, NumberOfMIPLevels( sphereImage ), GL_RGBA8, sphereImage.width, sphereImage.height );
	glTextureSubImage2D( m_sphereTextureID, 0, 0, 0, sphereImage.width, sphereImage.height, GL_RGBA, GL_UNSIGNED_BYTE, sphereImage.data() );

	glGenerateTextureMipmap( m_sphereTextureID );
}

void SimulationView::CleanTextures()
{
	glDeleteTextures( 1, &m_sphereTextureID );
}

bool SimulationView::Init()
{
	simulationManager.initSimulation(80000, PresetType::PRESET_GALAXY, PositionType::POSITION_RANDOM, VelocityType::VELOCITY_ORBIT);

	SetupDebugCallback();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	InitShaders();
	InitGeometry();
	InitTextures();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); 

	glEnable(GL_DEPTH_TEST);

	m_camera.SetView(
		glm::vec3(0.0, 5.0, 10.0),  // From
		glm::vec3(0.0, 0.0, 0.0),   // To
		glm::vec3(0.0, 1.0, 0.0));  // Up

	m_cameraManipulator.SetCamera( &m_camera );

	return true;
}

void SimulationView::Clean()
{
	CleanShaders();
	CleanGeometry();
	CleanTextures();
}

void SimulationView::Update( const SUpdateInfo& updateInfo )
{
	m_ElapsedTimeInSec = updateInfo.ElapsedTimeInSec;
	m_DeltaTimeInSec = updateInfo.DeltaTimeInSec;

	m_cameraManipulator.Update( updateInfo.DeltaTimeInSec );

	// FPS
	timeSinceLastSec += m_DeltaTimeInSec;
	frames++;
	if (timeSinceLastSec >= 1.0f) {
		fps = frames;
		frames = 0;
		timeSinceLastSec = 0.0f;
	}

	// Update model
	simulationManager.updateSimulation(updateInfo);

	// Copy particle positions to display
	particlePositions.clear();
	for (const auto& p : simulationManager.particles)
	{
		particlePositions.push_back(glm::vec4(p.getPosition(), p.getForce()));
	}

	// Orphan the old buffer and replace it with the new particle positions
	glNamedBufferData(instanceVBO, sizeof(glm::vec4) * particlePositions.size(), &particlePositions[0], GL_DYNAMIC_DRAW);
}

void SimulationView::Render()
{
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Uniforms
	glProgramUniform1f(m_programID, ul(m_programID, "scaleFactor"), scaleFactor);
	glProgramUniform3fv(m_programID, ul(m_programID, "cameraPos"), 1, glm::value_ptr(m_camera.GetEye()));
	glProgramUniform4fv(m_programID, ul(m_programID, "lightPos"), 1, glm::value_ptr(m_lightPos));
	glProgramUniform1f(m_programID, ul(m_programID, "lightConstantAttenuation"), m_lightConstantAttenuation);
	glProgramUniform1f(m_programID, ul(m_programID, "lightLinearAttenuation"), m_lightLinearAttenuation);
	glProgramUniform1f(m_programID, ul(m_programID, "lightQuadraticAttenuation"), m_lightQuadraticAttenuation);
	glProgramUniformMatrix4fv( m_programID, ul( m_programID, "viewProj"), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );

	glm::mat4 matWorld = glm::identity<glm::mat4>();
	glProgramUniformMatrix4fv(m_programID, ul(m_programID, "world"), 1, GL_FALSE, glm::value_ptr(matWorld));
	glProgramUniform1i( m_programID, ul( m_programID, "texImage" ), 0 );

	glUseProgram( m_programID );

	// Textures
	glBindSampler( 0, m_SamplerID );
	glBindTextureUnit(0, m_sphereTextureID);
	glBindSampler(0, m_SamplerID);

	glBindVertexArray(m_sphereGPU.vaoID);

	// Draw instanced geometry
	glDrawElementsInstanced(GL_TRIANGLES, m_sphereGPU.count, GL_UNSIGNED_INT, nullptr, particlePositions.size());

	// Cleanup
	glUseProgram( 0 );
	glBindTextureUnit( 0, 0 );
	glBindSampler( 0, 0 );
	glBindVertexArray( 0 );
}

void SimulationView::RenderGUI()
{
	if (ImGui::Begin("Settings"))
	{
		bool isPoint = m_lightPos.w >= 1.0f;
		ImGui::Checkbox("Is Point?", &isPoint);
		m_lightPos.w = isPoint ? 1.f : 0.f;

		if (m_lightPos.w == 0.0f) // Directional light
		{
			glm::vec3 dir = glm::vec3(m_lightPos);
			ImGui::SliderFloat3("Light Direction", glm::value_ptr(dir), -1.f, 1.f);
			if (dir != glm::vec3(0.0f))
				dir = glm::normalize(dir);
			else dir = glm::vec3(0.f, -1.f, 0.f);
			m_lightPos = glm::vec4(dir, 0.0f);
		}
		else if (m_lightPos.w >= 1.f) // Spot light
		{
			m_lightPos = glm::vec4(0.f, 0.f, 0.f, 1.f);
			ImGui::SliderFloat3("Light Position", glm::value_ptr(m_lightPos), -100.f, 100.f);

			ImGui::SliderFloat("Constant Att.", &m_lightConstantAttenuation, 0.f, 1.f);
			ImGui::SliderFloat("Linear Att.", &m_lightLinearAttenuation, 0.f, 1.f);
			ImGui::SliderFloat("Quadratic Att.", &m_lightQuadraticAttenuation, 0.f, 1.f);
		}

		ImGui::Text("FPS: %d", fps);

		float simulationSpeed = simulationManager.settings.getSimulationSpeed();
		if (ImGui::SliderFloat("Delta Time", &simulationSpeed, 0.f, 5.f))
			simulationManager.settings.setSimulationSpeed(simulationSpeed);

		ImGui::SliderFloat("Particle size", &scaleFactor, 0.001f, 0.1f);
	}
	ImGui::End();
}

void SimulationView::KeyboardDown(const SDL_KeyboardEvent& key)
{	
	if ( key.repeat == 0 )
	{
		if ( key.keysym.sym == SDLK_F5 && key.keysym.mod & KMOD_CTRL )
		{
			CleanShaders();
			InitShaders();
		}
		if ( key.keysym.sym == SDLK_F1 )
		{
			GLint polygonModeFrontAndBack[ 2 ] = {};
			glGetIntegerv( GL_POLYGON_MODE, polygonModeFrontAndBack );
			GLenum polygonMode = ( polygonModeFrontAndBack[ 0 ] != GL_FILL ? GL_FILL : GL_LINE );
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glPolygonMode.xhtml
			glPolygonMode( GL_FRONT_AND_BACK, polygonMode );
		}
	}
	m_cameraManipulator.KeyboardDown( key );
}

void SimulationView::KeyboardUp(const SDL_KeyboardEvent& key)
{
	m_cameraManipulator.KeyboardUp( key );
}

void SimulationView::MouseMove(const SDL_MouseMotionEvent& mouse)
{
	m_cameraManipulator.MouseMove( mouse );
}

void SimulationView::MouseDown(const SDL_MouseButtonEvent& mouse)
{
}

void SimulationView::MouseUp(const SDL_MouseButtonEvent& mouse)
{
}

void SimulationView::MouseWheel(const SDL_MouseWheelEvent& wheel)
{
	m_cameraManipulator.MouseWheel( wheel );
}

void SimulationView::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);
	m_camera.SetAspect( static_cast<float>(_w) / _h );
}

void SimulationView::OtherEvent( const SDL_Event& ev )
{

}