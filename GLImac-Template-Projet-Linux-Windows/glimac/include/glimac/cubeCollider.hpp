#ifndef CUBECOLLIDER_HPP_INCLUDED
#define CUBECOLLIDER_HPP_INCLUDED

#include <cstdlib>
#include "glm.hpp"
#include "collider.hpp"


class CubeCollider: public Collider{
private:
float length/*x*/, width/*y*/, height/*z*/;
bool isVisible;

public:
CubeCollider();
CubeCollider(glm::vec3 _position);
void toogleVisibility(bool newVisibility);
virtual bool testCollision(Collider& other);
virtual bool testCollisionCube(CubeCollider& other);
virtual bool testCollision(Collider* other);
bool testCollisionWithPoint(glm::vec3 point);
virtual std::vector<glm::vec3> getVerticesFace();
//virtual bool testCollisionSphere(SphereCollider* other);

};

#endif // CUBECOLLIDER_HPP_INCLUDED
