#include <cstdlib>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "glimac/common.hpp"
#include "glimac/item.hpp"
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include "glimac/directionnalLight.hpp"

DirectionnalLight::DirectionnalLight(glm::vec3 lightDirection, glm::vec3 lightIntensity ): direction(lightDirection), intensity(lightIntensity)
{

}
void DirectionnalLight::setDirection(glm::vec3 newDirection)
{
    direction = newDirection;
}

void DirectionnalLight::setIntensity(glm::vec3 newIntensity)
{
    intensity = newIntensity;
}

glm::vec3 DirectionnalLight::getDirection() const
{
    return direction;
}
glm::vec3 DirectionnalLight::getIntensity() const
{
    return intensity;
}
