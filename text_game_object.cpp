#include <glm/gtc/matrix_transform.hpp>

#include "text_game_object.h"

namespace game {

TextGameObject::TextGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture) : GameObject(position, geom, shader, texture) {

    text_ = "";
}


void TextGameObject::Render(glm::mat4 view_matrix, double current_time) {

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

    glm::mat4 parent_transformation_matrix;
    if (parent_ != nullptr)
    {
        //glm::mat4 parent_rotation_matrix = glm::rotate(glm::mat4(1.0f), parent_->GetRotation(), glm::vec3(0.0, 0.0, 1.0));
        glm::mat4 parent_translation_matrix = glm::translate(glm::mat4(1.0f), parent_->GetPosition());
        parent_transformation_matrix = parent_translation_matrix;
    }
    else {
        parent_transformation_matrix = glm::mat4(1.0f);
    }

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = parent_transformation_matrix * translation_matrix * rotation_matrix * scaling_matrix;



    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Set the text
#define TEXT_LENGTH 40
    // Set text length
    int final_size = text_.size();
    if (final_size > TEXT_LENGTH){
        final_size = TEXT_LENGTH;
    }
    shader_->SetUniform1i("text_len", final_size);

    // Set the text data
    GLint data[TEXT_LENGTH];
    for (int i = 0; i < final_size; i++){
        data[i] = text_[i];
    }
    shader_->SetUniformIntArray("text_content", final_size, data);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}


std::string TextGameObject::GetText(void) const {

    return text_;
}


void TextGameObject::SetText(std::string text){
    text_ = text;
}

} // namespace game
