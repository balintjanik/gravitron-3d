#include "SimulationView.h"
#include "SDL_GLDebugMessageCallback.h"
#include "ObjParser.h"
#include "GLUtils.hpp"

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

void SimulationView::CleanShaders() const
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

	// Node
	m_nodeGPU = CreateGLObjectFromMesh(createCube(), vertexAttribList);

	// Sphere
	MeshObject<Vertex> sphereMeshCPU = ObjParser::parse("Assets/rockICO.obj");

	m_sphereGPU = CreateGLObjectFromMesh( sphereMeshCPU, vertexAttribList );

	glBindVertexArray(m_sphereGPU.vaoID);

	// Instance VBO (particles contains the entire particle data)
	glCreateBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, simulationManager.particles.size() * sizeof(Particle), simulationManager.particles.data(), GL_DYNAMIC_DRAW);

	// Setup instance positionMass attribute (location 3, four-component vector)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	glVertexAttribDivisor(3, 1);

	// Setup instance velocitySize attribute (location 4, four-component vector)
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(glm::vec4)));
	glVertexAttribDivisor(4, 1);

	// Setup instance accelerationForce attribute (location 5, four-component vector)
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribDivisor(5, 1);

	// Setup instance colorMovable attribute (location 6, four-component vector)
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	///////////
	glBindVertexArray(m_nodeGPU.vaoID);

	const GLuint nodeEdges[] = {
		// Front face edges
		0, 1,  1, 2,  2, 3,  3, 0,
		// Back face edges
		4, 5,  5, 6,  6, 7,  7, 4,
		// Side edges connecting front and back
		0, 5,  2, 7,  3, 6,  4, 1
	};

	// Upload the edge indices to a separate `EBO`
	glGenBuffers(1, &edgeEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(nodeEdges), nodeEdges, GL_STATIC_DRAW);
}

void SimulationView::CleanGeometry()
{
	CleanOGLObject( m_nodeGPU );
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

	ImageRGBA nodeImage = ImageFromFile("Assets/node.jpg");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_nodeTextureID);
	glTextureStorage2D( m_nodeTextureID, NumberOfMIPLevels(nodeImage), GL_RGBA8, nodeImage.width, nodeImage.height );
	glTextureSubImage2D( m_nodeTextureID, 0, 0, 0, nodeImage.width, nodeImage.height, GL_RGBA, GL_UNSIGNED_BYTE, nodeImage.data() );

	glGenerateTextureMipmap( m_nodeTextureID );
}

void SimulationView::CleanTextures() const
{
	glDeleteTextures( 1, &m_nodeTextureID );
	glDeleteTextures( 1, &m_sphereTextureID );
}

void SimulationView::InitSimulation() {
	simulationManager.initSimulation(numberOfParticles, presetType, positionType, velocityType, massType, sizeType);
}

void SimulationView::InitImGuiSettings() {
	io = ImGui::GetIO();

	// Size
	windowWidth = 430.0f;
	windowHeight = io.DisplaySize.y;
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

	// Position
	ImGui::SetNextWindowPos(ImVec2(0, 0)); // Left side

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Font
	ImFontConfig font_cfg;
	font_cfg.FontNo = 0;
	font_cfg.OversampleH = 3;
	font_cfg.OversampleV = 3;
	font_cfg.PixelSnapH = true;
	font_cfg.GlyphOffset = ImVec2(0.0f, -1.0f);
	io.Fonts->AddFontFromFileTTF("Assets/Inter.ttc", 15.0f, &font_cfg);

	// Base Background
	colors[ImGuiCol_WindowBg] = ImVec4(0.0745f, 0.0745f, 0.0745f, 0.7f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);

	// Window Title Bar
	colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);

	// Text
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

	// Headers
	colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);

	// Buttons
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);

	// Frame BGs
	colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
	colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);

	// Separators & Borders
	colors[ImGuiCol_Border] = ImVec4(0.11f, 0.11f, 0.11f, 1.0f);
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);

	// Resize Grips
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.25f, 0.25f, 0.25f, 0.7f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.45f, 0.45f, 0.45f, 0.9f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.0f);

	// Rounding
	style.WindowRounding = 6.0f;
	style.FrameRounding = 4.0f;
	style.GrabRounding = 4.0f;
	style.ScrollbarRounding = 6.0f;
}

bool SimulationView::Init()
{
	simulationManager.initSettings();
	InitSimulation();

	SetupDebugCallback();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	InitShaders();
	InitGeometry();
	InitTextures();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); 

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_camera.SetView(
		DEFAULT_CAMERA_POSITION,  // From
		DEFAULT_CAMERA_TARGET,   // To
		DEFAULT_CAMERA_WORLDUP);  // Up

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

	// Light settings
	lightPos = simulationManager.settings.getLightPos();
	lightConstantAttenuation = simulationManager.settings.getLightConstantAttenuation();
	lightLinearAttenuation = simulationManager.settings.getLightLinearAttenuation();
	lightQuadraticAttenuation = simulationManager.settings.getLightQuadraticAttenuation();

	// Display settings
	cameraDistance = m_cameraManipulator.GetDistance();
	scrollZoomSpeed = m_cameraManipulator.GetSpeed();
	scaleFactor = simulationManager.settings.getScaleFactor();
	isForceColor = simulationManager.settings.getIsForceColor();
	minForceColor = simulationManager.settings.getMinForceColor();
	maxForceColor = simulationManager.settings.getMaxForceColor();

	// Simulation settings
	simulationSpeed = simulationManager.settings.getSimulationSpeed();
	theta = simulationManager.settings.getTheta();
	epsilon = simulationManager.settings.getEpsilon();
	minWorldBound = simulationManager.getMinWorldBound();
	maxWorldBound = simulationManager.getMaxWorldBound();
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

	// Orphan the old buffer and replace it with the new particles
	glNamedBufferData(instanceVBO, sizeof(Particle) * simulationManager.particles.size(), simulationManager.particles.data(), GL_DYNAMIC_DRAW);
}

