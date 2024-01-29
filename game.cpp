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
#include "game.h"

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
    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[5]));
    float pi_over_two = glm::pi<float>() / 2.0f;
    game_objects_[0]->SetRotation(pi_over_two);
    game_objects_[0]->SetScaleX(1.9f);

    // Setup other objects
    game_objects_.push_back(new EnemyGameObject(glm::vec3(-1.0f, 1.0f, 0.0f), sprite_, &sprite_shader_, tex_[6]));
    game_objects_[1]->SetRotation(pi_over_two);
    game_objects_[1]->SetScaleX(1.9f/1.5f);
    game_objects_[1]->SetScaleY(1.0f/1.5f);

    game_objects_.push_back(new EnemyGameObject(glm::vec3(1.0f, -0.5f, 0.0f), sprite_, &sprite_shader_, tex_[6]));
    game_objects_[2]->SetRotation(pi_over_two);
    game_objects_[2]->SetScaleX(1.9f / 1.5f);
    game_objects_[2]->SetScaleY(1.0f / 1.5f);

    game_objects_.push_back(new EnemyGameObject(glm::vec3(-0.5f, -2.5f, 0.0f), sprite_, &sprite_shader_, tex_[6]));
    game_objects_[3]->SetRotation(pi_over_two);
    game_objects_[3]->SetScaleX(1.9f / 1.5f);
    game_objects_[3]->SetScaleY(1.0f / 1.5f);

    game_objects_.push_back(new EnemyGameObject(glm::vec3(-1.5f, -1.0f, 0.0f), sprite_, &sprite_shader_, tex_[6]));
    game_objects_[4]->SetRotation(pi_over_two);
    game_objects_[4]->SetScaleX(1.9f / 1.5f);
    game_objects_[4]->SetScaleY(1.0f / 1.5f);

    game_objects_.push_back(new EnemyGameObject(glm::vec3(1.0f, 1.0f, 0.0f), sprite_, &sprite_shader_, tex_[6]));
    game_objects_[5]->SetRotation(pi_over_two);
    game_objects_[5]->SetScaleX(1.9f / 1.5f);
    game_objects_[5]->SetScaleY(1.0f / 1.5f);

    game_objects_.push_back(new CollectibleGameObject(glm::vec3(1.5f, 1.5f, 0.0f), sprite_, &sprite_shader_, tex_[8]));
    game_objects_[6]->SetRotation(pi_over_two);
    game_objects_[6]->SetScaleX(0.5f);
    game_objects_[6]->SetScaleY(0.5f);

    game_objects_.push_back(new CollectibleGameObject(glm::vec3(1.5f, 1.5f, 0.0f), sprite_, &sprite_shader_, tex_[8]));
    game_objects_[7]->SetRotation(pi_over_two);
    game_objects_[7]->SetScaleX(0.5f);
    game_objects_[7]->SetScaleY(0.5f);

    game_objects_.push_back(new CollectibleGameObject(glm::vec3(1.5f, 1.5f, 0.0f), sprite_, &sprite_shader_, tex_[8]));
    game_objects_[8]->SetRotation(pi_over_two);
    game_objects_[8]->SetScaleX(0.5f);
    game_objects_[8]->SetScaleY(0.5f);

    game_objects_.push_back(new CollectibleGameObject(glm::vec3(1.5f, 1.5f, 0.0f), sprite_, &sprite_shader_, tex_[8]));
    game_objects_[9]->SetRotation(pi_over_two);
    game_objects_[9]->SetScaleX(0.5f);
    game_objects_[9]->SetScaleY(0.5f);

    game_objects_.push_back(new CollectibleGameObject(glm::vec3(1.5f, 1.5f, 0.0f), sprite_, &sprite_shader_, tex_[8]));
    game_objects_[10]->SetRotation(pi_over_two);
    game_objects_[10]->SetScaleX(0.5f);
    game_objects_[10]->SetScaleY(0.5f);

    game_objects_.push_back(new CollectibleGameObject(glm::vec3(1.5f, 1.5f, 0.0f), sprite_, &sprite_shader_, tex_[8]));
    game_objects_[11]->SetRotation(pi_over_two);
    game_objects_[11]->SetScaleX(0.5f);
    game_objects_[11]->SetScaleY(0.5f);

    game_objects_.push_back(new CollectibleGameObject(glm::vec3(1.5f, 1.5f, 0.0f), sprite_, &sprite_shader_, tex_[8]));
    game_objects_[12]->SetRotation(pi_over_two);
    game_objects_[12]->SetScaleX(0.5f);
    game_objects_[12]->SetScaleY(0.5f);

    // Setup background
    // In this specific implementation, the background is always the
    // last object
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
    background->SetScaleX(20.0);
    background->SetScaleY(20.0);
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
        "/textures/stars2.png", 
        "/textures/orb.png",
        "/textures/blueship2.png",
        "/textures/blueship3.png",
        "/textures/explosion.png",
        "/textures/coin.png"};
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
    GameObject *player = game_objects_[0];
    // Get current position and angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetRotation();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();
    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = delta_time*1000.0;
    float motion_increment = 0.001*speed;
    float angle_increment = (glm::pi<float>() / 1800.0f)*speed;

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        player->SetPosition(curpos + motion_increment*dir);
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        player->SetPosition(curpos - motion_increment*dir);
    }
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        player->SetRotation(angle - angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        player->SetRotation(angle + angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS) {
        player->SetPosition(curpos - motion_increment*player->GetRight());
    }
    if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
        player->SetPosition(curpos + motion_increment*player->GetRight());
    }
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
}


