#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "Sprite.h"
#include "BulletType.h"


class Bullet : public DisplayObject
{
public:
	Bullet();
	~Bullet();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void Reset();

	bool active = false;

	float m_changeGravity; // Slider to incease gravity

	bool getBeginSimulation();
	float getGravity();
	bool getCollisionHappened();
	bool getPlaySound();
	BulletType getBulletType();
	float getPlayerBulletSpeed();

	void setGravity(float);
	void setBeginSimulation(bool);
	void setCollisionHappened(bool);
	void setPlaySound(bool);
	void setBulletType(BulletType);
	void setPlayerBulletSpeed(float);


private:
	bool   m_beginSimulation;
	float  m_gravity;
	bool   m_playSound;
	bool   m_collisionHappened;
	float  m_playerBulletSpeed;
	BulletType m_type;

	//Bullet* m_pBullet;

};

#endif


