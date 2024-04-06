#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>

#include <path_config.h>

#include "sprite.h"
#include "shader.h"
#include "player_game_object.h"
#include "enemy_game_object.h"
#include "collectible_game_object.h"
#include "bullet.h"
#include "game.h"
#include "shark.h"
#include "EnemyShip.h"


using namespace std; 

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Game Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}

void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();

    spawnCollectibleTimer_ = new Timer();
    spawnEnemyTimer_ = new Timer();
    closeTimer_ = new Timer();

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;
}


Game::~Game()
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    delete sprite_;
    delete spawnCollectibleTimer_;
    delete spawnEnemyTimer_;
    delete closeTimer_;
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }

    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();

}


void Game::Setup(void)
{

    // Setup the game world

    // Load textures
    SetAllTextures();

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
    SpawnObject(GameObject::Player, glm::vec3(0.0f, 0.f, 0.0f), tex_[5], 3/1.5f);
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
    player->SetInvincibleTex(tex_[9]);

    // Setup other objects


   Shark* SharkTest =  (Shark*)SpawnObject(GameObject::Shark, glm::vec3(4.0f, -4.5f, 0.0f), tex_[12], 1.f/2.f);
   cout << SharkTest->getState() << std::endl;

    //SpawnObject(GameObject::EnemyShip, glm::vec3(-2.5f, -1.5f, 0.0f), tex_[6], 1.f / 2.f);
    /*
    
    SpawnObject(GameObject::Enemy, glm::vec3(-2.5f, -2.0f, 0.0f), tex_[6], 1.f / 2.f);
    SpawnObject(GameObject::Enemy, glm::vec3(2.0f, 2.0f, 0.0f), tex_[6], 1.f / 2.f);

    SpawnObject(GameObject::Collectible, glm::vec3(-1.5f, 1.5f, 0.0f), tex_[8], 1.f / 3.f);
    SpawnObject(GameObject::Collectible, glm::vec3(1.5f, -1.5f, 0.0f), tex_[8], 1.f / 3.f);
    SpawnObject(GameObject::Collectible, glm::vec3(-1.5f, -2.5f, 0.0f), tex_[8], 1.f / 3.f);
    SpawnObject(GameObject::Collectible, glm::vec3(2.5f, 1.5f, 0.0f), tex_[8], 1.f / 3.f);
    SpawnObject(GameObject::Collectible, glm::vec3(2.5f, -1.5f, 0.0f), tex_[8], 1.f / 3.f);
    
    */
    spawnCollectibleTimer_->Start(3);
    spawnEnemyTimer_->Start(5);
    
    // Setup background
    // In this specific implementation, the background is always the
    // last object
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
    background->SetScale(20.0f * 7.f);
    background->SetTiling(7.f);
    game_objects_.push_back(background);
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


void Game::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        std::cout << "Cannot load texture " << fname << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::SetAllTextures(void)
{
    // Load all textures that we will need
    // Declare all the textures here
    const char *texture[] = {
        "/textures/destroyer_red.png", 
        "/textures/destroyer_green.png", 
        "/textures/destroyer_blue.png", 
        "/textures/water.png", 
        "/textures/orb.png",
        "/textures/player_ship.png",
        "/textures/ship.png",
        "/textures/explosion.png",
        "/textures/coin.png",
        "/textures/barrel.png",
        "/textures/axe.png",
        "/textures/cannon ball.png",
        "/textures/hai-fin-shadow.png",
        "/textures/chest.png"

    
    };
    // Get number of declared textures
    int num_textures = sizeof(texture) / sizeof(char *);
    // Allocate a buffer for all texture references
    tex_ = new GLuint[num_textures];
    glGenTextures(num_textures, tex_);
    // Load each texture
    for (int i = 0; i < num_textures; i++){
        SetTexture(tex_[i], (resources_directory_g+std::string(texture[i])).c_str());
    }
    // Set first texture in the array as default
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Handle user input
        HandleControls(delta_time);

        // Update all the game objects
        Update(delta_time);

        // Render all the game objects
        Render();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}


void Game::HandleControls(double delta_time)
{
    // Makes sure the first game object is the player before controlling
    if (!game_objects_[0]->GetObjectType() == 0)
    {
        return;
    }
    // Get player game object
    PlayerGameObject *player =(PlayerGameObject *) game_objects_[0];
    // Get current position and angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetRotation();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();
    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = delta_time*1500.0;
    float acceleration = delta_time*2.5f;
    float angle_increment = (glm::pi<float>() / 1550.0f)*speed;

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        player->AddForce((dir * acceleration));
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        player->AddForce(-(dir * acceleration));
    }
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        player->SetRotation(angle - angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        player->SetRotation(angle + angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS) {
        player->AddForce(-(player->GetRight() * acceleration));
    }
    if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
        player->AddForce((player->GetRight() * acceleration));
    }
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (player->Shoot()) {
            GameObject* bulletRight = SpawnObject(GameObject::Bullet, player->GetPosition(), tex_[11], .27f, game_objects_.size() - 2);
            bulletRight->SetRotation(player->GetAngle() - glm::pi<float>()/2);
            GameObject* bulletLeft = SpawnObject(GameObject::Bullet, player->GetPosition(), tex_[11], .27f, game_objects_.size() - 2);
            bulletLeft->SetRotation(player->GetAngle() + glm::pi<float>() / 2);
        }
    }

}


