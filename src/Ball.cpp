#include "Ball.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "Util.h"
#include "Game.h"
#include <cassert>
#include <iostream>

Ball::Ball()
{
	TextureManager::Instance()->load("../Assets/textures/Ball.png", "Ball");

	auto size = TextureManager::Instance()->getTextureSize("Ball");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(135.0f, 200.0f);
	getRigidBody()->velocity = glm::vec2(250.0f, 250.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);

	m_direction = glm::vec2(0.0f);
	m_previousPosition = getTransform()->position;
	m_acceleraton = 0.0f;
	m_force = 0.0f;
	m_mass = 2.0f; //In kg
	m_wallWeight = 2.0f;
	m_beginSimulation = false;
	m_frameCount = 0;
	m_PPM = 100;
	m_momentum = 0;
	collisionType = NO_WALL_COLLISION;
	setType(BALL);
	collisionCheckCounter = 0;
	collisionJustHappened = false;
}

Ball::~Ball() = default;

void Ball::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("Ball", x, y, 0, 255, true);
}

void Ball::update()
{
	updateVelocity();
	m_move();
	calculateMomentum();
	checkCollisionWalls();


}

void Ball::clean()
{
}

glm::vec2 Ball::getDirection()
{
	return m_direction;
}

void Ball::setDirection(glm::vec2 direction)
{
	m_direction = direction;
}

float Ball::getAcceleration()
{
	return m_acceleraton;
}

void Ball::setAcceleration(float acceleration)
{
	m_acceleraton = acceleration;
}

float Ball::getForce()
{
	return m_force;
}

void Ball::setForce(float force)
{
	m_force = force;
}

float Ball::getMass()
{
	return m_mass;
}

void Ball::setMass(float mass)
{
	m_mass = mass;
}

bool Ball::getBeginSimulation()
{
	return m_beginSimulation;
}

void Ball::setBeginSimulation(bool begin)
{
	m_beginSimulation = begin;
}

void Ball::checkCollisionWalls()
{
	if (getTransform()->position.x - getWidth() / 2 <= 0 && collisionJustHappened == false)
	{

		collisionType =  LEFT_WALL_COLLISION;
	}
	else if (getTransform()->position.x + getWidth() / 2 >= 800.0f && collisionJustHappened == false)
	{
		collisionType = RIGHT_WALL_COLLISION;
	}
	else if (getTransform()->position.y - getHeight() / 2 <= 0 && collisionJustHappened == false)
	{
		collisionType = CEILING_COLLISION;
	}
	else if (getTransform()->position.y + getHeight() / 2 >= 600 && collisionJustHappened == false)
	{
		collisionType = FLOOR_COLLISION;
	}
	else
		collisionType = NO_WALL_COLLISION;
}

float Ball::getWallWeight()
{
	return m_wallWeight;
}

void Ball::setWallWeight(float wallWeight)
{
	m_wallWeight = wallWeight;
}

float Ball::getMomentum()
{
	return m_momentum;
}

void Ball::setMomentum(float momentum)
{
	m_momentum = momentum;
}

WallCollison Ball::getCollisionType()
{
	return collisionType;
}

void Ball::setCollisionType(WallCollison collision)
{
	collisionType = collision;
}

void Ball::updateVelocity()
{
	if (collisionCheckCounter >= 5)
	{
		collisionJustHappened = false;

		if (collisionType == LEFT_WALL_COLLISION || collisionType == RIGHT_WALL_COLLISION)
		{
			velocityAfterCollision(m_wallWeight, 0);
			getRigidBody()->velocity.x = -getRigidBody()->velocity.x;
			collisionType = NO_WALL_COLLISION;
			collisionJustHappened = true;
		}
		else if (collisionType == FLOOR_COLLISION || collisionType == CEILING_COLLISION)
		{
			velocityAfterCollision(m_wallWeight, 0);
			getRigidBody()->velocity.y = -getRigidBody()->velocity.y;
			collisionType = NO_WALL_COLLISION;
			collisionJustHappened = true;
		}
		collisionCheckCounter = 0;
	}
	else
	{
		collisionCheckCounter++;
	}

}

void Ball::calculateMomentum()
{
	m_momentum = (m_mass * Util::magnitude(getRigidBody()->velocity))/m_PPM;
	std::cout << m_momentum << std::endl;
}

void Ball::velocityAfterCollision(float otherWeight, float otherVelocity)
{
	float otherObjectMomentum = otherWeight * otherVelocity;
	float totalMomentumBefore = m_momentum + otherObjectMomentum;
	float totalMass = m_mass + otherWeight;
	
	assert(totalMass > 0);

	float newVelocity = totalMomentumBefore / totalMass;
	float percentChangeVel = newVelocity / (Util::magnitude(getRigidBody()->velocity)/m_PPM);

	getRigidBody()->velocity.x *= percentChangeVel;
	getRigidBody()->velocity.y *= percentChangeVel;
}

void Ball::m_move()
{
	float deltaTime = 1.0f / 60.0f;

	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltaTime;
	pos.y += getRigidBody()->velocity.y * deltaTime;

	getTransform()->position = pos;
}

bool Ball::m_withinBounds()
{
	return false;
}
