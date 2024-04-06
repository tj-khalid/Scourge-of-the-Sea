#ifndef ENEMYSHIP_H_
#define ENEMYSHIP_H_

#include "enemy_game_object.h"

namespace game {

    // Inherits from GameObject
    class EnemyShip : public EnemyGameObject {
    protected:
        
        bool shooting_;


    public:
        EnemyShip(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        ~EnemyShip();
        // Update function for moving the player object around
        void Update(double delta_time) override;

        void Shoot();

    }; // class EnemyGameObject

} // namespace game

#endif // Enemy_GAME_OBJECT_H_
