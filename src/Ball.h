#pragma once
#ifndef __BALL__
#define __BALL__

#include "DisplayObject.h"
#include "WallCollision.h"
class Ball final : public DisplayObject
{
public:

	Ball();
	~Ball ();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	WallCollison getCollisionType();
	void updateVelocity();
	void calculateMomentum();
	void velocityAfterCollision(float, float);

	//Setters
	void setCollisionType(WallCollison);
	void setDirection(glm::vec2);
	void setAcceleration(float);
	void setForce(float);
	void setMass(float);
	void setBeginSimulation(bool);
	void setWallWeight(float);
	void setMomentum(float);

	//Getters
	void checkCollisionWalls();
	glm::vec2 getDirection();
	float getAcceleration();
	float getForce();
	float getMass();
	bool getBeginSimulation();
	float getWallWeight();
	float getMomentum();

private:
	glm::vec2 m_direction;
	glm::vec2 m_mousePosition;
	glm::vec2 m_previousPosition;

	float m_acceleraton;
	float m_force;
	float m_mass;
	bool m_beginSimulation;
	int m_frameCount;
	float m_PPM;
	float m_wallWeight;
	float m_momentum;
	int collisionCheckCounter;
	bool collisionJustHappened;

	WallCollison collisionType;

	void m_move();
	bool m_withinBounds();
	void m_reset();

	//Constants
	const float m_gravity = 9.8f;
};

#endif /* defined (__Ball__) */