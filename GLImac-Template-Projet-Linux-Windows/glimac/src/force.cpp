#include <cstdlib>
#include <iostream>
#include "glimac/force.hpp"

static float gravity = 0.1f;

Force::Force(): attenuation(Force::TYPE_ATTENUATION::CONSTANTE), tpPre(0), norme(0), direction(glm::vec3(0.f,0.f,0.f))
{

}

Force::Force( Force::TYPE_ATTENUATION _attenuation, int _tpPre, float _norme, glm::vec3 _direction): attenuation(_attenuation), tpPre(_tpPre), norme(_norme)
{
    if(_direction == glm::vec3(0,0,0))
    {
        direction = glm::vec3(0,0,0);
    }
    else
    {
        direction = glm::normalize(_direction);
    }
}

void Force::update(int time)
{
    int deltaTime = (time - tpPre)/1000;
    if(attenuation == Force::TYPE_ATTENUATION::LINEAIRE)
    {
        norme = norme/(deltaTime+1);
    }
}

float Force::getNorme() const
{
    return norme;
}
glm::vec3 Force::getDirection() const
{
    return direction;
}
Force::TYPE_ATTENUATION Force::getType() const
{
    return attenuation;
}


void Force::operator+=(Force const& other)
{

    glm::vec3 temp_direction = direction*norme + other.direction*other.norme;
    //std::cout<<direction<<std::endl;
    //std::cout<<other.direction<<std::endl;
    norme = glm::length(temp_direction);
    //std::cout<<temp_direction<<std::endl;
    if(temp_direction == glm::vec3(0,0,0))
    {
        direction = glm::vec3(0,0,0);
    }
    else
    {
        direction = glm::normalize(temp_direction);
    }

}

Force::Force(Force const& other)
{
    norme = other.norme;
    direction = other.direction;
    tpPre = other.tpPre;
    attenuation = other.attenuation;
}

Force operator+(Force const& force1, Force const& force2)
{
    Force resultat(force1);
    resultat+=force2;
    return resultat;
}

