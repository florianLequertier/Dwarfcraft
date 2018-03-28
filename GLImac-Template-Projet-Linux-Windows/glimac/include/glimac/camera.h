#ifndef CAMERA_ONCE
#define CAMERA_ONCE

#include <GL/glew.h>
#include "glm.hpp"



class Camera{

public : enum CAMERA_MOVEMENT {FORWARD, BACKWARD, RIGHT, LEFT, UP, DOWN};

protected:
	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_position;
	float m_vitesseDeplacement;
	float m_vitesseLook;
	float m_alpha;
	float m_beta;

public:
	Camera();
	void setPosition(glm::vec3 position);
	void setForawrd(glm::vec3 forward);
	void setRight(glm::vec3 up);
	void setVitesseDeplacement(float vitesseDeplacement);
	void setVitesseLook(float vitesseLook);

	glm::vec3 getPosition() const;
	glm::vec3 getForawrd() const;
	glm::vec3 getRight() const;
	float getvitesseDeplacement() const;
	float getVitesseLook() const;

	void move(CAMERA_MOVEMENT mouvement);
	void pan(float amplitude);
	void roll(float amplitude);
	virtual void updateLookAt(float x, float y);


};

#endif
