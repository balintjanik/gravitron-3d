#include "SimulationView.h"
#include "SDL_GLDebugMessageCallback.h"
#include "ObjParser.h"

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

void SimulationView::InitSimulation() {
	simulationManager.initSimulation(numberOfParticles, presetType, positionType, velocityType);

	simulationManager.settings.setTheta(theta);
	simulationManager.settings.setEpsilon(epsilon);
	
	simulationManager.settings.setSimulationSpeed(simulationSpeed);
}

void SimulationView::InitImGuiSettings() {
	io = ImGui::GetIO();

	windowWidth = 350.0f;
	windowHeight = io.DisplaySize.y;
	// ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - windowWidth, 0)); // Right side
	ImGui::SetNextWindowPos(ImVec2(0, 0)); // Left side
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
}

bool SimulationView::Init()
{
	InitSimulation();

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

	InitImGuiSettings();

	return true;
}

void SimulationView::Clean()
{
	CleanShaders();
	CleanGeometry();
	CleanTextures();
}

void SimulationView::UpdateData() {
	// FPS
	timeSinceLastSec += m_DeltaTimeInSec;
	frames++;
	if (timeSinceLastSec >= 1.0f) {
		fps = frames;
		frames = 0;
		timeSinceLastSec = 0.0f;
	}

	// Threads
	numberOfThreads = simulationManager.settings.getNumberOfThreads();

	// Current simulation info
	currentNumberOfParticles = simulationManager.settings.getNumberOfParticles();

	// Simulation settings
	simulationSpeed = simulationManager.settings.getSimulationSpeed();
	theta = simulationManager.settings.getTheta();
	epsilon = simulationManager.settings.getEpsilon();

}

void SimulationView::Update( const SUpdateInfo& updateInfo )
{
	m_ElapsedTimeInSec = updateInfo.ElapsedTimeInSec;
	m_DeltaTimeInSec = updateInfo.DeltaTimeInSec;

	m_cameraManipulator.Update( updateInfo.DeltaTimeInSec );

	// Update model
	simulationManager.updateSimulation(updateInfo);

	// Update data
	UpdateData();

	// Copy particle positions to display
	particlePositions.clear();
	for (const auto& p : simulationManager.particles)
	{
		particlePositions.push_back(glm::vec4(p.getPosition(), p.getForce()));
	}

	// Orphan the old buffer and replace it with the new particle positions
	glNamedBufferData(instanceVBO, sizeof(glm::vec4) * particlePositions.size(), particlePositions.data(), GL_DYNAMIC_DRAW);
}

