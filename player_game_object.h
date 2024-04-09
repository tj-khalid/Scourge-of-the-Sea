#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include "collectible_game_object.h"
#include "text_game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            ~PlayerGameObject();
            // Update function for moving the player object around
            void Update(double delta_time) override;

            void CollideWith(GameObject*) override;

            void AddCollectible(CollectibleGameObject*);

            bool isInvicible() { return invicibiltyTimer_->Running(); }

            bool Shoot2();

            void SetInvincibleTex(GLuint text) { invincibleTexture_ = text; }

            void SetTexture(GLuint texture) { texture_ = texture; }

            glm::vec3 GetVelocity(void) { return velocity_; }

            inline int getCoinCount() const { return coinCount_; };
            inline void setCoinCount(int s) { coinCount_ = s;  Coin_HUD->SetText("Coins: " + to_string(coinCount_)); }
            inline void setHP_UI(TextGameObject* t) { HP_UI = t; }
            inline void setCoin_HUD(TextGameObject* t) { Coin_HUD = t; }
        protected:
            int coinCount_;
            int boozeCount_;
            int chestCount_;
            Timer* invicibiltyTimer_;
            Timer* secondAttackCooldown_;

            TextGameObject* Coin_HUD;
            TextGameObject* HP_UI;

            GLuint invincibleTexture_;
            GLuint normTexture_;
    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
