#ifndef VERTEXTYPE_HPP_INCLUDED
#define VERTEXTYPE_HPP_INCLUDED

#include <cstdlib>
#include <iostream>
#include "glm.hpp"


struct VertexUD
{
    glm::vec3 pos;
    glm::vec2 texCoord;

    VertexUD() : pos(glm::vec3(0,0,0)), texCoord(glm::vec2(0,0))
    {

    }

    VertexUD(glm::vec3 position, glm::vec2 coord): pos(position), texCoord(coord)
    {

    }
};

#endif // VERTEXTYPE_HPP_INCLUDED
