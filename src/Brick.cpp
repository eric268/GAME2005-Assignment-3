#include "Brick.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "Util.h"
#include "Game.h"
#include <iostream>

Brick::Brick()
{
	TextureManager::Instance()->load("../Assets/textures/BrickSprite.png", "BrickSprite");

	auto size = TextureManager::Instance()->getTextureSize("BrickSprite");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(535.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);

	m_direction = glm::vec2(0.0f);
	m_previousPosition = getTransform()->position;
	m_acceleraton =0.0f;
	m_force =0.0f;
	m_mass =10.0f; //In kg
	m_beginSimulation = false;
	m_frameCount = 0;
	m_PPM = 100;
	setType(BRICK);
}

Brick::~Brick() = default;

void Brick::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("BrickSprite", x, y, 0, 255, true);
}

void Brick::update()
{
	calculateVelocity();

}

void Brick::clean()
{
}

glm::vec2 Brick::getDirection()
{
	return m_direction;
}

void Brick::setDirection(glm::vec2 direction)
{
	m_direction = direction;
}

float Brick::getAcceleration()
{
	return m_acceleraton;
}

void Brick::setAcceleration(float acceleration)
{
	m_acceleraton = acceleration;
}

float Brick::getForce()
{
	return m_force;
}

void Brick::setForce(float force)
{
	m_force = force;
}

float Brick::getMass()
{
	return m_mass;
}

void Brick::setMass(float mass)
{
	m_mass = mass;
}

bool Brick::getBeginSimulation()
{
	return m_beginSimulation;
}

void Brick::setBeginSimulation(bool begin)
{
	m_beginSimulation = begin;
}

void Brick::handleEvents()
{
	auto wheel = 0;

	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			m_mousePosition.x = event.motion.x;
			m_mousePosition.y = event.motion.y;
			break;
		}
	}
}

void Brick::brickMovement(glm::vec2 mousePos)
{
	if (m_withinBounds(mousePos))
	{
		getTransform()->position = mousePos;
	}


}

void Brick::calculateVelocity()
{
	//Only calculates velocity every 1/6 of a second 
	if (m_frameCount++ >=10)
	{
		getRigidBody()->velocity = (getTransform()->position / m_PPM - m_previousPosition/m_PPM)*6.0f; // Have to measure velocity in m/s not 1/6 of a second
		m_previousPosition = getTransform()->position;
		m_frameCount = 0;
	}
}

void Brick::m_move()
{

}

bool Brick::m_withinBounds(glm::vec2 mousePos)
{
	if (mousePos.x >= getWidth() / 2 && mousePos.x <= 800 - getWidth() / 2)
	{
		if (mousePos.y >= getHeight() / 2 && mousePos.y <= 600 - getHeight() / 2)
		{
			return true;
		}
	}
	return false;

}

void Brick::m_reset()
{
}
