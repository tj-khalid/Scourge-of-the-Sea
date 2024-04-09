#include "player_game_object.h"

#include <iostream>
namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
	: GameObject(position, geom, shader, texture){
	hp_ = 5;
	type_ = Player;
	collision_ = true;
	coinCount_ = 0;
	boozeCount_ = 0;
	chestCount_ = 0;
	invicibiltyTimer_ = new Timer();
	secondAttackCooldown_ = new Timer();
	normTexture_ = texture;
	maxspeed = 4; 
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
		if (hp_ < 5) {
			hp_ += 2;
			HP_UI->SetText("HP: " + to_string(hp_));
		}
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

//Checks Players Collisions With Other Game Objects
void PlayerGameObject::CollideWith(GameObject* obj) {
	switch (obj->GetObjectType()){
		default:
			break;
		case Enemy:
		case Shark:
		case EnemyShip:
		case HarpoonShip:
		case Bullet:
			if (!isInvicible()) {
				TakeDamage(1); 
				HP_UI->SetText("HP: " + to_string(hp_)); 
			}
			break;
		case Collectible:
			CollectibleGameObject* collectable = (CollectibleGameObject*)obj;
			AddCollectible(collectable);
			break;

	}
}

//Adds to the Collectible Count Of THe Player
void PlayerGameObject::AddCollectible(CollectibleGameObject* collectible) {
	switch (collectible->getCollectType())
	{
	//Chest Increases Score By 5 total
	case CollectibleGameObject::Chest:
		coinCount_ += 4;
	//Coin Increases Score by 1
	case CollectibleGameObject::Coin:
		coinCount_++;
		//update the HUD
		Coin_HUD->SetText("Coins: " + to_string(coinCount_));
		break;
	//If You Get 5 Barrels Of Booze you get invincible for a few seconds
	case CollectibleGameObject::Booze:
		boozeCount_++;
		break;


	default:
		break;
	}
}

//Shoots Harpoon
bool PlayerGameObject::Shoot2() {
	secondAttackCooldown_->Finished();
	if (!secondAttackCooldown_->Running())
	{
		secondAttackCooldown_->Start(2.f);
		return true;
	}
	return false;
}
} // namespace game
