#include <cstdlib>
#include <iostream>
#include "glimac/collider.hpp"

Collider::Collider(): position(0,0,0)
{

}

Collider::Collider(glm::vec3 _position): position(_position)
{

}

Collider::TypeCollider Collider::getType()
{
    return type;
}
glm::vec3 Collider::getPosition()
{
    return position;
}

bool Collider::testCollision(Collider& other)
{
    return true;

}

void Collider::setPosition(glm::vec3 newPosition)
{
    position = newPosition;

}

glm::vec3 Collider::getPosition() const
{
    return position;
}

bool Collider::testCollisionWithPoint(glm::vec3 point)
{
    return false;
}

std::vector<glm::vec3> Collider::getVerticesFace()
{

}
