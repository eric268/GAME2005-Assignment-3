#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include "Renderer.h"
#include "IMGUI_SDL/imgui_sdl.h"
#include "Player.h"
#include "BulletPool.h"
#include <iomanip>
#include "SoundManager.h"
#include "Bullet.h"
#include "GameObject.h"

PlayScene::PlayScene()
{

	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("Space Background", 400, 300, 0, 255, true);

	drawDisplayList();

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	m_updateUI();
	SDL_GetTicks();
	float deltaTime = 1.0f / 60.f;
	updateDisplayList();
	checkCollision();
	bulletOnBulletCollision();
	checkPlayerBulletBounds();
	updateLabels();

	if (SDL_GetTicks() - bulletSpawnTimerStart >= bulletSpawnTimerDuration)
	{
		SpawnBullet();
	}

	std::vector<Bullet*> activeBullets = m_pPool->all;
	for (std::vector<Bullet*>::iterator myiter = activeBullets.begin(); myiter != activeBullets.end(); ++myiter)
	{
		Bullet* bullet = *myiter;
		if (bullet->active && bullet->getTransform()->position.y >= 650)
		{
			m_pPool->Despawn(bullet);
			break;
		}
	}

	//Printing Labels
/*m_pVelocityLabel->setText("Velocity: " + std::to_string(Util::magnitude(m_pLootCrate->getRigidBody()->velocity)/PPM));
m_pMassLabel->setText("Mass of Crate: = " + std::to_string(m_pLootCrate->getMass()) + "kg");
if (m_pLootCrate->getOnGround())
{
	m_pDistanceLabel->setText("Distance on ground = " + std::to_string(calculateDistanceOnGround()) + "m");
}
else
{
	m_pDistanceLabel->setText("Distance on ground = 0m");
}

if (m_pLootCrate->getOnGround())
{
	m_pAccelerationLabel->setText("Acceleration = " + std::to_string(m_pLootCrate->getRigidBody()->acceleration.x) + "m/s^2");
}
else
{
	m_pAccelerationLabel->setText("Acceleration = " + std::to_string(Util::magnitude(m_pLootCrate->getRigidBody()->acceleration)) + "m/s^2");
}
if (m_pLootCrate->getOnGround())
{
	m_pForceLabel->setText("Net Force = " + std::to_string(m_pLootCrate->calculateNetForce(0,m_pLootCrate->calculateForceK())) + "N");
}
else
{
	m_pForceLabel->setText("Net Force = " + std::to_string(m_pLootCrate->calculateNetForce(m_pLootCrate->calculateForceGX(), 0)) + "N");
}

m_pMaxSpeed->setText("Max Speed = " + std::to_string(m_pLootCrate->getMaxSpeed()) + "m/s^2");

m_pPPM->setText("Scale: " + std::to_string(PPM) + " PPM");

m_pFrictionCoefficient->setText("Friction coefficient = " + std::to_string(m_pLootCrate->getFrictionCoefficient()));*/

}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{

	auto& io = ImGui::GetIO();
	auto wheel = 0;

	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	{
		m_pPlayer->moveLeft();
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		m_pPlayer->moveRight();
	}
	else {
		m_pPlayer->stopMovingX();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
	{
		m_pPlayer->moveUp();
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
	{
		m_pPlayer->moveDown();
	}
	else {
		m_pPlayer->stopMovingY();
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		if (!m_pPlayer->getBulletFired())
		{
			m_pPlayerPool->FireBullet(m_pPlayer);
			m_pPlayer->setBulletFired(true);
			SoundManager::Instance().playSound("1", 0);
		}
		
	}
	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_SPACE))
	{
		m_pPlayer->setBulletFired(false);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE_2);
		ImGui::EndFrame();
	}


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
		case SDLK_1:

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

			}
		case SDLK_2:
			//This was making it change the scene for some reason
			//TheGame::Instance()->changeSceneState(PLAY_SCENE_2);
			//ImGui::EndFrame();
			break;
			break;
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

