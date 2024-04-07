#ifndef HARPOONSHIP_H_
#define HARPOONSHIP_H_

#include "enemy_game_object.h"

namespace game {

    // Inherits from GameObject
    class HarpoonShip : public EnemyGameObject {
    protected:
        
        bool shooting_;


    public:
        HarpoonShip(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        ~HarpoonShip();
        // Update function for moving the player object around
        void Update(double delta_time) override;

        void Shoot();

    }; // class EnemyGameObject

} // namespace game

#endif // Enemy_GAME_OBJECT_H_
