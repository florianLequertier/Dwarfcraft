#ifndef DIRECTIONNALLIGHT_HPP
#define DIRECTIONNALLIGHT_HPP

#include <GL/glew.h>
#include "Program.hpp"
#include "FilePath.hpp"
#include "glm.hpp"
#include <cstdlib>
#include <string>
#include "light.hpp"

class DirectionnalLight{
private:

    glm::vec3 direction;
    glm::vec3 intensity;

public:
    DirectionnalLight(glm::vec3 lightDirection = glm::vec3(3,10,0), glm::vec3 lightIntensity = glm::vec3(0.5,0.5,0) );
    void setDirection(glm::vec3 newDirection);
    void setIntensity(glm::vec3 newIntensity);
    glm::vec3 getDirection() const;
    glm::vec3 getIntensity() const;


};

#endif // DIRECTIONNALLIGHT_HPP
