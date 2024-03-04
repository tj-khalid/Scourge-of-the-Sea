#include "bullet.h"
#include <iostream>
namespace game {
	Bullet::Bullet(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture):
	GameObject(position, geom, shader, texture){
		lifespan_ = new Timer;
		speed_ = 10.0f;
		lifespan_->Start(3);
		type_ = GameObject::Bullet;
	}

	Bullet::~Bullet() {
		GameObject::~GameObject();
		delete lifespan_;
	}

	void Bullet::Update(double delta_time) {
		SetPosition(GetPosition() + GetBearing() * speed_ * (float)delta_time);
		if (lifespan_->Finished()) { setToDestroy_ = true; }
	}

	void Bullet::CollideWith(GameObject* obj) {
		switch (obj->GetObjectType())
		{
		default:
			break;
		case Enemy:
			setToDestroy_ = true;
		}
	}
}