#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "geometry.h"
#include "timer.h"

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {
    protected:
        enum ObjectType {Player, Enemy, Collectible, Other};

    public:
        // Constructor
        GameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        // Update the GameObject's state. Can be overriden in children
        virtual void Update(double delta_time);

        // Renders the GameObject 
        virtual void Render(glm::mat4 view_matrix, double current_time);

        // Getters
        inline glm::vec3 GetPosition(void) const { return position_; }
        inline float GetScaleX(void) const { return scale_x_; }
        inline float GetScaleY(void) const { return scale_y_; }
        inline float GetRotation(void) const { return angle_; }
        Timer* GetDeathTimer(void) {return deathTimer_;}
        bool GetCollision(void) { return collision_; }
        float GetCollisionRadius(void) { return collisionRadius_; }
        ObjectType GetObjectType(void) { return type_; }
        int GetHP(void) { return hp_; }

        // Get bearing direction (direction in which the game object
        // is facing)
        glm::vec3 GetBearing(void) const;

        // Get vector pointing to the right side of the game object
        glm::vec3 GetRight(void) const;


        // Setters
        inline void SetPosition(const glm::vec3& position) { position_ = position; }
        inline void SetScaleX(float scale) { scale_x_ = scale; }
        inline void SetScaleY(float scale) { scale_y_ = scale; }
        void SetRotation(float angle);
        
        // Reduces hp by damage
        void TakeDamage(int damage);

        protected:
            // Object's Transform Variables
            glm::vec3 position_;
            float scale_x_;
            float scale_y_;
            float angle_;

            // Objects Health
            int hp_;

            // timer for deleting game object
            Timer *deathTimer_;

            // Objects collidabilty
            bool collision_;
            float collisionRadius_;

            // The type of Object
            ObjectType type_;

            // Geometry
            Geometry *geometry_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;


    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