void SimulationView::Render()
{
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Uniforms
	glProgramUniform1i(m_programID, ul(m_programID, "isSingleObject"), false);
	glProgramUniform1i(m_programID, ul(m_programID, "colorType"), isForceColor ? 1 : 0);
	glProgramUniform1f(m_programID, ul(m_programID, "scaleFactor"), scaleFactor);
	glProgramUniform1f(m_programID, ul(m_programID, "minVal"), minForceColor);
	glProgramUniform1f(m_programID, ul(m_programID, "maxVal"), maxForceColor);
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

	glBindVertexArray(m_sphereGPU.vaoID);

	// Draw instanced geometry
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glDrawElementsInstanced(GL_TRIANGLES, m_sphereGPU.count, GL_UNSIGNED_INT, nullptr, simulationManager.particles.size());

	// Draw individual if necessary
	if (spawnParticle_show) {
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);

		glProgramUniform1i(m_programID, ul(m_programID, "isSingleObject"), true);
		glProgramUniform3fv(m_programID, ul(m_programID, "position"), 1, glm::value_ptr(spawnParticle_position));
		glProgramUniform1f(m_programID, ul(m_programID, "scale"), spawnParticle_size);
		glProgramUniform1i(m_programID, ul(m_programID, "colorType"), 2);
		glDrawElements(GL_TRIANGLES,
			m_sphereGPU.count,
			GL_UNSIGNED_INT,
			nullptr);
	}

	// Nodes
	if (showOctree) {
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);

		glBindTextureUnit(0, m_nodeTextureID);
		glBindSampler(0, m_SamplerID);
		glBindVertexArray(m_nodeGPU.vaoID);

		glProgramUniform1i(m_programID, ul(m_programID, "isSingleObject"), true); // No instanced drawing
		glProgramUniform1i(m_programID, ul(m_programID, "colorType"), 0); // Fixed color
		glProgramUniform1f(m_programID, ul(m_programID, "scaleFactor"), 0.005f); // Fixed scale
		glProgramUniform4fv(m_programID, ul(m_programID, "lightPos"), 1, glm::value_ptr(glm::vec4(0.5f))); // Fixed constant light
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (int i = 0; i < simulationManager.octree.nodes.size(); i++) {
			if (simulationManager.octree.nodes[i].getMass() == 0) continue;

			glProgramUniform3fv(m_programID, ul(m_programID, "position"), 1, glm::value_ptr(simulationManager.octree.nodes[i].octant.getCenter()));
			glProgramUniform1f(m_programID, ul(m_programID, "scale"), simulationManager.octree.nodes[i].octant.getSize()*4);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeEBO);
			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);
		}
	}

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

	// Constant alpha
	float alpha = 1.0f;

	// Pulsate if message is new
	if (messageTime < 1.5f) {
		alpha = (sin(messageTime * 30.0f) + 1.0f) * 0.25f + 0.5f;
	}
	// Clear message after a while
	else if (messageTime > 5.0f) {
		message = "";
	}

	// Update coloring and positioning
	ImVec4 pulsatingColor = ImVec4(r, g, b, alpha);
	ImGui::PushStyleColor(ImGuiCol_Text, pulsatingColor);

	// Positioning
	float horizontalPadding = 10.0f;
	float verticalPadding = 20.0f;
	float wrapWidth = windowWidth - horizontalPadding;
	ImVec2 textSize = ImGui::CalcTextSize(message.c_str(), NULL, false, wrapWidth);
	float textYPos = windowHeight - textSize.y - verticalPadding;
	ImGui::SetCursorPos(ImVec2(horizontalPadding, textYPos));

	// Display text
	ImGui::PushTextWrapPos(wrapWidth);
	ImGui::TextWrapped(message.c_str());
	ImGui::PopTextWrapPos();

	// Restore the default text color
	ImGui::PopStyleColor();
}

void SimulationView::CollectSettingsFiles() {
	// Ensure directories exist before saving or loading
	std::filesystem::create_directories("UserData");
	std::filesystem::create_directories("UserData/Settings");

	// Collect all .stg files in the "UserData/Settings" folder
	availableSettingsFiles.clear();
	availableSettingsFiles.push_back("");
	for (const auto& entry : std::filesystem::directory_iterator("UserData/Settings")) {
		if (entry.is_regular_file() && entry.path().extension() == ".stg") {
			availableSettingsFiles.push_back(entry.path().stem().string());
		}
	}
}

