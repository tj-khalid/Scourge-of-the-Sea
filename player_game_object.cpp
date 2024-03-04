#include "player_game_object.h"
#include <iostream>
namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
	: GameObject(position, geom, shader, texture){
	hp_ = 3;
	type_ = Player;
	collision_ = true;
	collectibleCount_ = 0;
	invicibiltyTimer_ = new Timer();
	attackCooldown_ = new Timer();
	velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
	normTexture_ = texture;
}

PlayerGameObject::~PlayerGameObject() {
	GameObject::~GameObject();
	delete invicibiltyTimer_;
	delete attackCooldown_;
}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {
	// Special player updates go here
	if (collectibleCount_ == 5) {
		invicibiltyTimer_->Start(10);
		collectibleCount_ = 0;
	}
	invicibiltyTimer_->Finished();
	if (isInvicible()) {
		texture_ = invincibleTexture_;
	}
	else {
		texture_ = normTexture_;
	}

	SetPosition(GetPosition() + velocity_ * (float)delta_time);

	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time);
}

void PlayerGameObject::AddCollectible() {
	collectibleCount_++;
}

void PlayerGameObject::AddForce(glm::vec3& force) {
	velocity_ += force;
	if (glm::length(velocity_) > 2) {
		velocity_ = glm::normalize(velocity_) * 2.0f;
	}
}

void PlayerGameObject::CollideWith(GameObject* obj) {
	switch (obj->GetObjectType()){
		case Enemy:
			if (!isInvicible())
				TakeDamage(1);
			break;
		case Collectible:
			if(!obj->GetGhostMode())
				AddCollectible();
			break;
		default:
			break;
	}
}

bool PlayerGameObject::Shoot() {
	attackCooldown_->Finished();
	if (!attackCooldown_->Running())
	{
		attackCooldown_->Start(.3f);
		return true;
	}
	return false;
}
} // namespace game
