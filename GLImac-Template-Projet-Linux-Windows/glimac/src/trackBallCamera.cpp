#include <cstdlib>
#include "glimac/trackBallCamera.hpp"
#include "glimac/glm.hpp"

void TrackBallCamera::moveFront(float delta)
{
    m_fDistance += delta;
}
void TrackBallCamera::rotateLeft(float degrees)
{
    m_fAngleX += glm::radians(degrees);
}
void TrackBallCamera::rotateUp(float degrees)
{
    m_fAngleY += glm::radians(degrees);
}
glm::mat4 TrackBallCamera::getViewMatrix() const
{
    glm::mat4 viewMatrix;

    viewMatrix = glm::translate(glm::mat4(1), glm::vec3(0.f,0.f, m_fDistance) );
    viewMatrix = glm::rotate(viewMatrix, m_fAngleX, glm::vec3(1,0,0));
    viewMatrix = glm::rotate(viewMatrix, m_fAngleY, glm::vec3(0,1,0));

    return viewMatrix;

}
