#include <cstdlib>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glimac/glm.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/transformable.hpp>

Transformable::Transformable(): position(0,0,0), scale(1,1,1), rotationEuler(0,0,0)
{
    rotation = glm::quat(rotationEuler);
}

void Transformable::setPosition(glm::vec3 newPos)
{
    position = newPos;
}

void Transformable::setRotation(glm::vec3 newRot)
{
    rotationEuler = newRot;
    rotation =  glm::quat(rotationEuler);
}

void Transformable::setScale(glm::vec3 newScale)
{
    scale = newScale;
}

glm::vec3 Transformable::getPosition() const
{
    return position;
}
glm::vec3 Transformable::getRotationEuler() const
{
    return rotationEuler;
}
glm::quat Transformable::getRotationQuat() const
{
    return rotation;
}
glm::vec3 Transformable::getScale() const
{
    return scale;
}

void Transformable::computePosition( glm::mat4& model)
{
    model = glm::translate(model, position);
}

void Transformable::computeRotation( glm::mat4& model)
{
    glm::mat4 tempMatRot = glm::toMat4(rotation);
    model = model*tempMatRot;
}

void Transformable::computeScale( glm::mat4& model)
{
    model = glm::scale(model, scale);
}

void Transformable::computeTransformation(glm::mat4& model)
{
    computePosition(model);
    computeRotation(model);
    computeScale(model);
}
