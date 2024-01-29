#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class EnemyGameObject : public GameObject {
    protected:
        enum State { Patrolling, Intercepting };
        State state;
        glm::vec3 patrollingPosition_;

    public:
        EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        // Update function for moving the player object around
        void Update(double delta_time) override;
    }; // class EnemyGameObject

} // namespace game

#endif // Enemy_GAME_OBJECT_H_
