#include "PlayScene2.h"
#include "Game.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "Util.h"
#include "IMGUI_SDL/imgui_sdl.h"
#include "Renderer.h"
#include <iomanip>

PlayScene2::PlayScene2()
{
	PlayScene2::start();
}

PlayScene2::~PlayScene2() = default;

void PlayScene2::draw()
{
	// Background
	 TextureManager::Instance()->draw("Brick", 400, 300, 0, 255, true);
	drawDisplayList();
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene2::update()
{
	m_updateUI();
	updateDisplayList();

	updateGameObjects();
	updateLabels();
	checkCollision();

	calculateAverageBrickVel();
	calcAverageBrickMomentum();

}

void PlayScene2::clean()
{
	removeAllChildren();
}

void PlayScene2::handleEvents()
{
	auto& io = ImGui::GetIO();
	auto wheel = 0;

	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;
		case SDL_MOUSEMOTION:
			m_mousePosition.x = event.motion.x;
			m_mousePosition.y = event.motion.y;
			break;
		case SDL_MOUSEWHEEL:
			wheel = event.wheel.y;
			break;

		case SDL_TEXTINPUT:
			io.AddInputCharactersUTF8(event.text.text);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				TheGame::Instance()->quit();
				break;
			case SDLK_BACKQUOTE:
				m_displayUI = (m_displayUI) ? false : true;
				break;
			case SDLK_1:
				TheGame::Instance()->changeSceneState(PLAY_SCENE);
				ImGui::EndFrame();
				break;
			case SDLK_SPACE:
				m_pBrickSprite->pause = !m_pBrickSprite->pause;
				break;
			}
			{
				const int key = event.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (event.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			default:
				break;
			}
			{
				const int key = event.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (event.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			}
			break;
		default:
			break;
		}
	}
	io.DeltaTime = 1.0f / 60.0f;
	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);

	io.DisplaySize.x = 1280;
	io.DisplaySize.y = 720;

	m_ImGuiKeyMap();
	m_ImGuiSetStyle();
}

void PlayScene2::start()
{
	initGameObjects();
	initLabels(20, white);
	initMemberVariables();
	//Load Background
	TextureManager::Instance()->load("../Assets/textures/Pendulum.jpg", "Brick");

	m_pBallSprite->setBrickWidth(m_pBrickSprite->getWidth());
	m_pBallSprite->setBrickHeight(m_pBrickSprite->getHeight());

	m_updateUI();

}

