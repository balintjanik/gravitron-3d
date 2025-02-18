#include "MyApp.h"
#include "SDL_GLDebugMessageCallback.h"
#include "ObjParser.h"

#include <imgui.h>
#include <iostream>
#include <filesystem>

void update_particle_range(std::vector<Particle>& particles, Octree& octree, float theta, float epsilon, float m_DeltaTimeInSec, float speed, size_t start, size_t end) {
	for (size_t i = start; i < end; i++) {
		if (i == 0) continue; //  TODO: remove this, I only have it so that the center particle will not move away like this
		Particle& p = particles[i];
		float acc_x = 0.0, acc_y = 0.0, acc_z = 0.0;
		float all_force = octree.acc(acc_x, acc_y, acc_z, p.pos_x, p.pos_y, p.pos_z, theta, epsilon);
		p.update(acc_x, acc_y, acc_z, m_DeltaTimeInSec, speed, all_force);
	}
}

glm::vec3 accelerationToColor(const glm::vec3& acceleration, const float dt) {
	float magnitude = glm::length(acceleration);

	float lowThreshold = 0.0f;
	float highThreshold = 3.0f * (1.0 / dt);

	float normalizedMagnitude = (magnitude - lowThreshold) / (highThreshold - lowThreshold);
	normalizedMagnitude = std::clamp(normalizedMagnitude, 0.0f, 1.0f);

	glm::vec3 color;
	if (normalizedMagnitude <= 0.5f) {
		color = glm::mix(glm::vec3(0, 0, 1), glm::vec3(1, 1, 0), normalizedMagnitude * 2.0f);
	}
	else {
		color = glm::mix(glm::vec3(1, 1, 0), glm::vec3(1, 0, 0), (normalizedMagnitude - 0.5f) * 2.0f);
	}

	return color;
}

CMyApp::CMyApp()
{
}

CMyApp::~CMyApp()
{
}

