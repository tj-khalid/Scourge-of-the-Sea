#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class CollectibleGameObject : public GameObject {

    public:
        enum collectType {Coin, Booze, Chest}; 
        CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        enum collectType getCollectType();
        void Update(double delta_time) override;

        void CollideWith(GameObject*) override;

        inline void setCollectType(enum collectType s) {
            collectType = s;
        }
    protected:
        collectType collectType;
    };
    

} // namespace game

#endif // COLLECTIBLE_GAME_OBJECT_H_