void PlayScene::start()
{

	TextureManager::Instance()->load("../Assets/textures/Space Background.png", "Space Background");
	SoundManager::Instance().load("../Assets/audio/Pew.wav", "1", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Collision.wav", "2", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/MeteorHit.wav", "3", SOUND_SFX);
	

	m_pPlayer = new Player();
	addChild(m_pPlayer);
	initGuiVariables();
	initLabels();
	bulletSpawnTimerDuration = 10000.0 / 12.0f;
	createBullets();

	bulletSpawnTimerStart = SDL_GetTicks();
	m_updateUI();

	// Player Sprite


	//If you want to be able to have a max of 30 bullets 

	
	


	// Label

	//m_pDistanceLabel = new Label("Distance", "Consolas", 20, black, glm::vec2(596.0f, 30.0f));
	//m_pDistanceLabel->setParent(this);
	//addChild(m_pDistanceLabel);

	//m_pForceLabel = new Label("Force: ", "Consolas", 20, black, glm::vec2(571.0f, 230.0f));
	//m_pForceLabel->setParent(this);
	//addChild(m_pForceLabel);

	//m_pPPM = new Label("Scale: "+std::to_string(PPM), "Consolas", 20, black, glm::vec2(690.0f, 580.0f));
	//m_pPPM->setParent(this);
	//addChild(m_pPPM);

	//m_pFrictionCoefficient = new Label("Friction coefficient: ", "Consolas", 20, black, glm::vec2(583.0f, 270.0f));
	//m_pFrictionCoefficient->setParent(this);
	//addChild(m_pFrictionCoefficient);

	resetSceneSettings(); // nn change definition of function!!!!

}

float PlayScene::getPPM()
{
	return PPM;
}

void PlayScene::checkCollision()
{
	for (std::vector<Bullet*>::iterator myiter = m_pPool->all.begin(); myiter != m_pPool->all.end(); ++myiter)
	{
		Bullet* bullet = *myiter;
		if (bullet->active)
		{
			if (CollisionManager::AABBCheck(bullet, m_pPlayer))
			{
				if (bullet->getPlaySound())
				{
					std::cout << "COLLIDING!!!!\n";
						
						bullet->setPlaySound(false);
						SoundManager::Instance().playSound("2", 0);
						//If you want bullet to disappear
						m_pPool->Despawn(bullet);
						if (m_currentScore > m_highScore)
						{
							m_highScore = m_currentScore;
							m_currentScore = 0;
						}
				}

				//Do something with ship as well
			}
		}
	}
}

void PlayScene::initGuiVariables()
{
	GuiBoolValues[0] = false; // Begin simulation
	GuiBoolValues[1] = false; // Change Scene
	GuiBoolValues[2] = true;  // Max 10 bullets
	GuiBoolValues[3] = false; // Max 20 bullets
	GuiBoolValues[4] = false; // Max 30 bullets

	GuiFloatValues[0] = 10.0f; //Number of bullets 
	GuiFloatValues[1] = 9.8f;  // gravity
	GuiFloatValues[2] = 5.0f;  //Player bullet speed
	GuiFloatValues[3] = 50.0f; //Player acceleration speed

	m_currentScore = 0;
	m_highScore = 0;
}

void PlayScene::beginSimulation()
{

}

void PlayScene::resetSceneSettings()
{


	//m_pLootCrate->setAtMaxSpeed(false);
	//m_pLootCrate->setMaxSpeed(0.0f);
	//m_pLootCrate->setRotateTarget(0.0f);
	//m_pLootCrate->getRigidBody()->velocity = glm::vec2(0.0f);
	//m_pLootCrate->getRigidBody()->acceleration = glm::vec2(0.0f); // Stop the crate
	//m_pLootCrate->setDeccelerationCalculated(false);
	//m_pLootCrate->setOnGround(false);

}

void PlayScene::resetCrateSettings()
{
	//m_pLootCrate->setAtMaxSpeed(false);
	//m_pLootCrate->setMaxSpeed(0.0f);
	//m_pLootCrate->setBeginSimulation(false);
	//m_pLootCrate->getTransform()->position = glm::vec2(adjustRampPosition.x, adjustRampHeight.y - m_lootBoxOffset);
	//m_pLootCrate->getRigidBody()->velocity = glm::vec2(0.0f);
	//m_pLootCrate->getRigidBody()->acceleration = glm::vec2(0.0f);
	//m_pLootCrate->setRotateTarget(0.0f);
	//m_pLootCrate->setDeccelerationCalculated(false);
	//m_pLootCrate->setOnGround(false);

}