void CMyApp::SetupDebugCallback()
{
	// engedélyezzük és állítsuk be a debug callback függvényt ha debug context-ben vagyunk 
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

void CMyApp::InitShaders()
{
	m_programID = glCreateProgram();
	AttachShader( m_programID, GL_VERTEX_SHADER, "Shaders/shader.vert" );
	AttachShader( m_programID, GL_FRAGMENT_SHADER, "Shaders/shader.frag" );
	LinkProgram( m_programID );
}

void CMyApp::CleanShaders()
{
	glDeleteProgram( m_programID );
}

void CMyApp::InitGeometry()
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

void CMyApp::CleanGeometry()
{
	CleanOGLObject( m_sphereGPU );
}

void CMyApp::InitTextures()
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

void CMyApp::CleanTextures()
{
	glDeleteTextures( 1, &m_sphereTextureID );
}

bool CMyApp::Init()
{
	// Init
	particles = initParticles(n);

	SetupDebugCallback();

	// törlési szín legyen kékes
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	InitShaders();
	InitGeometry();
	InitTextures();

	//
	// egyéb inicializálás
	//

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hátrafelé néző lapok eldobását
	glCullFace(GL_BACK);    // GL_BACK: a kamerától "elfelé" néző lapok, GL_FRONT: a kamera felé néző lapok

	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	// kamera
	m_camera.SetView(
		glm::vec3(0.0, 5.0, 10.0),// honnan nézzük a színteret	   - eye
		glm::vec3(0.0, 0.0, 0.0),   // a színtér melyik pontját nézzük - at
		glm::vec3(0.0, 1.0, 0.0));  // felfelé mutató irány a világban - up

	m_cameraManipulator.SetCamera( &m_camera );

	return true;
}

void CMyApp::Clean()
{
	CleanShaders();
	CleanGeometry();
	CleanTextures();
}

void CMyApp::Update( const SUpdateInfo& updateInfo )
{
	m_ElapsedTimeInSec = updateInfo.ElapsedTimeInSec;
	m_DeltaTimeInSec = updateInfo.DeltaTimeInSec;

	m_cameraManipulator.Update( updateInfo.DeltaTimeInSec );

	// FPS
	time_since_last_sec += m_DeltaTimeInSec;
	frames++;
	if (time_since_last_sec >= 1.0f) {
		fps = frames;
		frames = 0;
		time_since_last_sec = 0.0f;
	}

	// Build tree
	Quad quad = Quad::new_containing(particles);
	octree.clear(quad);

	for (auto& p : particles)
	{
		octree.insert(p.pos_x, p.pos_y, p.pos_z, p.mass);
	}

	octree.propagate();

	if (num_threads == 0) num_threads = 4; // Fallback to 4 if hardware_concurrency cannot detect

	std::vector<std::thread> threads;
	size_t chunk_size = n / num_threads;

	for (size_t t = 0; t < num_threads; ++t) {
		size_t start = t * chunk_size;
		size_t end = (t == num_threads - 1) ? n : (t + 1) * chunk_size;

		threads.emplace_back(update_particle_range, std::ref(particles), std::ref(octree), theta, epsilon, m_DeltaTimeInSec, speed, start, end);
	}

	// Join all threads
	for (std::thread& thread : threads) {
		thread.join();
	}

	particlePositions.clear();
	for (const auto& p : particles)
	{
		//particlePositions.push_back(glm::vec3((p.pos_x / 550.0) - 1.0, 1.0 - (p.pos_y / 550.0), (p.pos_z / 550.0) - 1.0));
		particlePositions.push_back(glm::vec4(p.pos_x / 100, p.pos_y / 100, p.pos_z / 100, p.all_force));
		//std::cout << particlePositions[particlePositions.size() - 1].w << std::endl;
	}

	// Orphan the old buffer and replace it with the new particle positions
	glNamedBufferData(instanceVBO, sizeof(glm::vec4) * particlePositions.size(), &particlePositions[0], GL_DYNAMIC_DRAW);
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT)...
	// ... és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// - Uniform paraméterek

	glProgramUniform1f(m_programID, ul(m_programID, "scaleFactor"), scale_factor); // ez saját

	glProgramUniform3fv(
		m_programID,
		ul(m_programID, "cameraPos"),
		1,
		glm::value_ptr(m_camera.GetEye()));

	glProgramUniform4fv(m_programID, ul(m_programID, "lightPos"), 1,
		glm::value_ptr(m_lightPos));

	glProgramUniform1f(m_programID, ul(m_programID, "lightConstantAttenuation"), m_lightConstantAttenuation);
	glProgramUniform1f(m_programID, ul(m_programID, "lightLinearAttenuation"), m_lightLinearAttenuation);
	glProgramUniform1f(m_programID, ul(m_programID, "lightQuadraticAttenuation"), m_lightQuadraticAttenuation);

	// view és projekciós mátrix
	glProgramUniformMatrix4fv( m_programID, ul( m_programID, "viewProj"), 1, GL_FALSE, glm::value_ptr( m_camera.GetViewProj() ) );

	// Transzformációs mátrixok
	glm::mat4 matWorld = glm::identity<glm::mat4>();
	glProgramUniformMatrix4fv(m_programID, ul(m_programID, "world"), 1, GL_FALSE, glm::value_ptr(matWorld));

	// - textúraegységek beállítása
	glProgramUniform1i( m_programID, ul( m_programID, "texImage" ), 0 );

	glBindSampler( 0, m_SamplerID );

	glUseProgram( m_programID );

	// sphere

	/// std::cout << particles[1].pos_x << ", " << particles[1].pos_y << ", " << particles[1].pos_z << std::endl;
	/*for (int i = 0; i < n; i++) {
		matWorld = glm::translate( glm::vec3(particles[i].pos_x/550, particles[i].pos_y/550, particles[i].pos_z/550)) * glm::scale(glm::vec3(0.005, 0.005, 0.005));

		glProgramUniformMatrix4fv( m_programID, ul( m_programID, "world" ),    1, GL_FALSE, glm::value_ptr( matWorld ) );
		glProgramUniformMatrix4fv( m_programID, ul( m_programID, "worldIT" ),  1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( matWorld ) ) ) );

		glBindVertexArray( m_sphereGPU.vaoID );

		// - Textúrák beállítása, minden egységre külön
		glBindTextureUnit( 0, m_sphereTextureID );
		glBindSampler( 0, m_SamplerID );


		glDrawElements( GL_TRIANGLES,    
						m_sphereGPU.count,			 
						GL_UNSIGNED_INT,
						nullptr );
	}*/

	glBindVertexArray(m_sphereGPU.vaoID);

	glBindTextureUnit(0, m_sphereTextureID);
	glBindSampler(0, m_SamplerID);

	// Draw instanced geometry
	glDrawElementsInstanced(GL_TRIANGLES, m_sphereGPU.count, GL_UNSIGNED_INT, nullptr, particlePositions.size());

	// shader kikapcsolasa
	glUseProgram( 0 );

	// - Textúrák kikapcsolása, minden egységre külön
	glBindTextureUnit( 0, 0 );
	glBindSampler( 0, 0 );


	// VAO kikapcsolása
	glBindVertexArray( 0 );
}

