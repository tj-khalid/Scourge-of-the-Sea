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
		maxspeed = 2.5f;
		type_ = GameObject::EnemyShip;
		ReactionTime = 1.0f;
	}

	EnemyShip::~EnemyShip() {
		GameObject::~GameObject();
		delete chaseTimer;
	}
	// Update function for moving the player object around
	void EnemyShip::Update(double delta_time) {
		// Special enemy updates go here
		t += delta_time;
		float speed = 1.5f;
		float pi_over_two = glm::pi<float>() / 2.0f;
		//std::cout << state << std::endl;



		switch(state) {
		case Intercepting:
			AddForce(glm::normalize(targetDir_ + target_->GetBearing() * 2.f) * (float)delta_time);

			velocity_ = glm::distance(position_, target_->GetPosition()) * velocity_;

			if (glm::distance((target_->GetBearing() * 2.f + target_->GetPosition()), position_) < 0.3f) {
				state = Attacking;
				std::cout << state;
			}
		case Attacking:
			SetRotation((GetRotation() + target_->GetRotation() * delta_time));
		}


		SetRotation(std::atan2(glm::normalize(velocity_).y, glm::normalize(velocity_).x));

		if (chaseTimer->Finished()) {
			chaseTarget(target_);
		}
		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

} // namespace game
