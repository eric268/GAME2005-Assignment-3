#pragma once
#ifndef __BULLETPOOL__
#define __BULLETPOOL__

#include "Bullet.h"


class BulletPool
{
public:
	BulletPool(int);
	~BulletPool();


	Bullet* Spawn();
	Bullet* FireBullet(GameObject* player);
	void Despawn(Bullet*);

	std::vector<Bullet*> all;

private:
	std::vector<Bullet*> inactive;
	std::vector<Bullet*> active;
};

#endif

