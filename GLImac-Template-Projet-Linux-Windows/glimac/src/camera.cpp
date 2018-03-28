#include <cstdlib>
#include <GL/glew.h>
#include "glimac/glm.hpp"
#include <algorithm> //std::sort
#include <cmath>

#include "glimac/camera.h"
#include "glimac/opengl_tool.h"


Camera::Camera() : m_position(glm::vec3(0.f, 0.f, 0.f)), m_forward(glm::vec3(0.f, 0.f, 1.f)), m_right(glm::vec3(-1.f, 0.f, 0.f)), m_alpha(0.f), m_beta(0.f)
{

}
void Camera::setPosition(glm::vec3 position)
{
	m_position = position;
}
void Camera::setForawrd(glm::vec3 forward)
{
	m_forward = forward;
}
void Camera::setRight(glm::vec3 up)
{
	m_right = up;
}

glm::vec3 Camera::getPosition() const
{
	return m_position;
}
glm::vec3 Camera::getForawrd() const
{
	return m_forward;
}
glm::vec3 Camera::getRight() const
{
	return m_right;
}

void Camera::move(CAMERA_MOVEMENT mouvement)
{
	switch(mouvement)
	{
	case FORWARD :
		m_position += m_forward * m_vitesseDeplacement;
		break;
	case BACKWARD :
		m_position -= m_forward * m_vitesseDeplacement;
		break;
	case RIGHT:
		m_position += m_right * m_vitesseDeplacement;
		break;
	case LEFT:
		m_position -= m_right * m_vitesseDeplacement;
		break;
	case UP :
		m_position += glm::cross(m_right, m_forward) * m_vitesseDeplacement; //cross(right, foward) = vecteur up.
		break;
	case DOWN :
		m_position -= glm::cross(m_right, m_forward) * m_vitesseDeplacement;
		break;


	}
}
void Camera::pan(float amplitude)
{
	glm::vec3 target(sinf(amplitude), 0.f, cosf(amplitude));
	m_forward += (target - glm::vec3(0.f,0.f,1.f));

	m_right = glm::cross(m_forward, glm::vec3(0.f, 1.f, 0.f));
}
void Camera::roll(float amplitude)
{
	glm::vec3 target(0.f, sinf(amplitude), cosf(amplitude));
	m_forward += (target - glm::vec3(0.f, 0.f, 1.f));
}

void Camera::updateLookAt(float x, float y)
{
	m_alpha -= y*PI / 180;
	m_beta += x*PI / 180;

	//if (m_alpha > 3 * PI / 4)	{ m_alpha = 3 * PI / 4; }
	//else if (m_alpha < -3 * PI / 4)	{ m_alpha = -3 * PI / 4; }
	/*
	cam->direction.x = sin(cam->inclinaison*PI / 180) * cos(cam->rotation*PI / 180);
	cam->direction.y = sin(cam->inclinaison*PI / 180) * sin(cam->rotation*PI / 180);
	cam->direction.z = cos(cam->inclinaison*PI / 180);*/

	m_forward = glm::vec3(-cosf(m_alpha)*sinf(m_beta), sinf(m_alpha), cosf(m_alpha)*cosf(m_beta));
	m_right = glm::cross(m_forward, glm::vec3(0,1,0));
}

float Camera::getvitesseDeplacement() const
{
	return m_vitesseDeplacement;
}
void Camera::setVitesseDeplacement(float vitesseDeplacement)
{
	m_vitesseDeplacement = vitesseDeplacement;
}

void Camera::setVitesseLook(float vitesseLook)
{
	m_vitesseLook = vitesseLook;
}

float Camera::getVitesseLook() const
{
	return m_vitesseLook;
}
