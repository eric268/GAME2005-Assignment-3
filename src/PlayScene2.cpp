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
				m_pBrickSpite->pause = !m_pBrickSpite->pause;
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
	initLabels();
	initGuiSliderFloats();
	//Load Background
	TextureManager::Instance()->load("../Assets/textures/Brick2.jpg", "Brick");

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
		ImGui::SameLine();
		if (ImGui::Button("Reset Simulation"))
		{
			resetScene();
		}

		/*if (ImGui::Checkbox("text", &boolean))
		{
		}*/

		// Slider Settings
		if (ImGui::SliderFloat("Wall Absorbtion % ", &GuiSliderFloats[0], 0.1f, 0.9f));
		{
			m_pBallSprite->setWallAbsorbtion(GuiSliderFloats[0]);
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
		ImGui::Begin("Projectile Tragectory", &m_displayAbout, ImGuiWindowFlags_AlwaysAutoResize);
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

void PlayScene2::initLabels()
{
	m_pBrickVelocityLabel = new Label("Brick Velocity: ", "Consolas", 20, white, glm::vec2(600.0f, 30.0f));
	m_pBrickVelocityLabel->setParent(this);
	addChild(m_pBrickVelocityLabel);

	m_pPPM = new Label("Scale: " + std::to_string(m_PPM) + " PPM", "Consolas", 20, white, glm::vec2(670.0f, 570.0f));
	m_pPPM->setParent(this);
	addChild(m_pPPM);
}

void PlayScene2::initGameObjects()
{
	m_pBrickSpite = new Brick();
	m_pBrickSpite->setParent(this);
	addChild(m_pBrickSpite);

	m_pBallSprite = new Ball();
	m_pBallSprite->setParent(this);
	addChild(m_pBallSprite);
}

void PlayScene2::initGuiSliderFloats()
{
	GuiSliderFloats[0] = m_pBallSprite->getWallAbsorbtion();
}

void PlayScene2::updateLabels()
{
	m_pBrickVelocityLabel->setText("Brick Velocity: " + std::to_string(Util::magnitude(m_pBrickSpite->getRigidBody()->velocity)));
}

void PlayScene2::updateGameObjects()
{
	m_pBrickSpite->brickMovement(m_mousePosition);
}

void PlayScene2::checkCollision()
{
	if (collisionCounter >= 7)
	{
		if (CollisionManager::circleAABBCheck(m_pBallSprite, m_pBrickSpite))
		{
			m_pBallSprite->setCollisionType(BRICK_COLLISION);
			m_pBallSprite->setBrickWeight(m_pBrickSpite->getMass());
			m_pBallSprite->setBrickVelocity(m_pBrickSpite->getRigidBody()->velocity);
			m_pBallSprite->setBrickPosition(m_pBrickSpite->getTransform()->position);
			collisionCounter = 0;

		}
	}
	collisionCounter++;
	
}

void PlayScene2::resetScene()
{
	m_pBallSprite->getTransform()->position = glm::vec2(135.0f, 200.0f);
	m_pBallSprite->getRigidBody()->velocity = glm::vec2(250.0f, 250.0f);
	m_pBallSprite->setBeginSimulation(false);

}

void PlayScene2::changeScene()
{

}
