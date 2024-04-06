#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class EnemyGameObject : public GameObject {
    public:
        enum State { Patrolling, Intercepting, Attacking, Other };
    protected:
        State state;
        glm::vec3 patrollingPosition_;
        glm::vec3 targetDir_;
        GameObject* target_;
        Timer* chaseTimer;
        float detectionRadius_;
        float reaction_time_;
        float t;


    public:
        EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        ~EnemyGameObject();
        // Update function for moving the player object around
        void Update(double delta_time) override;

        void CollideWith(GameObject*) override;

        float getDetectionRadius() { return detectionRadius_; };

        void chaseTarget(GameObject* target);

        State getState(void) const { return state; }
    }; // class EnemyGameObject

} // namespace game

#endif // Enemy_GAME_OBJECT_H_
