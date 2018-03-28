#ifndef COLLIDER_HPP_INCLUDED
#define COLLIDER_HPP_INCLUDED

#include <cstdlib>
#include <vector>
#include "glm.hpp"



class Collider{
public:
enum TypeCollider{CUBE, SPHERE};
protected:
TypeCollider type;
glm::vec3 position;
public:
Collider();
Collider(glm::vec3 _position);
TypeCollider getType();
glm::vec3 getPosition();
virtual bool testCollision(Collider& other) = 0;
virtual void setPosition(glm::vec3 newPosition);
virtual glm::vec3 getPosition() const;
virtual bool testCollisionWithPoint(glm::vec3 point);
virtual std::vector<glm::vec3> getVerticesFace();
//virtual bool testCollisionCube(CubeCollider* other) = 0;
//virtual bool testCollisionSphere(SphereCollider* other) = 0;

};


#endif // COLLIDER_HPP_INCLUDED