void SimulationView::Render()
{
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Uniforms
	glProgramUniform1f(m_programID, ul(m_programID, "scaleFactor"), scaleFactor);
	glProgramUniform3fv(m_programID, ul(m_programID, "cameraPos"), 1, glm::value_ptr(m_camera.GetEye()));
	glProgramUniform4fv(m_programID, ul(m_programID, "lightPos"), 1, glm::value_ptr(lightPos));
	glProgramUniform1f(m_programID, ul(m_programID, "lightConstantAttenuation"), lightConstantAttenuation);
	glProgramUniform1f(m_programID, ul(m_programID, "lightLinearAttenuation"), lightLinearAttenuation);
	glProgramUniform1f(m_programID, ul(m_programID, "lightQuadraticAttenuation"), lightQuadraticAttenuation);
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

template <typename EnumType, size_t N>
bool SimulationView::ShowEnumDropdown(const char* label, const char* (&names)[N], EnumType& currentValue) {
	bool valueChanged = false;

	if (ImGui::BeginCombo(label, names[static_cast<int>(currentValue)])) {
		for (size_t i = 0; i < N; i++) {
			bool isSelected = (currentValue == static_cast<EnumType>(i));

			if (ImGui::Selectable(names[i], isSelected)) {
				currentValue = static_cast<EnumType>(i);
				valueChanged = true;
			}

			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	return valueChanged;
}

void SimulationView::UpdateMessage(std::string newMessage, glm::vec3 newMessageColor) {
	message = newMessage;
	messageColor = newMessageColor;
	messageTime = 0.0f;
}

void SimulationView::ShowMessage(float r, float g, float b) {
	messageTime += ImGui::GetIO().DeltaTime;

	// Pulsate when the message is new
	float alpha = 1.0f;
	if (messageTime < 1.5f) {
		alpha = (sin(messageTime * 30.0f) + 1.0f) * 0.25f + 0.5f;
	}
	ImVec4 pulsatingColor = ImVec4(r, g, b, alpha);

	ImGui::PushStyleColor(ImGuiCol_Text, pulsatingColor);

	ImGui::SetCursorPos(ImVec2(10, ImGui::GetIO().DisplaySize.y - 30));
	ImGui::Text(message.c_str());

	// Restore the default text color
	ImGui::PopStyleColor();
}

void SimulationView::ShowLoadSaveSettingsUI() {
	// Ensure directories exist before saving or loading
	std::filesystem::create_directories("UserData");
	std::filesystem::create_directories("UserData/Settings");

	// List all .stg files in the "UserData/Settings" folder
	availableFiles.clear();
	availableFiles.push_back("");
	for (const auto& entry : std::filesystem::directory_iterator("UserData/Settings")) {
		if (entry.is_regular_file() && entry.path().extension() == ".stg") {
			availableFiles.push_back(entry.path().stem().string());
		}
	}

	// Load/save settings
	if (ImGui::CollapsingHeader("Load/Save Settings")) {
		// Save settings
		ImGui::SeparatorText("Save settings");
		ImGui::InputText("##SettingsFilename", saveFileName, IM_ARRAYSIZE(saveFileName));
		ImGui::SameLine();
		if (ImGui::Button("Save Settings")) {
			try {
				if (strlen(saveFileName) > 0) {
					std::string filePath = "UserData/Settings/" + std::string(saveFileName) + ".stg";

					if (std::filesystem::exists(filePath)) {
						UpdateMessage("File already exists. Please choose a different name.", glm::vec3(1.0f, 0.0f, 0.0f));
					}
					else {
						simulationManager.saveSettings(filePath);
						UpdateMessage("Settings saved as " + std::string(saveFileName), glm::vec3(1.0f));
					}
				}
				else {
					UpdateMessage("You must enter a name for the save first!", glm::vec3(1.0f, 0.0f, 0.0f));
				}
			}
			catch (const std::exception& e) {
				UpdateMessage("Error while saving settings: " + std::string(e.what()), glm::vec3(1.0f, 0.0f, 0.0f));
			}
			catch (...) {
				UpdateMessage("An unknown error occured while saving settings.", glm::vec3(1.0f, 0.0f, 0.0f));
			}
		}

		// Load settings
		ImGui::SeparatorText("Load settings");
		if (!availableFiles.empty()) {
			// Convert availableFiles to const char* array for Combo box
			std::vector<const char*> fileNames;
			for (const auto& file : availableFiles) {
				fileNames.push_back(file.c_str());
			}

			// Display combo box with file names
			if (ImGui::BeginCombo("##SettingsFiles", selectedFileIndex > 0 ? availableFiles[selectedFileIndex].c_str() : "Select a file")) {
				for (int i = 0; i < availableFiles.size(); ++i) {
					bool isSelected = (i == selectedFileIndex);
					if (ImGui::Selectable(availableFiles[i].c_str(), isSelected)) {
						selectedFileIndex = i;
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			ImGui::SameLine();
			if (ImGui::Button("Load Settings")) {
				try {
					if (selectedFileIndex > 0) {
						std::string filePath = "UserData/Settings/" + availableFiles[selectedFileIndex] + ".stg";
						simulationManager.loadSettings(filePath);
						UpdateMessage("Settings loaded from " + availableFiles[selectedFileIndex], glm::vec3(1.0f));
					}
					else {
						UpdateMessage("You must choose a save to load!", glm::vec3(1.0f, 0.0f, 0.0f));
					}
				}
				catch (const std::exception& e) {
					UpdateMessage("Error while loading settings: " + std::string(e.what()), glm::vec3(1.0f, 0.0f, 0.0f));
				}
				catch (...) {
					UpdateMessage("An unknown error occured while loading settings.", glm::vec3(1.0f, 0.0f, 0.0f));
				}
			}
		}
		else {
			ImGui::Text("No saved settings found.");
		}
	}
}

void SimulationView::RenderGUI()
{
	// Window
	windowHeight = io.DisplaySize.y;
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoMove);
	windowWidth = ImGui::GetWindowSize().x;

	// Performance
	if (ImGui::CollapsingHeader("Performance")) {
		ImGui::Text("FPS: %d", fps);
		ImGui::Text("Threads: %d", numberOfThreads);
	}

	// Simulation info
	if (ImGui::CollapsingHeader("Simulation info")) {
		ImGui::Text("Number of particles: %d", currentNumberOfParticles);
	}

	// Light settings
	if (ImGui::CollapsingHeader("Light settings")) {
		bool isPoint = lightPos.w >= 1.0f;
		ImGui::Checkbox("Spot light (on) / Directional light (off)", &isPoint);
		lightPos.w = isPoint ? 1.f : 0.f;

		if (lightPos.w == 0.0f) // Directional light
		{
			glm::vec3 dir = glm::vec3(lightPos);
			ImGui::SliderFloat3("Light Direction", glm::value_ptr(dir), -1.f, 1.f);
			if (dir != glm::vec3(0.0f))
				dir = glm::normalize(dir);
			else dir = glm::vec3(0.f, -1.f, 0.f);
			lightPos = glm::vec4(dir, 0.0f);
		}
		else if (lightPos.w >= 1.f) // Spot light
		{
			lightPos = glm::vec4(0.f, 0.f, 0.f, 1.f);
			ImGui::SliderFloat3("Light Position", glm::value_ptr(lightPos), -100.f, 100.f);

			ImGui::SliderFloat("Constant Att.", &lightConstantAttenuation, 0.f, 1.f);
			ImGui::SliderFloat("Linear Att.", &lightLinearAttenuation, 0.f, 1.f);
			ImGui::SliderFloat("Quadratic Att.", &lightQuadraticAttenuation, 0.f, 1.f);
		}
	}

	// Display settings
	if (ImGui::CollapsingHeader("Display settings")) {
		ImGui::SliderFloat("Particle size", &scaleFactor, 0.001f, 0.1f);

		// TODO: add coloring, etc.
	}

	// Calculation settings
	if (ImGui::CollapsingHeader("Calculation settings")) {
		if (ImGui::SliderFloat("Simulation speed", &simulationSpeed, 0.f, 5.f))
			simulationManager.settings.setSimulationSpeed(simulationSpeed);
		
		if (ImGui::SliderFloat("Theta", &theta, simulationManager.settings.getThetaMin(), simulationManager.settings.getThetaMax()))
			simulationManager.settings.setTheta(theta);

		if (ImGui::SliderFloat("Epsilon", &epsilon, simulationManager.settings.getEpsilonMin(), simulationManager.settings.getEpsilonMax()))
			simulationManager.settings.setEpsilon(epsilon);
	}

	// Load/save settings
	ShowLoadSaveSettingsUI();

	// New simulation
	if (ImGui::CollapsingHeader("New simulation")) {
		ImGui::SliderInt("Number of particles", &numberOfParticles, 0, 150000);

		ShowEnumDropdown("Preset Type", PRESET_TYPE_NAMES, presetType);

		if (presetType == PRESET_CUSTOM) {
			ShowEnumDropdown("Position Type", POSITION_TYPE_NAMES, positionType);
			ShowEnumDropdown("Velocity Type", VELOCITY_TYPE_NAMES, velocityType);
		}

		if (ImGui::Button("Start New Simulation")) {
			InitSimulation();
		}
	}

	// Messages
	ShowMessage(messageColor.r, messageColor.g, messageColor.b);

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
