#include "shark.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace game {

	/*
		Shark inherits from GameObject
	*/

	Shark::Shark(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: EnemyGameObject(position, geom, shader, texture) {
		ReactionTime = 1.0f;
		type_ = GameObject::EnemyShip;
	}

	Shark::~Shark() {
		GameObject::~GameObject();
		delete chaseTimer;
	}
	// Update function for moving the player object around
	void Shark::Update(double delta_time) {
		// Special enemy updates go here
		t += delta_time;
		float speed = 1.5f;
		float pi_over_two = glm::pi<float>() / 2.0f;
		switch (state){
			case Patrolling:
				position_.x = glm::cos(t*speed) + patrollingPosition_.x;
				position_.y = glm::sin(t*speed) + patrollingPosition_.y;

				SetRotation(std::atan2(glm::sin(t* speed), glm::cos(t* speed)) + pi_over_two);
				break;
			case Intercepting:
				position_ = patrollingPosition_ + (glm::normalize(targetDir_) * t*speed);
				SetRotation(std::atan2(glm::normalize(targetDir_).y, glm::normalize(targetDir_).x));
				break;
			default:
				break;
		}
		if (chaseTimer->Finished()) {
			chaseTarget(target_);
		}
		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

} // namespace game
