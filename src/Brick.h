#pragma once
#ifndef __BRICK__
#define __BRICK__
#include "Sprite.h"
#include "BrickOrientation.h"

class Brick final : public DisplayObject
{
public:
	Brick();
	~Brick();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void brickMovement(glm::vec2);
	void calculateVelocity();

	//Getters
	BrickOrientation getOrientation();
	glm::vec2 getDirection();
	float getAcceleration();
	float getForce();
	float getMass();
	bool getBeginSimulation();

	//Setters
	void setDirection(glm::vec2);
	void setAcceleration(float);
	void setForce(float);
	void setMass(float);
	void setBeginSimulation(bool);
	void handleEvents();
	void setOrientation(BrickOrientation);


private: 
	glm::vec2 m_direction;
	glm::vec2 m_mousePosition;
	glm::vec2 m_previousPosition;

	float m_acceleraton;
	float m_force;
	float m_mass;
	float m_PPM;
	int m_frameCount;
	bool m_beginSimulation;
	BrickOrientation m_orientation;

	void m_move();
	bool m_withinBounds(glm::vec2);
	void m_reset();

	//Constants
	const float m_gravity = 9.8f;
};

#endif /* defined (__BRICK__) */