#pragma once

#include <GL/glew.h>
#include "glm.hpp"

namespace glimac {

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    ShapeVertex(): position(glm::vec3(0,0,0)), normal(glm::vec3(0,0,0)), texCoords(glm::vec2(0,0))
    {

    }

    ShapeVertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _texCoord): position(_position), normal(_normal), texCoords(_texCoord)
    {

    }
};

}
