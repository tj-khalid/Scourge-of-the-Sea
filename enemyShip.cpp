#include "enemyShip.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace game {

	/*
		EnemyShip inherits from GameObject
	*/

	EnemyShip::EnemyShip(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: EnemyGameObject(position, geom, shader, texture) {
		maxspeed = 4.5f;
		reaction_time_ = 1.0f;
		type_ = GameObject::EnemyShip;
	}

	EnemyShip::~EnemyShip() {
		EnemyGameObject::~EnemyGameObject();
	}
	// Update function for moving the player object around
	void EnemyShip::Update(double delta_time) {
		// Special enemy updates go here
		t += delta_time;
		float speed = 1.5f;
		float pi_over_two = pi<float>() / 2.0f;
		//std::cout << state << std::endl;

		//cout << targetDir_.x << " " << targetDir_.y << endl;
		switch (state) {
		case Intercepting:
			AddForce((targetDir_ + target_->GetBearing() * 4.f) * (float)delta_time);
			if (RayCollision(GetRight(), target_)) {
				shooting_ = true;
				cout << "shooting" << endl;
			}
			else {
				shooting_ = false;
				//cout << "not shooting" << endl;
			}
		}
		SetRotation(atan2(normalize(velocity_).y, normalize(velocity_).x));

		// Call the parent's update method to move the object in standard way, if desired
		EnemyGameObject::Update(delta_time);
	}

} // namespace game
