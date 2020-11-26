#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	TextureManager::Instance()->draw("BackgroundA2", 400, 300, 0, 255, true);
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}


}

void StartScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/CollisionBackground.jpg", "BackgroundA2");

	const SDL_Color blue = { 0, 0, 255, 255 };
	const SDL_Color black = { 0, 0, 0, 255 };
	
	m_pStartLabel = new Label("2D Collision Detection and Response", "Consolas", 36, white, glm::vec2(400.0f, 100.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pInstructionsLabel = new Label("Select a scene", "Consolas", 36, white, glm::vec2(400.0f, 400.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	
	m_pName[0]  = new Label("Eric Galway 101252535", "Consolas", 15, white, glm::vec2(700.0f, 570.0f));
	m_pName[0]->setParent(this);
	addChild(m_pName[0]);

	m_pName[1] = new Label("Ian Chapman 101248866", "Consolas", 15, white, glm::vec2(700.0f, 585.0f));
	m_pName[1]->setParent(this);
	addChild(m_pName[1]);


	
	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 470.0f); 

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

	m_pScene2StartButton = new Button();
	m_pScene2StartButton->getTransform()->position = glm::vec2(400.0f, 550.0f);
	

	m_pScene2StartButton->addEventListener(CLICK, [&]()-> void
		{
			m_pScene2StartButton->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_SCENE_2);
		});

	m_pScene2StartButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pScene2StartButton->setAlpha(128);
		});

	m_pScene2StartButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pScene2StartButton->setAlpha(255);
		});
	addChild(m_pScene2StartButton);

	m_pButtonLabel[0] = new Label("Scene 1", "Consolas", 25, white, glm::vec2(400.0f, 470.0f));
	m_pButtonLabel[0]->setParent(this);
	addChild(m_pButtonLabel[0]);
	m_pButtonLabel[1] = new Label("Scene 2", "Consolas", 25, white, glm::vec2(400.0f, 550.0f));
	m_pButtonLabel[1]->setParent(this);
	addChild(m_pButtonLabel[1]);
}

