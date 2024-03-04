#ifndef BULLET_H_
#define BULLET_H_

#include "game_object.h"

namespace game {
	class Bullet : public GameObject {
	public:
		Bullet(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
		~Bullet();
		void Update(double delta_time) override;

		void CollideWith(GameObject*) override;
	protected:
		Timer* lifespan_;
		float speed_;
	};
}
#endif

