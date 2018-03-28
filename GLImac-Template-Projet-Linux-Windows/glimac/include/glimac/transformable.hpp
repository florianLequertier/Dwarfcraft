#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include <cstdlib>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glimac/glm.hpp>
#include <glimac/SDLWindowManager.hpp>



class Transformable{

private:
    glm::vec3 position;
    glm::vec3 rotationEuler;
    glm::quat rotation;
    glm::vec3 scale;
public:
    Transformable();

    void setPosition(glm::vec3 newPos);
    void setRotation(glm::vec3 newRot);
    void setScale(glm::vec3 newScale);

    glm::vec3 getPosition() const;
    glm::vec3 getRotationEuler() const;
    glm::quat getRotationQuat() const;
    glm::vec3 getScale() const;

    void computePosition( glm::mat4& model);
    void computeRotation( glm::mat4& model);
    void computeScale( glm::mat4& model);
    void computeTransformation(glm::mat4& model);

};

#endif // TRANSFORMABLE_HPP
