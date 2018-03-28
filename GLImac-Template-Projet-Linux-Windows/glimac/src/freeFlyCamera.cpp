#include <cstdlib>
#include "glimac/glm.hpp"
#include "glimac/freeFlyCamera.hpp"


void FreeFlyCamera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(cos(m_fTheta)*sin(m_fPhi), sin(m_fTheta), cos(m_fTheta)*cos(m_fPhi));
    m_LeftVector = glm::vec3( sin(m_fPhi + glm::pi<float>()), 0, cos(m_fPhi + glm::pi<float>()) );
    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}


FreeFlyCamera::FreeFlyCamera(): m_Position(0,0,0), m_fPhi(glm::pi<float>()), m_fTheta(0.f)
{
    computeDirectionVectors();
}

void FreeFlyCamera::moveLeft(float t)
{
    m_Position += t*m_LeftVector;
}
void FreeFlyCamera::moveFront(float t)
{
     m_Position += t*m_FrontVector;
}
void FreeFlyCamera::rotateLeft(float degrees)
{
    m_fPhi += glm::radians(degrees);
}
void FreeFlyCamera::rotateUp(float degrees)
{
/*
    if(m_fTheta+glm::radians(degrees) > glm::pi<float>()/2)
    {
        return;
    }
    if(m_fTheta+glm::radians(degrees) < -glm::pi<float>()/2 )
    {
        return;
    }*/

    m_fTheta += glm::radians(degrees);

}
glm::mat4 FreeFlyCamera::FreeFlyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}