void SimulationView::SaveSettingsUI() {
	ImGui::SeparatorText("Save settings");
	ImGui::InputText("##SettingsFilename", saveSettingsFileName, IM_ARRAYSIZE(saveSettingsFileName));
	ImGui::SameLine();
	if (ImGui::Button("Save Settings")) {
		try {
			if (strlen(saveSettingsFileName) > 0) {
				std::string filePath = "UserData/Settings/" + std::string(saveSettingsFileName) + ".stg";

				if (std::filesystem::exists(filePath)) {
					UpdateMessage("File already exists. Please choose a different name.", glm::vec3(1.0f, 0.0f, 0.0f));
				}
				else {
					simulationManager.saveSettings(filePath);
					UpdateMessage("Settings saved as " + std::string(saveSettingsFileName) + ".", glm::vec3(1.0f));
					saveSettingsFileName[0] = '\0';
				}
			}
			else {
				UpdateMessage("You must enter a name for the save first!", glm::vec3(1.0f, 0.0f, 0.0f));
			}
		}
		catch (const std::exception& e) {
			UpdateMessage("Error while saving settings: " + std::string(e.what()) + ".", glm::vec3(1.0f, 0.0f, 0.0f));
		}
		catch (...) {
			UpdateMessage("An unknown error occured while saving settings.", glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}
}

void SimulationView::LoadSettingsUI() {
	ImGui::SeparatorText("Load settings");
	if (!availableSettingsFiles.empty()) {
		// Convert availableFiles to const char* array for Combo box
		std::vector<const char*> fileNames;
		for (const auto& file : availableSettingsFiles) {
			fileNames.push_back(file.c_str());
		}

		// Display combo box with file names
		if (ImGui::BeginCombo("##SettingsFiles", selectedSettingsFileIndex > 0 ? availableSettingsFiles[selectedSettingsFileIndex].c_str() : "Select a file")) {
			for (int i = 0; i < availableSettingsFiles.size(); ++i) {
				bool isSelected = (i == selectedSettingsFileIndex);
				if (ImGui::Selectable(availableSettingsFiles[i].c_str(), isSelected)) {
					selectedSettingsFileIndex = i;
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
				if (selectedSettingsFileIndex > 0) {
					std::string filePath = "UserData/Settings/" + availableSettingsFiles[selectedSettingsFileIndex] + ".stg";
					simulationManager.loadSettings(filePath);
					UpdateMessage("Settings loaded from " + availableSettingsFiles[selectedSettingsFileIndex] + ".", glm::vec3(1.0f));
					selectedSettingsFileIndex = -1;
				}
				else {
					UpdateMessage("You must choose a save to load!", glm::vec3(1.0f, 0.0f, 0.0f));
				}
			}
			catch (const std::exception& e) {
				UpdateMessage("Error while loading settings: " + std::string(e.what()) + ".", glm::vec3(1.0f, 0.0f, 0.0f));
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

void SimulationView::CollectParticlesFiles() {
	// Ensure directories exist before saving or loading
	std::filesystem::create_directories("UserData");
	std::filesystem::create_directories("UserData/Particles");

	// Collect all .csv files in the "UserData/Particles" folder
	availableParticlesFiles.clear();
	availableParticlesFiles.push_back("");
	for (const auto& entry : std::filesystem::directory_iterator("UserData/Particles")) {
		if (entry.is_regular_file() && entry.path().extension() == ".csv") {
			availableParticlesFiles.push_back(entry.path().stem().string());
		}
	}
}

void SimulationView::SaveParticlesUI() {
	ImGui::SeparatorText("Save particles");
	ImGui::InputText("##ParticlesFilename", saveParticlesFileName, IM_ARRAYSIZE(saveParticlesFileName));
	ImGui::SameLine();
	if (ImGui::Button("Save Particles")) {
		try {
			if (strlen(saveParticlesFileName) > 0) {
				std::string filePath = "UserData/Particles/" + std::string(saveParticlesFileName) + ".csv";

				if (std::filesystem::exists(filePath)) {
					UpdateMessage("File already exists. Please choose a different name.", glm::vec3(1.0f, 0.0f, 0.0f));
				}
				else {
					simulationManager.saveParticles(filePath);
					UpdateMessage("Particles saved as " + std::string(saveParticlesFileName) + ".", glm::vec3(1.0f));
					saveParticlesFileName[0] = '\0';
				}
			}
			else {
				UpdateMessage("You must enter a name for the save first!", glm::vec3(1.0f, 0.0f, 0.0f));
			}
		}
		catch (const std::exception& e) {
			UpdateMessage("Error while saving particles: " + std::string(e.what()) + ".", glm::vec3(1.0f, 0.0f, 0.0f));
		}
		catch (...) {
			UpdateMessage("An unknown error occured while saving particles.", glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}
}

void SimulationView::LoadParticlesUI() {
	ImGui::SeparatorText("Load particles");
	if (!availableParticlesFiles.empty()) {
		// Convert availableFiles to const char* array for Combo box
		std::vector<const char*> fileNames;
		for (const auto& file : availableParticlesFiles) {
			fileNames.push_back(file.c_str());
		}

		// Display combo box with file names
		if (ImGui::BeginCombo("##ParticlesFiles", selectedParticlesFileIndex > 0 ? availableParticlesFiles[selectedParticlesFileIndex].c_str() : "Select a file")) {
			for (int i = 0; i < availableParticlesFiles.size(); ++i) {
				bool isSelected = (i == selectedParticlesFileIndex);
				if (ImGui::Selectable(availableParticlesFiles[i].c_str(), isSelected)) {
					selectedParticlesFileIndex = i;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load Particles")) {
			try {
				if (selectedParticlesFileIndex > 0) {
					std::string filePath = "UserData/Particles/" + availableParticlesFiles[selectedParticlesFileIndex] + ".csv";
					simulationManager.loadParticles(filePath);
					UpdateMessage("Particles loaded from " + availableParticlesFiles[selectedParticlesFileIndex] + ".", glm::vec3(1.0f));
					selectedParticlesFileIndex = -1;
				}
				else {
					UpdateMessage("You must choose a save to load!", glm::vec3(1.0f, 0.0f, 0.0f));
				}
			}
			catch (const std::exception& e) {
				UpdateMessage("Error while loading particles: " + std::string(e.what()) + ".", glm::vec3(1.0f, 0.0f, 0.0f));
			}
			catch (...) {
				UpdateMessage("An unknown error occured while loading particles.", glm::vec3(1.0f, 0.0f, 0.0f));
			}
		}
	}
	else {
		ImGui::Text("No saved particles found.");
	}
}

void SimulationView::SetCameraPresetView(CameraPresets direction) {
	float distance = glm::distance(m_camera.GetEye(), m_camera.GetAt());
	glm::vec3 newCameraPosition = m_camera.GetAt();
	glm::vec3 lookTarget = m_camera.GetAt();
	glm::vec3 worldUp = m_camera.GetWorldUp();

	switch (direction)
	{
	case CAMERA_TOP:
		newCameraPosition.x += 0.005f;
		newCameraPosition.y += distance;
		break;
	case CAMERA_BOTTOM:
		newCameraPosition.x -= 0.005f;
		newCameraPosition.y -= distance;
		break;
	case CAMERA_FRONT:
		newCameraPosition.z += distance;
		break;
	case CAMERA_BACK:
		newCameraPosition.z -= distance;
		break;
	case CAMERA_LEFT:
		newCameraPosition.x -= distance;
		break;
	case CAMERA_RIGHT:
		newCameraPosition.x += distance;
		break;
	case CAMERA_CENTER:
		newCameraPosition = m_camera.GetEye();
		lookTarget = glm::vec3(0);
		break;
	case CAMERA_DEFAULT:
		newCameraPosition = DEFAULT_CAMERA_POSITION;
		lookTarget = DEFAULT_CAMERA_TARGET;
		worldUp = DEFAULT_CAMERA_WORLDUP;
		break;
	default:
		// Don't change anything
		newCameraPosition = m_camera.GetEye();
		break;
	}

	m_camera.SetView(newCameraPosition, lookTarget, worldUp);
	m_cameraManipulator.SetCamera(&m_camera);
}

void SimulationView::ShowCameraSettings() {
	float buttonWidth = 50.0f; 
	float buttonHeight = 20.0f;
	float spacing = 5.0f;     

	ImVec2 pos = ImVec2(spacing, spacing);

	ImGui::BeginChild("Default directions", ImVec2(3 * (buttonWidth + spacing) + spacing, 4 * (buttonHeight + spacing) + spacing), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::SetCursorPos(ImVec2(pos.x + buttonWidth + spacing, pos.y));
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
	if (ImGui::Button("Top", ImVec2(buttonWidth, buttonHeight))) {
		SetCameraPresetView(CAMERA_TOP);
	}

	ImGui::SetCursorPos(ImVec2(pos.x, pos.y + buttonHeight + spacing));
	if (ImGui::Button("Left", ImVec2(buttonWidth, buttonHeight))) {
		SetCameraPresetView(CAMERA_LEFT);
	}

	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(pos.x + buttonWidth + spacing, pos.y + buttonHeight + spacing));
	if (ImGui::Button("Front", ImVec2(buttonWidth, buttonHeight))) {
		SetCameraPresetView(CAMERA_FRONT);
	}

	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(pos.x + 2 * (buttonWidth + spacing), pos.y + buttonHeight + spacing));
	if (ImGui::Button("Right", ImVec2(buttonWidth, buttonHeight))) {
		SetCameraPresetView(CAMERA_RIGHT);
	}

	ImGui::SetCursorPos(ImVec2(pos.x + buttonWidth + spacing, pos.y + 2 * (buttonHeight + spacing)));
	if (ImGui::Button("Bottom", ImVec2(buttonWidth, buttonHeight))) {
		SetCameraPresetView(CAMERA_BOTTOM);
	}

	ImGui::SetCursorPos(ImVec2(pos.x + buttonWidth + spacing, pos.y + 3 * (buttonHeight + spacing)));
	if (ImGui::Button("Back", ImVec2(buttonWidth, buttonHeight))) {
		SetCameraPresetView(CAMERA_BACK);
	}
	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginGroup();

	float itemWidthInGroup = ImGui::GetContentRegionAvail().x * 0.5f;

	if (ImGui::Button("Look at center", ImVec2(itemWidthInGroup, 0))) {
		SetCameraPresetView(CAMERA_CENTER);
	}

	if (ImGui::Button("Back to default", ImVec2(itemWidthInGroup, 0))) {
		SetCameraPresetView(CAMERA_DEFAULT);
	}

	ImGui::PushItemWidth(itemWidthInGroup);

	if (ImGui::SliderFloat("Camera distance", &cameraDistance, m_cameraManipulator.GetMinDistance(), m_cameraManipulator.GetMaxDistance()))
		m_cameraManipulator.SetDistance(cameraDistance);
	
	if (ImGui::DragFloat("Scroll zoom speed", &scrollZoomSpeed, 0.5f, m_cameraManipulator.GetMinSpeed(), m_cameraManipulator.GetMaxSpeed())) {
		scrollZoomSpeed = glm::clamp(scrollZoomSpeed, m_cameraManipulator.GetMinSpeed(), m_cameraManipulator.GetMaxSpeed());
		m_cameraManipulator.SetSpeed(scrollZoomSpeed);
	}

	ImGui::PopItemWidth();
	ImGui::EndGroup();
}

void SimulationView::ShowLightTypeSettings() {
	float spacing = 5.0f;
	float buttonWidth = ImGui::GetContentRegionAvail().x * 0.333f - spacing / 2.0f;

	int lightType = 0; // Constant
	if (lightPos.w == 0.0f)
		lightType = 1; // Directional light
	else if (lightPos.w == 1.0f)
		lightType = 2; // Spot light

	auto ToggleButton = [&](const char* label, int type) {
		if (lightType == type) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.50f, 0.50f, 0.50f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
		}

		if (ImGui::Button(label, ImVec2(buttonWidth, 0))) {
			lightType = type;
		}

		if (lightType == type) {
			ImGui::PopStyleColor(2);
		}
	};

	// Draw buttons
	ToggleButton("Constant", 0);
	ImGui::SameLine(0.0f, spacing);
	ToggleButton("Directional", 1);
	ImGui::SameLine(0.0f, spacing);
	ToggleButton("Point", 2);

	// Update light position
	if (lightType == 0 && lightPos.w != 0.5f) // Constant
	{
		lightPos.w = 0.5f;
		simulationManager.settings.setLightPos(lightPos);
	}
	else if (lightType == 1 && lightPos.w != 0.0f) // Directional light
	{
		lightPos = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
		simulationManager.settings.setLightPos(lightPos);
	}
	else if (lightType == 2 && lightPos.w != 1.0f) // Spot light
	{
		lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		simulationManager.settings.setLightPos(lightPos);
	}
}

void SimulationView::ShowLightParameterSettings() {
	if (lightPos.w == 0.5f) // Constant
	{
		ImGui::Text("There are no parameters for this light type.");
	}
	else if (lightPos.w == 0.0f) // Directional light
	{
		glm::vec3 dir = glm::vec3(lightPos);
		if (ImGui::DragFloat3("Light Direction", glm::value_ptr(dir), 0.05f, -1.0f, 1.0f)) {
			if (dir != glm::vec3(0.0f))
				dir = glm::normalize(dir);
			else dir = glm::vec3(0.f, -1.f, 0.f);
			lightPos = glm::vec4(dir, 0.0f);
			simulationManager.settings.setLightPos(lightPos);
		}
	}
	else if (lightPos.w >= 1.f) // Spot light
	{
		if (ImGui::DragFloat3("Light Position", glm::value_ptr(lightPos), 0.1f, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound())) {
			lightPos = glm::clamp(lightPos, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound());
			simulationManager.settings.setLightPos(lightPos);
		}

		if (ImGui::DragFloat("Constant Att.", &lightConstantAttenuation, 0.05f, simulationManager.settings.getMinLightConstantAttenuation(), simulationManager.settings.getMaxLightConstantAttenuation())) {
			lightConstantAttenuation = glm::clamp(lightConstantAttenuation, simulationManager.settings.getMinLightConstantAttenuation(), simulationManager.settings.getMaxLightConstantAttenuation());
			simulationManager.settings.setLightConstantAttenuation(lightConstantAttenuation);
		}
		if (ImGui::DragFloat("Linear Att.", &lightLinearAttenuation, 0.05f, simulationManager.settings.getMinLightLinearAttenuation(), simulationManager.settings.getMaxLightLinearAttenuation())) {
			lightLinearAttenuation = glm::clamp(lightLinearAttenuation, simulationManager.settings.getMinLightLinearAttenuation(), simulationManager.settings.getMaxLightLinearAttenuation());
			simulationManager.settings.setLightLinearAttenuation(lightLinearAttenuation);
		}
		if (ImGui::DragFloat("Quadratic Att.", &lightQuadraticAttenuation, 0.05f, simulationManager.settings.getMinLightQuadraticAttenuation(), simulationManager.settings.getMaxLightQuadraticAttenuation())) {
			lightQuadraticAttenuation = glm::clamp(lightQuadraticAttenuation, simulationManager.settings.getMinLightQuadraticAttenuation(), simulationManager.settings.getMaxLightQuadraticAttenuation());
			simulationManager.settings.setLightQuadraticAttenuation(lightQuadraticAttenuation);
		}
	}
}

void SimulationView::ShowSpawnParticleSettings() {
	if (ImGui::DragFloat3("Position", glm::value_ptr(spawnParticle_position), 10.0f, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound()))
		spawnParticle_position = glm::clamp(spawnParticle_position, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound());
	if (ImGui::DragFloat3("Velocity", glm::value_ptr(spawnParticle_velocity), 1.0f, -100.0f, 100.0f))
		spawnParticle_velocity = glm::clamp(spawnParticle_velocity, -100.0f, 100.0f);
	if (ImGui::DragFloat3("Color", glm::value_ptr(spawnParticle_color), 0.05f, 0.0f, 1.0f))
		spawnParticle_color = glm::clamp(spawnParticle_color, 0.0f, 1.0f);
	if (ImGui::DragFloat("Mass", &spawnParticle_mass, 1.0f, 0.001f, 1000000.0f))
		spawnParticle_mass = glm::clamp(spawnParticle_mass, 0.001f, 1000000.0f);
	if (ImGui::DragFloat("Size", &spawnParticle_size, 1.0f, 0.1f, 100.0f))
		spawnParticle_size = glm::clamp(spawnParticle_size, 0.1f, 100.0f);
	ImGui::Checkbox("Movable", &spawnParticle_movable);

	if (ImGui::Button("Spawn")) {
		simulationManager.addParticle(
			glm::vec4(spawnParticle_position, spawnParticle_mass),
			glm::vec4(spawnParticle_velocity, spawnParticle_size),
			glm::vec4(0.0f),
			glm::vec4(spawnParticle_color, spawnParticle_movable ? 1 : 0));
		if (currentNumberOfParticles >= threshold_numberOfParticles)
			UpdateMessage("Warning: adding too many particles might result in lower performance!", glm::vec3(1.0f, 1.0f, 0.0f));
	}
}

void SimulationView::ShowSpawnGroupPositionSettings(PositionType positionType) {
	switch (positionType) {
	case POSITION_RANDOM:
		if (ImGui::DragFloat3("Minimum position##Group", glm::value_ptr(group_cubeMin), 10.0f, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound()))
			group_cubeMin = glm::clamp(group_cubeMin, glm::vec3(simulationManager.getMinWorldBound()), group_cubeMax);
		if (ImGui::DragFloat3("Maximum position##Group", glm::value_ptr(group_cubeMax), 10.0f, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound()))
			group_cubeMax = glm::clamp(group_cubeMax, group_cubeMin, glm::vec3(simulationManager.getMaxWorldBound()));
		break;
	case POSITION_SPHERE:
		if (ImGui::DragFloat3("Center##Group", glm::value_ptr(group_sphereCenter), 10.0f, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound()))
			group_sphereCenter = glm::clamp(group_sphereCenter, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound());
		if (ImGui::DragFloat("Minimum radius##Group", &group_sphereRadiusMin, 10.0f, 0.001f, group_sphereRadiusMax))
			group_sphereRadiusMin = glm::clamp(group_sphereRadiusMin, 0.001f, group_sphereRadiusMax);
		if (ImGui::DragFloat("Maximum radius##Group", &group_sphereRadiusMax, 10.0f, group_sphereRadiusMin, simulationManager.getMaxWorldBound()))
			group_sphereRadiusMax = glm::clamp(group_sphereRadiusMax, group_sphereRadiusMin, simulationManager.getMaxWorldBound());
		break;
	case POSITION_DISK:
		if (ImGui::DragFloat3("Center##Group", glm::value_ptr(group_sphereCenter), 10.0f, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound()))
			group_sphereCenter = glm::clamp(group_sphereCenter, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound());
		if (ImGui::DragFloat("Minimum radius##Group", &group_sphereRadiusMin, 10.0f, 0.001f, group_sphereRadiusMax))
			group_sphereRadiusMin = glm::clamp(group_sphereRadiusMin, 0.001f, group_sphereRadiusMax);
		if (ImGui::DragFloat("Maximum radius##Group", &group_sphereRadiusMax, 10.0f, group_sphereRadiusMin, simulationManager.getMaxWorldBound()))
			group_sphereRadiusMax = glm::clamp(group_sphereRadiusMax, group_sphereRadiusMin, simulationManager.getMaxWorldBound());
		break;
	case POSITION_GRID_2D:
		if (ImGui::DragFloat3("Minimum position##Group", glm::value_ptr(group_cubeMin), 10.0f, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound())) {
			group_cubeMax.y = group_cubeMin.y;
			group_cubeMin = glm::clamp(group_cubeMin, glm::vec3(simulationManager.getMinWorldBound()), group_cubeMax);
		}
		if (ImGui::DragFloat3("Maximum position##Group", glm::value_ptr(group_cubeMax), 10.0f, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound())) {
			group_cubeMin.y = group_cubeMax.y;
			group_cubeMax = glm::clamp(group_cubeMax, group_cubeMin, glm::vec3(simulationManager.getMaxWorldBound()));
		}
		break;
	case POSITION_GRID_3D:
		if (ImGui::DragFloat3("Minimum position##Group", glm::value_ptr(group_cubeMin), 10.0f, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound()))
			group_cubeMin = glm::clamp(group_cubeMin, glm::vec3(simulationManager.getMinWorldBound()), group_cubeMax);
		if (ImGui::DragFloat3("Maximum position##Group", glm::value_ptr(group_cubeMax), 10.0f, simulationManager.getMinWorldBound(), simulationManager.getMaxWorldBound()))
			group_cubeMax = glm::clamp(group_cubeMax, group_cubeMin, glm::vec3(simulationManager.getMaxWorldBound()));
		break;
	default:
		break;
	}
}

void SimulationView::ShowSpawnGroupVelocitySettings(VelocityType velocityType) {
	switch (velocityType) {
	case VELOCITY_ORBIT:
		if (ImGui::DragFloat("Velocity scale##Group", &group_velocityScale, 0.1f, -5.0f, 5.0f))
			group_velocityScale = glm::clamp(group_velocityScale, -5.0f, 5.0f);
		if (ImGui::DragFloat("Center mass##Group", &group_centerMass, 0.1f, 0.0f, 1000000.0f))
			group_centerMass = glm::clamp(group_centerMass, 0.0f, 1000000.0f);
		break;
	case VELOCITY_RANDOM:
		if (ImGui::DragFloat3("Minimum velocity##Group", glm::value_ptr(group_velocityRandomMin), 1.0f, -1000.0f, 1000.0f)) {
			group_velocityRandomMin = glm::clamp(group_velocityRandomMin, glm::vec3(-1000.0f), group_velocityRandomMax);
		}
		if (ImGui::DragFloat3("Maximum velocity##Group", glm::value_ptr(group_velocityRandomMax), 1.0f, -1000.0f, 1000.0f)) {
			group_velocityRandomMax = glm::clamp(group_velocityRandomMax, group_velocityRandomMin, glm::vec3(1000.0f));
		}
		break;
	default:
		break;
	}
}

void SimulationView::ShowSpawnGroupMassSettings(MassType massType) {
	switch (massType)
	{
	case MASS_CONSTANT:
		if (ImGui::DragFloat("Mass value##Group", &group_massValue, 1.0f, 0.1f, 100000.0f))
			group_massValue = glm::clamp(group_massValue, 0.1f, 100000.0f);
		break;
	case MASS_RANDOM:
		if (ImGui::DragFloat("Minimum value##GroupMass", &group_massRandomMin, 1.0f, 0.1f, group_massRandomMax))
			group_massRandomMin = glm::clamp(group_massRandomMin, 0.1f, group_massRandomMax);
		if (ImGui::DragFloat("Maximum value##GroupMass", &group_massRandomMax, 1.0f, group_massRandomMin, 10000.0f))
			group_massRandomMax = glm::clamp(group_massRandomMax, group_massRandomMin, 10000.0f);
		break;
	default:
		break;
	}
}

void SimulationView::ShowSpawnGroupSizeSettings(SizeType sizeType) {
	switch (sizeType)
	{
	case SIZE_CONSTANT:
		if (ImGui::DragFloat("Size value##Group", &group_sizeValue, 0.1f, 0.1f, 100.0f))
			group_sizeValue = glm::clamp(group_sizeValue, 0.1f, 100.0f);
		break;
	case SIZE_RANDOM:
		if (ImGui::DragFloat("Minimum value##GroupSize", &group_sizeRandomMin, 0.1f, 0.1f, group_sizeRandomMax))
			group_sizeRandomMin = glm::clamp(group_sizeRandomMin, 0.1f, group_sizeRandomMax);
		if (ImGui::DragFloat("Maximum value##GroupSize", &group_sizeRandomMax, 0.1f, group_sizeRandomMin, 100.0f))
			group_sizeRandomMax = glm::clamp(group_sizeRandomMax, group_sizeRandomMin, 100.0f);
		break;
	default:
		break;
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
		ImGui::SeparatorText("Light type");
		ShowLightTypeSettings();
		ImGui::SeparatorText("Parameters");
		ShowLightParameterSettings();
	}

	// Display settings
	if (ImGui::CollapsingHeader("Display settings")) {
		ImGui::SeparatorText("Camera settings");
		ShowCameraSettings();

		ImGui::SeparatorText("Other display settings");
		if (ImGui::DragFloat("Particle size", &scaleFactor, 0.005f, simulationManager.settings.getMinScaleFactor(), simulationManager.settings.getMaxScaleFactor())) {
			scaleFactor = glm::clamp(scaleFactor, simulationManager.settings.getMinScaleFactor(), simulationManager.settings.getMaxScaleFactor());
			simulationManager.settings.setScaleFactor(scaleFactor);
		}

		ImGui::Checkbox("Display octree structure", &showOctree);

		if (ImGui::Checkbox("Color based on force", &isForceColor)) {
			if (isForceColor != simulationManager.settings.getIsForceColor())
				simulationManager.settings.setIsForceColor(isForceColor);
		}

		if (isForceColor) {
			if (ImGui::DragFloat("Minimum force", &minForceColor, 1.0f, 0.0f, maxForceColor)) {
				minForceColor = glm::clamp(minForceColor, 0.0f, maxForceColor);
				simulationManager.settings.setMinForceColor(minForceColor);
			}

			if (ImGui::DragFloat("Maximum force", &maxForceColor, 1.0f, minForceColor, std::numeric_limits<float>::max())) {
				maxForceColor = glm::clamp(maxForceColor, minForceColor, std::numeric_limits<float>::max());
				simulationManager.settings.setMaxForceColor(maxForceColor);
			}
		}
		
	}

	// Calculation settings
	if (ImGui::CollapsingHeader("Calculation settings")) {
		if (ImGui::DragFloat("Simulation speed", &simulationSpeed, 0.01f, simulationManager.settings.getMinSimulationSpeed(), simulationManager.settings.getMaxSimulationSpeed())) {
			simulationSpeed = glm::clamp(simulationSpeed, simulationManager.settings.getMinSimulationSpeed(), simulationManager.settings.getMaxSimulationSpeed());
			simulationManager.settings.setSimulationSpeed(simulationSpeed);
			if (simulationSpeed > threshold_simulationSpeed)
				UpdateMessage("Warning: too large value for simulation speed might result in lower simulation accuracy!", glm::vec3(1.0f, 1.0f, 0.0f));
		}
		
		if (ImGui::InputFloat("Theta", &theta, 0.05f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)) {
			simulationManager.settings.setTheta(theta);
			if (theta < threshold_lowerTheta)
				UpdateMessage("Warning: too small value for theta will result in significantly lower performance!", glm::vec3(1.0f, 1.0f, 0.0f));
			else if (theta > threshold_higherTheta)
				UpdateMessage("Warning: too large value for theta might result in lower simulation accuracy!", glm::vec3(1.0f, 1.0f, 0.0f));
		}

		if (ImGui::InputFloat("Epsilon", &epsilon, 0.05f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			simulationManager.settings.setEpsilon(epsilon);

		if (ImGui::InputFloat("World bound minimum", &minWorldBound, 100.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			simulationManager.setMinWorldBound(minWorldBound);

		if (ImGui::InputFloat("World bound maximum", &maxWorldBound, 100.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			simulationManager.setMaxWorldBound(maxWorldBound);
	}

	// Load/save settings
	if (ImGui::CollapsingHeader("Load/Save Settings")) {
		// Collect existing saves
		CollectSettingsFiles();

		// Save settings
		SaveSettingsUI();

		// Load settings
		LoadSettingsUI();

		// Default settings
		ImGui::SeparatorText("Default settings");
		if (ImGui::Button("Set back to default"))
			simulationManager.defaultSettings();
	}

	// Load/save particles
	if (ImGui::CollapsingHeader("Load/Save Particles")) {
		// Collect existing saves
		CollectParticlesFiles();

		// Save particles
		SaveParticlesUI();

		// Load particles
		LoadParticlesUI();
	}

	// Spawn particle
	if (ImGui::CollapsingHeader("Spawn particle")) {
		spawnParticle_show = true;
		ShowSpawnParticleSettings();
	}
	else
		spawnParticle_show = false;

	// Spawn group
	if (ImGui::CollapsingHeader("Spawn group")) {
		ImGui::SeparatorText("Parameters");
		if (ImGui::InputInt("Number of particles##Group", &group_numberOfParticles, 50, 1000, ImGuiInputTextFlags_EnterReturnsTrue)) {
			if (currentNumberOfParticles + group_numberOfParticles > threshold_numberOfParticles)
				UpdateMessage("Warning: adding too many particles might result in lower performance!", glm::vec3(1.0f, 1.0f, 0.0f));
		}
		
		ImGui::SeparatorText("Position settings");
		ShowEnumDropdown("Position Type##Group", POSITION_TYPE_NAMES, group_positionType);
		ShowSpawnGroupPositionSettings(group_positionType);

		ImGui::SeparatorText("Individual velocity settings");
		ShowEnumDropdown("Velocity Type##Group", VELOCITY_TYPE_NAMES, group_velocityType);
		ShowSpawnGroupVelocitySettings(group_velocityType);

		ImGui::SeparatorText("Group velocity settings");
		ImGui::Checkbox("Movable (center)", &group_movable);
		if (group_movable) {
			if (ImGui::DragFloat3("Group velocity##Group", glm::value_ptr(group_overallVelocity), 1.0f, -1000.0f, 1000.0f))
				group_overallVelocity = glm::clamp(group_overallVelocity, -1000.0f, 1000.0f);
		}

		ImGui::SeparatorText("Mass settings");
		ShowEnumDropdown("Mass Type##Group", MASS_TYPE_NAMES, group_massType);
		ShowSpawnGroupMassSettings(group_massType);

		ImGui::SeparatorText("Size settings");
		ShowEnumDropdown("Size Type##Group", SIZE_TYPE_NAMES, group_sizeType);
		ShowSpawnGroupSizeSettings(group_sizeType);

		ImGui::SeparatorText("Finalize");
		if (ImGui::Button("Add group")) {
			simulationManager.addGroup(group_numberOfParticles,
				group_positionType,
				group_cubeMin, group_cubeMax,
				group_sphereCenter, group_sphereRadiusMin, group_sphereRadiusMax,
				group_velocityType,
				group_velocityScale, group_centerMass,
				group_velocityRandomMin, group_velocityRandomMax,
				group_movable, group_overallVelocity,
				group_massType,
				group_massValue, group_massRandomMin, group_massRandomMax,
				group_sizeType,
				group_sizeValue, group_sizeRandomMin, group_sizeRandomMax);
		}
	}

	// New simulation
	if (ImGui::CollapsingHeader("New simulation")) {
		if (ImGui::InputInt("Number of particles", &numberOfParticles, 50, 1000, ImGuiInputTextFlags_EnterReturnsTrue)) {
			if (numberOfParticles > threshold_numberOfParticles)
				UpdateMessage("Warning: adding too many particles might result in lower performance!", glm::vec3(1.0f, 1.0f, 0.0f));
		}

		ShowEnumDropdown("Preset Type", PRESET_TYPE_NAMES, presetType);

		if (presetType == PRESET_CUSTOM) {
			ShowEnumDropdown("Position Type", POSITION_TYPE_NAMES, positionType);
			ShowEnumDropdown("Velocity Type", VELOCITY_TYPE_NAMES, velocityType);
			ShowEnumDropdown("Mass Type", MASS_TYPE_NAMES, massType);
			ShowEnumDropdown("Size Type", SIZE_TYPE_NAMES, sizeType);
		}

		if (ImGui::Button("Start New Simulation")) {
			InitSimulation();
		}
	}

	// Messages
	if (message != "")
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
