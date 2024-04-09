#include "HarpoonShip.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace game {

	/*
		HarpoonShip inherits from EnemyGameObject
	*/

	HarpoonShip::HarpoonShip(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: EnemyGameObject(position, geom, shader, texture) {
		maxspeed = 3.f;
		reaction_time_ = .05f;
		type_ = GameObject::HarpoonShip;
		attackcooldowntime_ = 1.f;
	}

	HarpoonShip::~HarpoonShip() {
		EnemyGameObject::~EnemyGameObject();
	}
	// Update function for moving the player object around
	void HarpoonShip::Update(double delta_time) {
		// Special enemy updates go here
		t += delta_time;
		float speed = 1.5f;
		float pi_over_two = pi<float>() / 2.0f;

		//Intercepting AI (If plyer is within a certain range fires a Harpoon)
		switch (state) {
		case Intercepting:
			AddForce((targetDir_) * (float)delta_time);
			if (length(targetDir_) <= 2.0f) {
				shooting_ = true;
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