void PlayScene::checkGuiChangs()
{
	if (ImGui::Button("Begin Simulation"))
	{
		GuiBoolValues[0] = true;
		setBulletBeginSim(true);
		m_pPlayer->setBeginSimulation(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause Simulation"))
	{
		GuiBoolValues[0] = false;
		setBulletBeginSim(false);
		m_pPlayer->setBeginSimulation(false);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset Simulation"))
	{
		GuiBoolValues[0] = false;
		for (std::vector<Bullet*>::iterator myiter = m_pPool->all.begin(); myiter != m_pPool->all.end(); ++myiter)
		{
			Bullet* bullet = *myiter;
			bullet->setBeginSimulation(false);
			if (bullet->active)
			{
				m_pPool->Despawn(bullet);
			}
		}
		m_pPlayer->setBeginSimulation(false);
		m_currentScore = 0;
		m_highScore = 0;
	}


	if (ImGui::Checkbox("10 Meteors Total", &GuiBoolValues[2]))
	{
		if (GuiBoolValues[2])
		{
			GuiBoolValues[3] = false;
			GuiBoolValues[4] = false;
			bulletSpawnTimerDuration = 10000.0 / 12.0f;
		}
		checkOneBoxTrue(GuiBoolValues[2]);
	}
	if (ImGui::Checkbox("20 Meteors Total", &GuiBoolValues[3]))
	{
		if (GuiBoolValues[3])
		{
			GuiBoolValues[2] = false;
			GuiBoolValues[4] = false;
			bulletSpawnTimerDuration = 10000.0 / 26.0f;
		}
		checkOneBoxTrue(GuiBoolValues[3]);
	}
	if (ImGui::Checkbox("30 Meteors Total", &GuiBoolValues[4]))
	{
		if (GuiBoolValues[4])
		{
			GuiBoolValues[3] = false;
			GuiBoolValues[2] = false;
			bulletSpawnTimerDuration = 10000.0 / 41.0f;
		}
		checkOneBoxTrue(GuiBoolValues[4]);
	}

	if (ImGui::SliderFloat("Player Bullet Speed ", &GuiFloatValues[2], 3, 6));
	{
		if (GuiFloatValues[2] != m_pPlayerPool->all[0]->getPlayerBulletSpeed())
		{
			for (std::vector<Bullet*>::iterator myiter = m_pPlayerPool->all.begin(); myiter != m_pPlayerPool->all.end(); ++myiter)
			{
				Bullet* bullet = *myiter;
				bullet->setPlayerBulletSpeed(GuiFloatValues[2]);
			}
		}
	}
	if (ImGui::SliderFloat("Player Acceleration ", &GuiFloatValues[3], 25.0f, 250.0f));
	{
		if (GuiFloatValues[3] != m_pPlayer->getAcceleration())
		{
			m_pPlayer->setAcceleration(GuiFloatValues[3]);
		}
	}
	// Slider Settings
	if (ImGui::SliderFloat("Gravity ", &GuiFloatValues[1], 9.8, 30));
	{
		if (m_pPool->all[0]->getGravity() != GuiFloatValues[1])
		{
			for (std::vector<Bullet*>::iterator myiter = m_pPool->all.begin(); myiter != m_pPool->all.end(); ++myiter)
			{
				Bullet* bullet = *myiter;
				bullet->setGravity(GuiFloatValues[1]);
			}
		}
	}

}

void PlayScene::m_ImGuiKeyMap()
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

void PlayScene::m_ImGuiSetStyle()
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

void PlayScene::m_updateUI()
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

void PlayScene::createBullets()
{
	m_pPool = new BulletPool(30);
	for (std::vector<Bullet*>::iterator myiter = m_pPool->all.begin(); myiter != m_pPool->all.end(); ++myiter)
	{
		Bullet* bullet = *myiter;
		bullet->setParent(this);
		addChild(bullet);
	}

	m_pPlayerPool = new BulletPool(50);
	for (std::vector<Bullet*>::iterator myiter = m_pPlayerPool->all.begin(); myiter != m_pPlayerPool->all.end(); ++myiter)
	{
		Bullet* bullet = *myiter;
		bullet->setParent(this);
		addChild(bullet);
		bullet->setBulletType(PLAYER_BULLET);
	}
}

void PlayScene::SpawnBullet()
{
	if (GuiBoolValues[0])
	{
		Bullet* bullet = m_pPool->Spawn();
		if (bullet)
		{
			bullet->getTransform()->position = glm::vec2(50 + rand() % 700, 0);
		}

		bulletSpawnTimerStart = SDL_GetTicks();
	}
}

void PlayScene::setBulletBeginSim(bool beginSim)
{
	for (std::vector<Bullet*>::iterator myiter = m_pPool->all.begin(); myiter != m_pPool->all.end(); ++myiter)
	{
		Bullet* bullet = *myiter;
		bullet->setBeginSimulation(beginSim);
	}
}

void PlayScene::checkPlayerBulletBounds()
{
	for (std::vector<Bullet*>::iterator myiter = m_pPlayerPool->all.begin(); myiter != m_pPlayerPool->all.end(); ++myiter)
	{
		Bullet* bullet = *myiter;
		if (bullet->active)
		{
			if (bullet->getTransform()->position.y <= -100.0f)
			{
				m_pPlayerPool->Despawn(bullet);
			}
		}
	}
}

void PlayScene::bulletOnBulletCollision()
{
	for (std::vector<Bullet*>::iterator myiter = m_pPool->all.begin(); myiter != m_pPool->all.end(); ++myiter)
	{
		Bullet* enemyBullet = *myiter;
		if (enemyBullet->active)
		{
			for (std::vector<Bullet*>::iterator playerIter = m_pPlayerPool->all.begin(); playerIter != m_pPlayerPool->all.end(); ++playerIter)
			{
				Bullet* playerBullet = *playerIter;
				if (playerBullet->active)
				{
					if (CollisionManager::AABBCheck(playerBullet, enemyBullet))
					{
						m_pPool->Despawn(enemyBullet);
						m_pPlayerPool->Despawn(playerBullet);
						SoundManager::Instance().playSound("3", 0);
						m_currentScore++;
					}
				}
			}
		}
	}
}

void PlayScene::initLabels()
{
	SDL_Color white = { 225,225,225,225 };

	m_pHighScoreLabel = new Label("High Score: ", "Consolas", 20, white, glm::vec2(600.0f, 100.0f));
	m_pHighScoreLabel->setParent(this);
	addChild(m_pHighScoreLabel);

	m_pCurrentScoreLabel = new Label("Current Score: ", "Consolas", 20, white, glm::vec2(600.0f, 140.0f));
	m_pCurrentScoreLabel->setParent(this);
	addChild(m_pCurrentScoreLabel);

	m_pChangeSceneNote = new Label("Press 1 to change scene", "Consolas", 15, white, glm::vec2(110.0f, 580.0f));
	m_pChangeSceneNote->setParent(this);
	addChild(m_pChangeSceneNote);

	m_pPPM = new Label("Scale: 10 PPM", "Consolas", 15, white, glm::vec2(730.0f, 580.0f));
	m_pPPM->setParent(this);
	addChild(m_pPPM);

	//------------------------------------

	m_pShipMomentumLabel = new Label("Ship Momentum: ", "Consolas", 20, white, glm::vec2(600.0f, 180.0f));
	m_pShipMomentumLabel->setParent(this);
	addChild(m_pShipMomentumLabel);

	m_pShipVelocityLabel = new Label("Ship Velocity: ", "Consolas", 20, white, glm::vec2(600.0f, 220.0f));
	m_pShipVelocityLabel->setParent(this);
	addChild(m_pShipVelocityLabel);

	m_pMassLabel = new Label("Ship Mass: 2,030,000 kg ", "Consolas", 20, white, glm::vec2(600.0f, 260.0f));  // Fixed mass label - the weight of a space shuttle
	m_pMassLabel->setParent(this);
	addChild(m_pMassLabel);

}

void PlayScene::updateLabels()
{
	m_pHighScoreLabel->setText("High Score: " + std::to_string(m_highScore));
	m_pCurrentScoreLabel->setText("Current Score: " +std::to_string(m_currentScore));
	
	if (mass < (Util::magnitude(m_pPlayer->getRigidBody()->velocity * mass) / PPM))
	{
		m_pShipMomentumLabel->setText("Ship Momentum: " + std::to_string(Util::magnitude(m_pPlayer->getRigidBody()->velocity * mass) / PPM) + " kg*m/s");
	}
	else
		m_pShipMomentumLabel->setText("Ship Momentum: " + std::to_string(mass) + " kg*m/s");

		m_pShipVelocityLabel->setText("Ship Velocity: " + std::to_string(Util::magnitude(m_pPlayer->getRigidBody()->velocity) / PPM) + " m/s^2");
	
}

void PlayScene::checkOneBoxTrue(bool& check)
{
	if (!GuiBoolValues[2] || !GuiBoolValues[3] || !GuiBoolValues[4])
	{
		check = true;
	}
}