void Game::Update(double delta_time)
{

    // Update time
    current_time_ += delta_time;
    // Temporary solution to ending the game, there will be 3 objects left when player dies

    // Update all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        // Update the current game object
        current_game_object->Update(delta_time);

        // If the timer within the game object is finished, delete game object
        if (current_game_object->GetDeathTimer()->Finished()) {
            delete current_game_object->GetDeathTimer();
            game_objects_.erase(game_objects_.begin() + i);
            if (i = 0) {
                std::cout << "Game Over!";
                glfwDestroyWindow(window_);
            }
            delete current_game_object;
        }

        // If a game object with collision has 0 hp, delete the game object
        // Create a explosion object in their place and set 5 sec timer to delete
        if (current_game_object->GetCollision()) {
            if (current_game_object->GetHP() == 0){
                game_objects_.erase(game_objects_.begin() + i);
                game_objects_.insert(game_objects_.begin() + 1, new GameObject(current_game_object->GetPosition(), sprite_, &sprite_shader_, tex_[7]));
                game_objects_.at(1)->GetDeathTimer()->Start(5);
                delete current_game_object->GetDeathTimer();
                delete current_game_object;
            }
        }

        // Check for collision with other game objects
        // Note the loop bounds: we avoid testing the last object since
        // it's the background covering the whole game world
        for (int j = i + 1; j < (game_objects_.size()-1); j++) {
            GameObject* other_game_object = game_objects_[j];

            // Compute distance between object i and object j
            float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());
            // If distance is below a threshold, we have a collision
            float threshold = current_game_object->GetCollisionRadius() + other_game_object->GetCollisionRadius();
            if (distance < threshold) {
                // First check whether the game objects are collidable
                switch (other_game_object->GetObjectType())
                {
                case 1:
                    // Both Objects take damage
                    if (current_game_object->GetObjectType() == 0) {
                        PlayerGameObject* player = (PlayerGameObject*)current_game_object;
                        if (!player->isInvicible()) {
                            player->TakeDamage(1);
                        }
                        other_game_object->TakeDamage(1);
                    }
                    break;
                case 2:
                    if (current_game_object->GetObjectType() == 0) {
                        //players get collectible
                        PlayerGameObject* player = (PlayerGameObject*)current_game_object;
                        game_objects_.erase(game_objects_.begin() + j);
                        delete other_game_object;
                        player->AddCollectible();
                    }
                default:
                    break;
                }
                
            }
        }
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
    float camera_zoom = 0.25f;
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));
    glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix;

    // Render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        game_objects_[i]->Render(view_matrix, current_time_);
    }
}
      
} // namespace game
