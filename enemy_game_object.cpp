#include "enemy_game_object.h"

namespace game {

	/*
		EnemyGameObject inherits from GameObject
	*/

	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		hp_ = 1;
		collision_ = true;
		collisionRadius_ = 0.4f;
		state = Patrolling;
		type_ = Enemy;
		patrollingPosition_ = position;
	}

	// Update function for moving the player object around
	void EnemyGameObject::Update(double delta_time) {

		// Special enemy updates go here
		if (state = Patrolling) {
			position_.x = glm::cos(delta_time) + patrollingPosition_.x;
			position_.y = glm::sin(delta_time) + patrollingPosition_.y;
		}
		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

} // namespace game
