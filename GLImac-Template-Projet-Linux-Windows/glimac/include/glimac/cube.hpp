#ifndef CUBE_HPP_INCLUDED
#define CUBE_HPP_INCLUDED

class RigidBody;

#include <cstdlib>
#include <iostream>
#include "rigidBody.hpp"
#include <glimac/particuleManager.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/shared.hpp>



class Cube{


private :
float life;
TYPE_CUBE type;
RigidBody *body;
int indiceForParticleEmitter;
glm::vec3 position; //position du cube, relativement à sa chunk
int indiceTexture;
int indiceInVbo;


public :

Cube();
Cube(glm::vec3 _position, int _indiceInVbo = 0);
Cube(glm::vec3 _position, int _indiceInVbo, ParticuleManager& particuleManager, const glimac::FilePath& path, std::string texturePath, TYPE_CUBE _type);
~Cube();
void setTextureIndice(int indice);
int getTextureIndice();
RigidBody* getRigidBody() ;
void sendDammage(float dammageCount);
glm::vec3 getPosition() const;
float getLife()const;
void setLife(float newLife);
void setIndiceInVbo(int newIndice);
int getIndiceInVbo()const;

void setType(TYPE_CUBE newType);
TYPE_CUBE getType() const;
void setIndiceForParticleEmitter(int newIndice);
int getIndiceForParticleEmitter() const;
void setEmitter(ParticuleManager& particuleManager, const glimac::FilePath& path, std::string texturePath, TYPE_CUBE _type);

Cube* updateTarget(glm::vec3 position,  bool isHover);


};


#endif // CUBE_HPP_INCLUDED