void Game::Update(double delta_time){

    // Update time
    current_time_ += delta_time;

    if (closeTimer_->Finished()) {
        std::cout << "Game Over!";
        glfwSetWindowShouldClose(window_, true);
        return;
    }

    // Update all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];
        
        // Update the current game object
        current_game_object->Update(delta_time);

        // If the game object is set to destroy, delete game object
        if (current_game_object->isSetToDestroy()) {
            switch (current_game_object->GetObjectType()){
            default:
                delete game_objects_[i];
                game_objects_.erase(game_objects_.begin() + i);
                break;
            case GameObject::Player:
                closeTimer_->Start(5);
            case GameObject::Enemy:
            case GameObject::EnemyShip:
                glm::vec3 curPos = current_game_object->GetPosition();
                
                delete game_objects_[i];
                game_objects_.erase(game_objects_.begin() + i);
                SpawnObject(GameObject::Other, curPos, tex_[7], 1 / 1.f, i);
                game_objects_[i]->GetDeathTimer()->Start(5);
                
                break;
            }
            i--;
            continue;
        }

        // Check for collision with other game objects
        // Note the loop bounds: we avoid testing the last object since
        // it's the background covering the whole game world
        for (int j = i + 1; j < (game_objects_.size()-1); j++) {
            GameObject* other_game_object = game_objects_[j];
            // Compute distance between object i and object j
            float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());

            if (other_game_object->GetObjectType() != GameObject::Bullet) {
                // If distance is below a threshold, we have a collision
                float threshold = current_game_object->GetCollisionRadius() + other_game_object->GetCollisionRadius();

                if (distance < threshold) {
                    current_game_object->CollideWith(other_game_object);
                    other_game_object->CollideWith(current_game_object);
                }
            }
            else if(current_game_object->GetObjectType() != GameObject::Bullet){
                if (RayCollision(other_game_object, current_game_object)) {
                    current_game_object->CollideWith(other_game_object);
                    other_game_object->CollideWith(current_game_object);
                }
            }
            
            if (i == 0) {
                switch (other_game_object->GetObjectType())
                {
                case GameObject::Enemy:
                case GameObject::EnemyShip:
                case GameObject::Shark:
                    EnemyGameObject* enemy = (EnemyGameObject*)other_game_object;

                    if (enemy->getState() == EnemyGameObject::Intercepting){break;}

                    if (length(enemy->GetPosition() - game_objects_[0]->GetPosition()) < enemy->getDetectionRadius()) {
                        enemy->chaseTarget(game_objects_[0]);
                    }
                break;
                }
            }
            }
            
    }

    if (spawnCollectibleTimer_->Finished()){
        int random_angle = std::rand() % 360 + 1;
        int random_collectable = std::rand() % 3;
        glm::vec3 randomCollectableSpawn = (game_objects_[0]->GetPosition() + glm::vec3(std::cos(random_angle), std::sin(random_angle), 0) * 3.0f) ;
        
        if (random_collectable == 0) {
            CollectibleGameObject* newCollectible = (CollectibleGameObject*)SpawnObject(GameObject::Collectible, randomCollectableSpawn, tex_[8], 2.f / 3.f, game_objects_.size() - 1);
            newCollectible->setCollectType(CollectibleGameObject::Coin);
        }
        else if (random_collectable == 1) {
            CollectibleGameObject* newCollectible = (CollectibleGameObject*)SpawnObject(GameObject::Collectible, randomCollectableSpawn, tex_[9], 2.f / 3.f, game_objects_.size() - 1);
            newCollectible->setCollectType(CollectibleGameObject::Booze);
        }
        else if (random_collectable == 2) {
            CollectibleGameObject* newCollectible = (CollectibleGameObject*)SpawnObject(GameObject::Collectible, randomCollectableSpawn, tex_[13], 2.f / 3.f, game_objects_.size() - 1);
            newCollectible->setCollectType(CollectibleGameObject::Chest);
        }

        
        spawnCollectibleTimer_->Start(6);
    }

    if (spawnEnemyTimer_->Finished()) {
        int random_angle = std::rand() % 360 + 1;
        int random_enemy = std::rand() % 2;
        glm::vec3 randomEnemySpawn = (game_objects_[0]->GetPosition() + glm::vec3(std::cos(random_angle), std::sin(random_angle), 0) * 5.0f);
        switch (random_enemy)
        {
        case 0:
            SpawnObject(GameObject::Shark, randomEnemySpawn, tex_[12], 1.f / 2.f, game_objects_.size() - 1);
            break;
        case 1:
            EnemyGameObject* enemy = (EnemyGameObject*) SpawnObject(GameObject::EnemyShip, randomEnemySpawn, tex_[6], 1.f / 2.f, game_objects_.size() - 1);
            enemy->chaseTarget(game_objects_[0]);
            break;
        }
        spawnEnemyTimer_->Start(5);
    }       
 }


