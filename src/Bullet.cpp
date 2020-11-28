#include "Bullet.h"
#include "TextureManager.h"

Bullet::Bullet() {
	TextureManager::Instance()->load("../Assets/textures/Meteor2.png", "Meteor");
	TextureManager::Instance()->load("../Assets/textures/lazer2.png", "Lazer");
	float deltaTime = 1.0f / 60.0f;
	auto size = TextureManager::Instance()->getTextureSize("Meteor");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(700.0f, 300.0f); //use rand() to bring in random bullets on x axis
	setType(ENEMY);
	m_playSound = true;
	m_collisionHappened = false;
	m_type = ENEMY_BULLET;
	m_playerBulletSpeed = 5.0f;
	Reset();
}

Bullet::~Bullet()
= default;

void Bullet::draw() {
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	if (m_type == ENEMY_BULLET)
	{
		if (active) {
			// alias for x and y

			TextureManager::Instance()->draw("Meteor", x, y, 0, 255, true);
		}
	}
	else if (m_type == PLAYER_BULLET)
	{
		if (active) {
			TextureManager::Instance()->draw("Lazer", x, y, 0, 255, true);
		}
	}
}

void Bullet::update() {
	if (m_type == ENEMY_BULLET)
	{
		getRigidBody()->acceleration.y = m_gravity; // Gravity

		if (m_beginSimulation && active) {
			float deltaTime = 1.0f / 60.0f;
			getRigidBody()->velocity = getRigidBody()->velocity + (getRigidBody()->acceleration * deltaTime);
			getTransform()->position = getTransform()->position + getRigidBody()->velocity * deltaTime;

		}
	}
	else if (m_type == PLAYER_BULLET)
	{
		getTransform()->position.y -= m_playerBulletSpeed;
	}
}

void Bullet::clean() {

}

void Bullet::Reset()
{
	getRigidBody()->velocity = glm::vec2(0.0f, 50.0f);
	getRigidBody()->acceleration = glm::vec2(0, 9.8); // Gravity
	getRigidBody()->isColliding = false;
}

bool Bullet::getBeginSimulation()
{
	return m_beginSimulation;
}

float Bullet::getGravity()
{
	return m_gravity;
}

bool Bullet::getCollisionHappened()
{
	return m_collisionHappened;
}

bool Bullet::getPlaySound()
{
	return m_playSound;
}

BulletType Bullet::getBulletType()
{
	return m_type;
}

float Bullet::getPlayerBulletSpeed()
{
	return m_playerBulletSpeed;
}

void Bullet::setGravity(float gravity)
{
	m_gravity = gravity;
}

void Bullet::setBeginSimulation(bool beginSim)
{
	m_beginSimulation = beginSim;
}

void Bullet::setCollisionHappened(bool collisionHappened)
{
	m_collisionHappened = collisionHappened;
}

void Bullet::setPlaySound(bool playSound)
{
	m_playSound = playSound;
}

void Bullet::setBulletType(BulletType bt)
{
	m_type = bt;
	if (m_type == ENEMY_BULLET)
	{
		auto size = TextureManager::Instance()->getTextureSize("Meteor");
		setWidth(size.x);
		setHeight(size.y);
	}
	else if (m_type == PLAYER_BULLET)
	{
		auto size = TextureManager::Instance()->getTextureSize("Lazer");
		setWidth(size.x);
		setHeight(size.y);
	}
}

void Bullet::setPlayerBulletSpeed(float bulletSpeed)
{
	m_playerBulletSpeed = bulletSpeed;
}
