#ifndef FORCE_HPP_INCLUDED
#define FORCE_HPP_INCLUDED

#include <cstdlib>
#include "glm.hpp"

class Force{
public: enum TYPE_ATTENUATION{CONSTANTE, LINEAIRE};
private:
TYPE_ATTENUATION attenuation;
int tpPre;
float norme;
glm::vec3 direction;
static float gravity;

public:
Force();
Force(TYPE_ATTENUATION _attenuation, int tpPre, float norme, glm::vec3 direction);
void update(int time);
float getNorme() const;
glm::vec3 getDirection() const;
TYPE_ATTENUATION getType() const;

void operator+=(Force const& other);
Force(Force const& other);


};

Force operator+(Force const& force1, Force const& force2);



#endif // FORCE_HPP_INCLUDED
