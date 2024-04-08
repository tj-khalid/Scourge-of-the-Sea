#include "bullet.h"
#include <iostream>
namespace game {
	Bullet::Bullet(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture):
	GameObject(position, geom, shader, texture){
		speed_ = 10.0f;
		deathTimer_->Start(0.5f);
		type_ = GameObject::Bullet;
	}

	Bullet::~Bullet() {
		GameObject::~GameObject();
	}

	void Bullet::Update(double delta_time) {
		SetPosition(GetPosition() + GetBearing() * speed_ * (float)delta_time);
		if (deathTimer_->Finished()) { setToDestroy_ = true; }
	}

	void Bullet::CollideWith(GameObject* obj) {
		switch (obj->GetObjectType())
		{
		default:
			break;
		case Enemy:
		case EnemyShip:
		case Shark:
		case Player:
			setToDestroy_ = true;
		}
	}
}