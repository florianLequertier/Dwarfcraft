#ifndef TRACKBALLCAMERA_HPP_INCLUDED
#define TRACKBALLCAMERA_HPP_INCLUDED

#include <cstdlib>
#include "glm.hpp"

class TrackBallCamera{
private:
float m_fDistance;
float m_fAngleX;
float m_fAngleY;


public:
TrackBallCamera(): m_fDistance(5.f), m_fAngleX(0.f), m_fAngleY(0.f)
{}

void moveFront(float delta);
void rotateLeft(float degrees);
void rotateUp(float degrees);

glm::mat4 getViewMatrix() const;

};

#endif // TRACKBALLCAMERA_HPP_INCLUDED
