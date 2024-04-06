#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "game_object.h"

namespace game {

    

GameObject::GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture) 
{

    // Initialize all attributes
    position_ = position;
    angle_ = 0.0;
    orbitAngle_ = 0.0;
    isOrbiting_ = false;
    geometry_ = geom;
    shader_ = shader;
    texture_ = texture;
    collision_ = false;
    type_ = Other;
    tiling_ = 1.f;
    setToDestroy_ = false;
    ghost_mode_ = false;
    hp_ = 1;
    velocity_ = vec3(0.0f);
    attackCooldown_ = new Timer();
    parent_ = nullptr;


    int width, height;
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    glBindTexture(GL_TEXTURE_2D, 0);
    float wf = width, hf = height;

    scale_ = glm::vec3(wf / hf, 1.0f, 0.0f);
    orbitPointOffset = position_ + vec3(-scale_.x / 2.f, scale_.y / 2.f, 0.0f);

    deathTimer_ = new Timer();
    collisionRadius_ = (scale_.x + scale_.y)/7;
}

GameObject::~GameObject() {
    delete deathTimer_;
    for each (GameObject * child in children) {
        child->setToDestroy_ = true;
        delete child;
    }
}

glm::vec3 GameObject::GetBearing(void) const {

    glm::vec3 dir(cos(angle_), sin(angle_), 0.0);
    return dir;
}

glm::vec3 GameObject::GetRight(void) const {

    float pi_over_two = glm::pi<float>() / 2.0f;
    glm::vec3 dir(cos(angle_ - pi_over_two), sin(angle_ - pi_over_two), 0.0);
    return dir;
}

void GameObject::SetRotation(float angle){ 

    // Set rotation angle of the game object
    // Make sure angle is in the range [0, 2*pi]
    float two_pi = 2.0f*glm::pi<float>();
    angle = fmod(angle, two_pi);
    if (angle < 0.0){
        angle += two_pi;
    }
    angle_ = angle;
}

void GameObject::Update(double delta_time) {
    SetPosition(GetPosition() + velocity_ * (float)delta_time);
    if (deathTimer_->Finished() || hp_ <= 0) {
        setToDestroy_ = true;
    }
    if (isOrbiting_){
        orbitAngle_ += delta_time*1.5f;

        float pi_over_two = pi<float>() / 2.0f;
        vec3 right(cos(orbitAngle_ - pi_over_two), sin(orbitAngle_ - pi_over_two), 0.0);
        vec3 bearing(cos(orbitAngle_), sin(orbitAngle_), 0.0);

        // finding top left of texture
        right = right * -(scale_.x/2.f);
        bearing = bearing * (scale_.y / 2.f);

        orbitPointOffset = position_ + (right + bearing);
    }
}

void GameObject::TakeDamage(int damage) {
    hp_ -= damage;
}

void GameObject::Render(glm::mat4 view_matrix, double current_time){

    for each (GameObject * child in children) {
        child->Render(view_matrix, current_time);
    }
    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Set up the orbit matrix for the shader
    glm::mat4 orbit_matrix = glm::translate(glm::mat4(1.0f), orbitPointOffset) * glm::rotate(glm::mat4(1.0f), orbitAngle_, glm::vec3(0.0, 0.0, 1.0))/* * glm::translate(glm::mat4(1.0f), -orbitPointOffset)*/;
    
    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix;

    if (isOrbiting_) {
        transformation_matrix = translation_matrix * orbit_matrix * rotation_matrix * scaling_matrix;
    }else {
        transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;
    }
    
    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    shader_->SetUniform1i("ghost_mode", (int)ghost_mode_);

    shader_->SetUniform1f("tiling", tiling_);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    
    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);
    if (tiling_) {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    
    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

void GameObject::SetGhostMode(bool mode) {
    ghost_mode_ = mode;
}

void GameObject::AddForce(vec3& force) {
    velocity_ += force;
    if (length(velocity_) > maxspeed) {
        velocity_ = normalize(velocity_) * 2.0f;
    }
}

bool GameObject::Shoot() {
    attackCooldown_->Finished();
    if (!attackCooldown_->Running())
    {
        attackCooldown_->Start(.6f);
        return true;
    }
    return false;
}

bool GameObject::RayCollision(vec3 rayObj, GameObject* circObj) {
    vec3 dir = rayObj;
    vec3 P = position_;
    vec3 C = circObj->GetPosition();
    float a = dot(dir, dir);
    float b = dot((2.f * dir), P - C);
    float c = dot(P - C, P - C) - pow(circObj->GetCollisionRadius(), 2);
    float disc = pow(b, 2) - 4 * a * c;
    float t1, t2;
    if (disc < 0) {return false;}
    
    t1 = (-b + std::sqrt(disc)) / (2 * a);
    t2 = (-b - std::sqrt(disc)) / (2 * a);

    float closert = (t1 <= t2) ? t1 : t2;
    return (closert < 2.0f);
}


} // namespace game
