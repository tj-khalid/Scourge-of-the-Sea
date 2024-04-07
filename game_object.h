#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "geometry.h"
#include "timer.h"
#include <vector>

using namespace std;
using namespace glm;

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {
    public:
        enum ObjectType {Player, Enemy, Collectible, SwingingAxe, Other, Bullet, Shark, EnemyShip, CoinsText, HpText, HarpoonShip};

    public:
        // Constructor
        GameObject(const vec3& position, Geometry* geom, Shader* shader, GLuint texture);
        ~GameObject();
        // Update the GameObject's state. Can be overriden in children
        virtual void Update(double delta_time);

        // Renders the GameObject 
        virtual void Render(mat4 view_matrix, double current_time);

        virtual void CollideWith(GameObject*) {};

        virtual void Destroy(void) {};
        // Getters
        inline vec3 GetPosition(void) const { return position_; }
        inline float GetScaleX(void) const { return scale_.x; }
        inline float GetScaleY(void) const { return scale_.y; }
        inline float GetRotation(void) const { return angle_; }
        inline float GetOrbitAngle(void) const { return orbitAngle_; }
        inline float GetAngle(void) const { return angle_; }
        Timer* GetDeathTimer(void) {return deathTimer_;}
        bool GetCollision(void) const { return collision_; }
        float GetCollisionRadius(void) const { return collisionRadius_; }
        ObjectType GetObjectType(void) const { return type_; }
        int GetHP(void) const { return hp_; }
        bool GetGhostMode(void) const { return ghost_mode_; }
        bool isSetToDestroy(void) const { return setToDestroy_; }
        // Get bearing direction (direction in which the game object
        // is facing)
        glm::vec3 GetBearing(void) const;


        // Get vector pointing to the right side of the game object
        glm::vec3 GetRight(void) const;


        // Setters
        inline void SetPosition(const vec3& position) { position_ = position; }
        inline void SetScale(float scaleFactor) { scale_.x *= scaleFactor; scale_.y *= scaleFactor; collisionRadius_ *= scaleFactor; }
        inline void SetScaleX(float scale) { scale_.x = scale; collisionRadius_ = (scale_.x + scale_.y) / 7;}
        inline void SetScaleY(float scale) { scale_.y = scale; collisionRadius_ = (scale_.x + scale_.y) / 7;}
        inline void SetParent(GameObject* p) { parent_ = p; }

        void SetRotation(float angle);

        void SetGhostMode(bool mode);

        //void SetOrbitPointOffset(glm::vec3 offset);
        
        void SetOrbitMode(bool mode) { isOrbiting_ = mode; }
        void SetTiling(float scale) { tiling_ = scale; }
        // Reduces hp by damage
        void TakeDamage(int damage);
        //Add Force To Velocity
        void AddForce(vec3&);
        bool RayCollision(vec3 rayObj, GameObject* circObj);
        bool Shoot(void);
        void addChild(GameObject* obj) { children.push_back(obj); obj->parent_ = this; }

        protected:
            // Object's Transform Variables
            glm::vec3 position_;
            glm::vec3 orbitPointOffset;
            glm::vec3 scale_;
            float angle_;
            glm::vec3 velocity_;

            float orbitAngle_;

            bool ghost_mode_;
            bool isOrbiting_;
            bool setToDestroy_;
            GameObject* parent_;

            float maxspeed;

            float tiling_;
            // Objects Health
            int hp_;

            // timer for deleting game object
            Timer* attackCooldown_;
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

            vector<GameObject*> children;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