void CMyApp::RenderGUI()
{
	// ImGui::ShowDemoWindow();

	if (ImGui::Begin("Settings"))
	{
		bool isPoint = m_lightPos.w >= 1.0f;
		ImGui::Checkbox("Is Point?", &isPoint);
		m_lightPos.w = isPoint ? 1.f : 0.f;

		if (m_lightPos.w == 0.0f) //Irány fényforrás
		{
			glm::vec3 dir = glm::vec3(m_lightPos);
			ImGui::SliderFloat3("Light Direction", glm::value_ptr(dir), -1.f, 1.f);
			if (dir != glm::vec3(0.0f))
				dir = glm::normalize(dir);
			else dir = glm::vec3(0.f, -1.f, 0.f);
			m_lightPos = glm::vec4(dir, 0.0f);
		}
		else if (m_lightPos.w >= 1.f) //Pont (vagy spot) fényforrás
		{
			m_lightPos = glm::vec4(0.f, 0.f, 0.f, 1.f);
			ImGui::Checkbox("Animate", &m_animatePointLight);
			if (m_animatePointLight)
				m_lightPos = glm::vec4(sinf(m_ElapsedTimeInSec * glm::pi<float>() * 2.f * 0.25f) * 5.f, 2.f, 0.f, 1.f);
			else
				ImGui::SliderFloat3("Light Position", glm::value_ptr(m_lightPos), -100.f, 100.f);

			ImGui::SliderFloat("Constant Att.", &m_lightConstantAttenuation, 0.f, 1.f);
			ImGui::SliderFloat("Linear Att.", &m_lightLinearAttenuation, 0.f, 1.f);
			ImGui::SliderFloat("Quadratic Att.", &m_lightQuadraticAttenuation, 0.f, 1.f);
		}

		ImGui::Text("FPS: %d", fps);

		ImGui::SliderFloat("Delta Time", &speed, 0.f, 5.f);

		ImGui::SliderFloat("Particle size", &scale_factor, 0.001f, 0.1f);
	}
	ImGui::End();
}

// https://wiki.libsdl.org/SDL2/SDL_KeyboardEvent
// https://wiki.libsdl.org/SDL2/SDL_Keysym
// https://wiki.libsdl.org/SDL2/SDL_Keycode
// https://wiki.libsdl.org/SDL2/SDL_Keymod

void CMyApp::KeyboardDown(const SDL_KeyboardEvent& key)
{	
	if ( key.repeat == 0 ) // Először lett megnyomva
	{
		if ( key.keysym.sym == SDLK_F5 && key.keysym.mod & KMOD_CTRL )
		{
			CleanShaders();
			InitShaders();
		}
		if ( key.keysym.sym == SDLK_F1 )
		{
			GLint polygonModeFrontAndBack[ 2 ] = {};
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml
			glGetIntegerv( GL_POLYGON_MODE, polygonModeFrontAndBack ); // Kérdezzük le a jelenlegi polygon módot! Külön adja a front és back módokat.
			GLenum polygonMode = ( polygonModeFrontAndBack[ 0 ] != GL_FILL ? GL_FILL : GL_LINE ); // Váltogassuk FILL és LINE között!
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glPolygonMode.xhtml
			glPolygonMode( GL_FRONT_AND_BACK, polygonMode ); // Állítsuk be az újat!
		}
	}
	m_cameraManipulator.KeyboardDown( key );
}

void CMyApp::KeyboardUp(const SDL_KeyboardEvent& key)
{
	m_cameraManipulator.KeyboardUp( key );
}

// https://wiki.libsdl.org/SDL2/SDL_MouseMotionEvent

void CMyApp::MouseMove(const SDL_MouseMotionEvent& mouse)
{
	m_cameraManipulator.MouseMove( mouse );
}

// https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent

void CMyApp::MouseDown(const SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(const SDL_MouseButtonEvent& mouse)
{
}

// https://wiki.libsdl.org/SDL2/SDL_MouseWheelEvent

void CMyApp::MouseWheel(const SDL_MouseWheelEvent& wheel)
{
	m_cameraManipulator.MouseWheel( wheel );
}


// a két paraméterben az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);
	m_camera.SetAspect( static_cast<float>(_w) / _h );
}

// Le nem kezelt, egzotikus esemény kezelése
// https://wiki.libsdl.org/SDL2/SDL_Event

void CMyApp::OtherEvent( const SDL_Event& ev )
{

}