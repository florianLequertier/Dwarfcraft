#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include <GL/glew.h>
#include "Program.hpp"
#include "FilePath.hpp"
#include "glm.hpp"
#include <cstdlib>
#include <string>
#include "light.hpp"

class PointLight{
private:

    glm::vec3 position;
    glm::vec3 intensity;

public:
    PointLight(glm::vec3 lightPosition = glm::vec3(0,1,0), glm::vec3 lightIntensity = glm::vec3(255,255,0) );
    void setPosition(glm::vec3 newPosition);
    void setIntensity(glm::vec3 newIntensity);
    glm::vec3 getPosition() const;
    glm::vec3 getIntensity() const;


};

#endif // POINTLIGHT_HPP
