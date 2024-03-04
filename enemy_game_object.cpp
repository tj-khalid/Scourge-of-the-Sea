#include "enemy_game_object.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace game {

	/*
		EnemyGameObject inherits from GameObject
	*/

	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		hp_ = 1;
		collision_ = true;
		detectionRadius_ = 2.f;
		state = Patrolling;
		type_ = Enemy;
		patrollingPosition_ = position_;
		t = 0;

		chaseTimer = new Timer;
	}

	EnemyGameObject::~EnemyGameObject() {
		GameObject::~GameObject();
		delete chaseTimer;
	}
	// Update function for moving the player object around
	void EnemyGameObject::Update(double delta_time) {
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
				position_ = patrollingPosition_ + (targetDir_ * t*speed);
				SetRotation(std::atan2(targetDir_.y, targetDir_.x));
				break;
			default:
				break;
		}
		if (chaseTimer->Finished()){
			chaseTarget(target_);
		}
		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

	void EnemyGameObject::chaseTarget(GameObject* target) {
		state = Intercepting;
		target_ = target;
		targetDir_ = glm::normalize(target->GetPosition() - position_);
		patrollingPosition_ = position_;
		chaseTimer->Start(2);
		t = 0;
	}

	void EnemyGameObject::CollideWith(GameObject* obj) {
		switch (obj->GetObjectType()){
		case Player:
		case Bullet:
			TakeDamage(1);
			break;
		default:
			break;
		}
	}
} // namespace game
