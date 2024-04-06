#ifndef SHARK_H_
#define SHARK_H_

#include "enemy_game_object.h"

namespace game {

    // Inherits from GameObject
    class Shark : public EnemyGameObject {
    protected:
        
        


    public:
        Shark(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        // Update function for moving the player object around
        void Update(double delta_time) override;

    }; // class EnemyGameObject

} // namespace game

#endif // Enemy_GAME_OBJECT_H_
