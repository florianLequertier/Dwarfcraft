#ifndef RIGIDBODY_HPP_INCLUDED
#define RIGIDBODY_HPP_INCLUDED

#include <cstdlib>
#include <iostream>
#include "collider.hpp"
#include <vector>
#include "force.hpp"
#include "map.hpp"

struct RayInfo{
    glm::vec3 position; ///positionCollider
    bool intersection;
    glm::vec3 positionHit;
    glm::vec3 normale;

};


class RigidBody{
public :
static float gravityValue;

private:
Collider* collider;
bool useGravity;
bool isSolid;
std::vector<Force*> tabForce;


public:
RigidBody();
~RigidBody();
RigidBody(bool _useGravity, bool _isSolid);
RigidBody(glm::vec3 positionCollider, bool _useGravity, bool _isSolid);
bool testCollision(Collider& other);
bool testCollision(Collider* other);
Collider* getCollider() const;

void addForce(Force* newForce);
void removeForce(int index);
void updateForce(int time);
Force forceResulting();
Force getForce(int index) const;
void setPosition(glm::vec3 newPosition);
glm::vec3 getPosition()const;
bool getBoolGravity() const;
void setBoolGravity(bool newState);
///retourne la position du cube intersecté :
RayInfo testRayTracerWithEnvironment( Map& environment,glm::vec3 positionBegin, glm::vec3 direction, float pas, int nbIterration);
bool rayTracerWithEnvironment(RayInfo& info, Map& environment,glm::vec3 positionBegin, glm::vec3 direction, float pas, int nbIterration);
bool rayTracerFace(RayInfo& info, const float limitDistance, const glm::vec3& fA, const glm::vec3& fB, const glm::vec3& fC, const glm::vec3 fD, const glm::vec3& A, const glm::vec3& B);
bool isPointIntoFace(const glm::vec3& point, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C, const glm::vec3& D);
bool isPointIntoTriangle(const glm::vec3& point, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);
bool isLineIntersectingFace(const glm::vec3& fA, const glm::vec3& fB, const glm::vec3& fC, const glm::vec3& A, const glm::vec3& B, glm::vec3& result );




};


#endif // RIGIDBODY_HPP_INCLUDED
