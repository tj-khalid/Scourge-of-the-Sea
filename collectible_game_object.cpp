#include "collectible_game_object.h"

//Texture 8,9 

namespace game {

	/*
		CollectibleGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		hp_ = 1;
		type_ = Collectible;
		collision_ = true;
		collectType = Coin; 
	}

	// Update function for moving the player object around
	void CollectibleGameObject::Update(double delta_time) {

		// Special player updates go here

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

	void CollectibleGameObject::CollideWith(GameObject* obj) {
		switch (obj->GetObjectType()){
			case Player:
				setToDestroy_ = true; 
				break;
			default:
				break;
		}
	}

	enum CollectibleGameObject::collectType CollectibleGameObject::getCollectType() {
		return collectType;
	}

} // namespace game
