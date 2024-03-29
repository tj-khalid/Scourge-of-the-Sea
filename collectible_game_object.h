#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class CollectibleGameObject : public GameObject {

    public:
        CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        
        void Update(double delta_time) override;

        void CollideWith(GameObject*) override;
    };

} // namespace game

#endif // COLLECTIBLE_GAME_OBJECT_H_