void Game::Render(void){

    // Clear background
    glClearColor(viewport_background_color_g.r,
                 viewport_background_color_g.g,
                 viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix;
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    } else {
        float aspect_ratio = ((float) height)/((float) width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f/aspect_ratio, 1.0f));
    }

    // Set view to zoom out, centered by default at 0,0
    float camera_zoom = 0.2f;
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));
    glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix;

    glm::mat4 playerTrans = glm::translate(glm::mat4(1.0f), -game_objects_[0]->GetPosition());
    view_matrix = view_matrix * playerTrans;

    // Render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        game_objects_[i]->Render(view_matrix, current_time_);
    }
}


GameObject* Game::SpawnObject(GameObject::ObjectType type, const vec3& position, GLuint texture, float scale, int index) {
    GameObject* newObject;
    switch (type) {
    case GameObject::Player:
        newObject = new PlayerGameObject(position, sprite_, &sprite_shader_, texture);
        break;
    case GameObject::Enemy:
        newObject = new EnemyGameObject(position, sprite_, &sprite_shader_, texture);
        break;
    case GameObject::Collectible:
        newObject = new CollectibleGameObject(position, sprite_, &sprite_shader_, texture);
        break;
    case GameObject::Bullet:
        newObject = new Bullet(position, sprite_, &sprite_shader_, texture);
        break;
    case GameObject::Other:
        newObject = new GameObject(position, sprite_, &sprite_shader_, texture);
        break;
    case GameObject::Shark:
        newObject = new Shark(position, sprite_, &sprite_shader_, texture);
        break;
    case GameObject::EnemyShip:
        newObject = new EnemyShip(position, sprite_, &sprite_shader_, texture);
        break;
    }

    game_objects_.insert(game_objects_.begin() + index, newObject);

    float pi_over_two = glm::pi<float>() / 2.0f;
    game_objects_[index]->SetRotation(pi_over_two);
    game_objects_[index]->SetScale(scale);
    return newObject;
}


GameObject* Game::SpawnObject(GameObject::ObjectType type, const glm::vec3& position, GLuint texture, float scale) {
    return Game::SpawnObject(type, position, texture, scale, game_objects_.size());
}

bool Game::RayCollision(GameObject* rayObj, GameObject* circObj) {
    vec3 dir = rayObj->GetBearing();
    vec3 P = rayObj->GetPosition();
    vec3 C = circObj->GetPosition();
    float a = dot(dir, dir);
    float b = dot((2.f * dir), P - C);
    float c = dot(P - C, P - C) - (circObj->GetCollisionRadius(), 2);
    float disc = pow(b, 2) - 4 * a * c;
    float t1, t2;
    if (disc < 0) { return false; }
     
    t1 = (-b + std::sqrt(disc)) / (2 * a);
    t2 = (-b - std::sqrt(disc)) / (2 * a);
    
    float closert = (t1 <= t2) ? t1 : t2;
    return (closert > 0 && closert < 0.25f);
}

} // namespace game
