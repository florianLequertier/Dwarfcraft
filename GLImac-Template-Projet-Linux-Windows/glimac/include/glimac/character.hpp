#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <cstdlib>
#include "glm.hpp"
#include "camera.h"
#include "rigidBody.hpp"
#include "map.hpp"


class Character{
public:
enum CHARACTER_MOVEMENT{FORWARD, BACKWARD, RIGHT, LEFT};
protected:
Camera head;
glm::vec3 direction;
float walkSpeed;
float runSpeed;
glm::vec3 foot_forward;
glm::vec3 foot_right;
RigidBody body;
glm::vec3 position;
bool isRunning;
bool canJump;
bool jumpInit;
int tpPreJump;
bool isSwimming;


public:
Character();
Character(int Time);
void prepareMove(CHARACTER_MOVEMENT mouvement);
virtual void updateLookAt(float x, float y);
void setSpeed(float walkSpeed, float runSpeed);
void updatePosition();
void updatePosition(Map& world);
Camera& getHead();
void setPosition(glm::vec3 newPosition);
void setIsRunning(bool newState);
bool getIsRunning()const;
glm::vec3 getPosition();
void jump(int time);
void updateForces(int time);
void setUseGravity(bool useGravity);
void setUseGravity(bool useGravity, int time);
void toogleIsSwimming(bool newState);



};

#endif // CHARACTER_HPP_INCLUDED
