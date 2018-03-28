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
#include "glimac/pointLight.hpp"


PointLight::PointLight(glm::vec3 lightPosition, glm::vec3 lightIntensity ): position(lightPosition), intensity(lightIntensity)
{

}

void PointLight::setPosition(glm::vec3 newPosition)
{
    position = newPosition;
}

void PointLight::setIntensity(glm::vec3 newIntensity)
{
    intensity = newIntensity;
}

glm::vec3 PointLight::getPosition() const
{
    return position;
}
glm::vec3 PointLight::getIntensity() const
{
    return intensity;
}
