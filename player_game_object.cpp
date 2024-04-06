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
	coinCount_ = 0;
	boozeCount_ = 0;
	chestCount_ = 0;
	invicibiltyTimer_ = new Timer();
	normTexture_ = texture;
	maxspeed = 2; 
}

PlayerGameObject::~PlayerGameObject() {
	GameObject::~GameObject();
	delete invicibiltyTimer_;
	delete attackCooldown_;
}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {
	// Special player updates go here
	if (boozeCount_ == 5) {
		invicibiltyTimer_->Start(10);
		boozeCount_ = 0;
	}
	invicibiltyTimer_->Finished();
	if (isInvicible()) {
		texture_ = invincibleTexture_;
	}
	else {
		texture_ = normTexture_;
	}

	

	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time);
}

void PlayerGameObject::CollideWith(GameObject* obj) {
	switch (obj->GetObjectType()){
		default:
			break;
		case Enemy:
			if (!isInvicible())
				TakeDamage(1);
			break;
		case Collectible:
			CollectibleGameObject* collectable = (CollectibleGameObject*)obj;
			AddCollectible(collectable);
			break;
	}
}

void PlayerGameObject::AddCollectible(CollectibleGameObject* collectible) {
	switch (collectible->getCollectType())
	{
	case CollectibleGameObject::Chest:
		coinCount_ += 4;
	case CollectibleGameObject::Coin:
		coinCount_++;
		for each (GameObject* child in children){
			if (child->GetObjectType() == CoinsText) {
				TextGameObject* coinText = (TextGameObject*) child;
				coinText->SetText("Coins: " + to_string(coinCount_));
			}
		}
		break;
	case CollectibleGameObject::Booze:
		boozeCount_++;
		break;


	default:
		break;
	}
}

} // namespace game
