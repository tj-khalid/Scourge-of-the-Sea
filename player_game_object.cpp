#include "player_game_object.h"

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
	: GameObject(position, geom, shader, texture) {
	hp_ = 3;
	type_ = Player;
	collision_ = true;
	collisionRadius_ = 0.6f;
	collectibleCount_ = 0;
	invicibiltyTimer_ = new Timer();
}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {

	// Special player updates go here
	if (collectibleCount_ == 5) {
		invicibiltyTimer_->Start(10);
		collectibleCount_ = 0;
	}
	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time);
}

void PlayerGameObject::AddCollectible() {
	collectibleCount_++;
}

} // namespace game