void PlayScene2::checkGuiChangs()
{
			//GUI Button
		if (ImGui::Button("Begin Simulation"))
		{
			m_pBallSprite->setBeginSimulation(true);


		}
		ImGui::SameLine();
		if (ImGui::Button("Pause Simulation"))
		{
			m_pBallSprite->setBeginSimulation(false);
		}
		if (ImGui::Button("Reset Simulation"))
		{
			resetScene();
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset HighScore"))
		{
			m_pBallSprite->setHighScore(0);
		}

		if (ImGui::Checkbox("Vertical Paddle", &GuiSliderBools[0]))
		{
			if (GuiSliderBools[0] == false)
			{
				updateOrientation(HORIZONTAL);
			}
			else
			{
				updateOrientation(VERTICAL);
			}
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Gravity Enabled", &GuiSliderBools[1]))
		{
			if (GuiSliderBools[1])
			{
				m_pBallSprite->setGravityEnabled(true);
			}
			else
			{
				m_pBallSprite->setGravityEnabled(false);
			}
		}

		// Slider Settings
		if (ImGui::SliderFloat("Wall Absorbtion % ", &GuiSliderFloats[0], 0.1f, 1.0f));
		{
			m_pBallSprite->setWallAbsorbtion(GuiSliderFloats[0]);
		}
		if (ImGui::SliderFloat("Ball Weight ", &GuiSliderFloats[1], 0.2f, 5.0f));
		{
			m_pBallSprite->setMass(GuiSliderFloats[1]);
		}
		if (ImGui::SliderFloat("Brick Weight ", &GuiSliderFloats[2], 2.0f, 50.0f));
		{
			m_pBrickSprite->setMass(GuiSliderFloats[2]);
		}
		if (ImGui::SliderFloat("Ball Starting Velocity ", &GuiSliderFloats[3], 0.5f, 10.0f));
		{
			if (prevVelocity != GuiSliderFloats[3])
			{
				updateStartingVelocity();
			}
		}

}

void PlayScene2::m_ImGuiKeyMap()
{
	auto& io = ImGui::GetIO();

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
	io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;

	io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
	io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
	io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
	io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
	io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;
}

void PlayScene2::m_ImGuiSetStyle()
{

	auto& style = ImGui::GetStyle();

	style.Alpha = 0.8f;
	style.FrameRounding = 3.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void PlayScene2::m_updateUI()
{

	// Prepare Window Frame
	ImGui::NewFrame();

	ImGui::Begin("Settings ", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	// set window to top getLeft corner
	ImGui::SetWindowPos(ImVec2(0, 0), true);

	/*************************************************************************************************/
	/* MENU                                                                                          */
	/*************************************************************************************************/

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();
			ImGui::MenuItem("Exit", NULL, &m_exitApp);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (m_exitApp)
	{
		TheGame::Instance()->quit();
	}
	if (m_displayAbout)
	{
		ImGui::Begin("2D Collisions", &m_displayAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();
		ImGui::Text("Authors:");
		ImGui::Text("Eric Galway & Ian Chapman ");
		ImGui::End();
	}
	//ImGui::SameLine();
	checkGuiChangs();

	// Main Window End
	ImGui::End();
}

void PlayScene2::initLabels(int font, SDL_Color color)
{
	m_pHighScoreLabel = new Label("High Score: 0", "Consolas", font, color, glm::vec2(600.0f, 20.0f));
	m_pHighScoreLabel->setParent(this);
	addChild(m_pHighScoreLabel);

	m_pKeepUpScoreLabel = new Label("Current Score: 0", "Consolas", font, color, glm::vec2(600.0f, 50.0f));
	m_pKeepUpScoreLabel->setParent(this);
	addChild(m_pKeepUpScoreLabel);

	m_pBrickVelocityLabel = new Label("Brick Velocity: 0", "Consolas", font, color, glm::vec2(600.0f, 80.0f));
	m_pBrickVelocityLabel->setParent(this);
	addChild(m_pBrickVelocityLabel);

	m_pBallVelocityLabel = new Label("Ball Velocity: 0", "Consolas", font, color, glm::vec2(600.0f, 110.0f));
	m_pBallVelocityLabel->setParent(this);
	addChild(m_pBallVelocityLabel);

	m_pMaxSpeedLabel = new Label("Ball Max Speed: 0", "Consolas", font, color, glm::vec2(600.0f, 140.0f));
	m_pMaxSpeedLabel->setParent(this);
	addChild(m_pMaxSpeedLabel);

	m_pBallMomentumLabel = new Label("Ball Momentum: 0", "Consolas", font, color, glm::vec2(600.0f, 170.0f));
	m_pBallMomentumLabel->setParent(this);
	addChild(m_pBallMomentumLabel);

	m_pBrickMomentumLabel = new Label("Brick Avg Momentum: 0", "Consolas", font, color, glm::vec2(600.0f, 200.0f));
	m_pBrickMomentumLabel->setParent(this);
	addChild(m_pBrickMomentumLabel);

	m_pPPMLabel = new Label("Scale: " + std::to_string(m_PPM) + " PPM", "Consolas", font, color, glm::vec2(670.0f, 570.0f));
	m_pPPMLabel->setParent(this);
	addChild(m_pPPMLabel);
}

void PlayScene2::initMemberVariables()
{
	brickVelocityCounter = 0.0f;
	averageBrickVelocity = 0;
	avgBrickVelTemp = 0.0f;

	collisionCounter = 0;

	brickMomentumCounter =0;
	avgBrickMomentumTemp =0;
	avgerageBrickMomentum=0;

	maxSpeed = 0.0f;

	initGuiSliderVariables();
}

void PlayScene2::initGameObjects()
{
	m_pBrickSprite = new Brick();
	m_pBrickSprite->setParent(this);
	addChild(m_pBrickSprite);


	m_pBallSprite = new Ball();
	m_pBallSprite->setParent(this);
	addChild(m_pBallSprite);
}

void PlayScene2::initGuiSliderVariables()
{
	// Floats
	GuiSliderFloats[0] = m_pBallSprite->getWallAbsorbtion();
	GuiSliderFloats[1] = m_pBallSprite->getMass();
	GuiSliderFloats[2] = m_pBrickSprite->getMass();
	GuiSliderFloats[3] = 2.0f;

	//Bools
	GuiSliderBools[0] = false;
	GuiSliderBools[1] = false;
}

void PlayScene2::updateLabels()
{
	m_pBrickVelocityLabel->setText("Avg. Brick Velocity: " + std::to_string(averageBrickVelocity) + "m/s^2");

	if (m_pBallSprite->getCollisionJustHappened())
	{
		m_pBallVelocityLabel->setText("Ball Velocity: " + std::to_string(Util::magnitude(m_pBallSprite->getRigidBody()->velocity) / m_PPM) + "m/s^2");
		if (maxSpeed < (Util::magnitude(m_pBallSprite->getRigidBody()->velocity) / m_PPM))
		{
			maxSpeed = (Util::magnitude(m_pBallSprite->getRigidBody()->velocity) / m_PPM);
		}
		m_pBallMomentumLabel->setText("Ball Momentum: " + std::to_string(Util::magnitude(m_pBallSprite->getMomentum())) + "kg*m/s");
	}
	m_pBrickMomentumLabel->setText("Avg. Brick Momentum: " + std::to_string(avgerageBrickMomentum) + "kg*m/s");

	m_pKeepUpScoreLabel->setText("Current Score: " + std::to_string(m_pBallSprite->getKeepUpScore()));

	m_pHighScoreLabel->setText("High Score: " + std::to_string(m_pBallSprite->getHighScore()));

	m_pMaxSpeedLabel->setText("Ball Max Speed: " + std::to_string(maxSpeed) + "m/s");
}

void PlayScene2::updateGameObjects()
{
	m_pBrickSprite->brickMovement(m_mousePosition);
}

void PlayScene2::updateOrientation(BrickOrientation orientation)
{
	m_pBrickSprite->setOrientation(orientation);
	m_pBallSprite->setBrickOrientation(orientation);
	m_pBallSprite->setBrickWidth(m_pBrickSprite->getWidth());
	m_pBallSprite->setBrickHeight(m_pBrickSprite->getHeight());
}

void PlayScene2::checkCollision()
{
	if (collisionCounter >= 3)
	{
		if (CollisionManager::circleAABBCheck(m_pBallSprite, m_pBrickSprite) && m_pBallSprite->getBeginSimulation())
		{
			m_pBallSprite->setCollisionType(BRICK_COLLISION);
			m_pBallSprite->setBrickWeight(m_pBrickSprite->getMass());
			m_pBallSprite->setBrickVelocity(m_pBrickSprite->getRigidBody()->velocity);
			m_pBallSprite->setBrickPosition(m_pBrickSprite->getTransform()->position);
			collisionCounter = 0;
		}
	}
	collisionCounter++;
}

void PlayScene2::calculateAverageBrickVel()
{
	if (brickVelocityCounter < 59)
	{
		avgBrickVelTemp += Util::magnitude(m_pBrickSprite->getRigidBody()->velocity);
		brickVelocityCounter++;
	}
	else if (brickVelocityCounter >= 59)
	{
		brickVelocityCounter = 0;
		averageBrickVelocity = avgBrickVelTemp / 60.0f;
		avgBrickVelTemp = 0;
	}
}

void PlayScene2::calcAverageBrickMomentum()
{
	if (brickMomentumCounter < 59)
	{
		avgBrickMomentumTemp += Util::magnitude(m_pBrickSprite->getRigidBody()->velocity)*m_pBrickSprite->getMass();
		brickMomentumCounter++;
	}
	else 
	{
		brickMomentumCounter = 0;
		avgerageBrickMomentum = avgBrickMomentumTemp / 60.0f;
		avgBrickMomentumTemp = 0;
	}
}

void PlayScene2::resetScene()
{
	m_pBallSprite->getTransform()->position = glm::vec2(400.0f, 300.0f);
	m_pBallSprite->getRigidBody()->velocity = glm::vec2(2.5f, 2.5f);
	updateStartingVelocity();
	m_pBallSprite->setBeginSimulation(false);
	m_pBallSprite->setWallAbsorbtion(0.1f);
	m_pBallSprite->setHighScore(0);
	m_pBallSprite->setKeepUpScore(0);
	m_pBrickSprite->setOrientation(HORIZONTAL);
	m_pBallSprite->setBrickOrientation(HORIZONTAL);
	m_pBallSprite->setMass(2.0f);
	m_pBrickSprite->setMass(10.0f);
	initMemberVariables();
	maxSpeed = 0.0f;
	m_pBallSprite->setMomentum(glm::vec2(0.0f));
	m_pBallSprite->setCollisionJustHappened(true);
}

void PlayScene2::updateStartingVelocity()
{
		coordinates = Util::normalize(m_pBallSprite->getRigidBody()->velocity);
		m_pBallSprite->getRigidBody()->velocity = coordinates * GuiSliderFloats[3] * m_PPM;
		prevVelocity = GuiSliderFloats[3];
}
