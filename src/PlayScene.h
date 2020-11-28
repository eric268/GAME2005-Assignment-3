#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Bullet.h"
#include "BulletPool.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	void beginSimulation();
	void resetSceneSettings();
	void resetCrateSettings();
	void checkGuiChangs();
	float getPPM();
	void checkCollision();
	void initGuiVariables();

	const float mass = 2030000; //Roughly the weight of a space shuttle
	const int velocity = 0;

private:
	float bulletSpawnTimerDuration;
	float bulletSpawnTimerStart;

	glm::vec2 m_mousePosition;

	BulletPool* m_pPool;
	BulletPool* m_pPlayerPool;

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Bullet* m_pBullet;

	Button* m_pBackButton;
	Button* m_pNextButton;

	bool GuiBoolValues[5];
	float GuiFloatValues[5];

	void createBullets();
	void SpawnBullet();
	void setBulletBeginSim(bool);
	void checkPlayerBulletBounds();

	void bulletOnBulletCollision();
	void initLabels();
	void updateLabels();
	void checkOneBoxTrue(bool&);

	// ImGui utility functions
	void m_ImGuiKeyMap();
	void m_ImGuiSetStyle();
	void m_updateUI();

	bool m_exitApp = false;
	bool m_displayAbout = false;
	bool m_displayUI = true;

	bool m_bulletsTen; // Default checkbox of 10 bullets
	bool m_bulletsTwenty; // Checkbox of 20 bullets
	bool m_impossibleThirty; // Checkbox of 30 bullets

	bool m_bulletsTenChecked;
	Label* m_pHighScoreLabel;
	Label* m_pCurrentScoreLabel;
	Label* m_pChangeSceneNote;
	Label* m_pShipMomentumLabel;
	Label* m_pShipVelocityLabel;
	Label* m_pMeteorMomentumLabel;
	Label* m_pMeteorVelocityLabel;
	Label* m_pMassLabel; // Fixed mass label
	Label* m_pPPM; // Fixed scale label


	float m_changeGravity; // Slider to incease gravity

	int m_currentScore;
	int m_highScore;

	float PPM = 10.0f;
	float PPMPrev;
	const SDL_Color black = { 0,0,0,255 };
	const SDL_Color white = { 255,255,255, 255 };
	const SDL_Color red = { 255,0,0,255 };




};

#endif /* defined (__PLAY_SCENE__) */