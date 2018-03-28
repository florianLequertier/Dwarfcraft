#include <cstdlib>
#include <glimac/FilePath.hpp>
#include <glimac/particuleManager.hpp>
#include <glimac/particuleEmitter.hpp>
#include "glimac/cube.hpp"


Cube::Cube() : life(100.f), type(TYPE_CUBE::TERRE01), indiceInVbo(0), indiceForParticleEmitter(-1)  //, body(false, true)
 {
    body = new RigidBody(false, true);
 }

 Cube::Cube(glm::vec3 _position, int _indiceInVbo) : life(100.f), type(TYPE_CUBE::TERRE01), position(_position), indiceInVbo(_indiceInVbo), indiceForParticleEmitter(-1)//, body(_position,false, true)
 {
     body = new RigidBody(_position, false, true);
 }

 //créé automatiquement un cube possédant un systeme de particules  :
 Cube::Cube(glm::vec3 _position, int _indiceInVbo, ParticuleManager& particuleManager, const glimac::FilePath& path, std::string texturePath, TYPE_CUBE _type) : life(100.f), type(_type), position(_position), indiceInVbo(_indiceInVbo), indiceForParticleEmitter(-1)//, body(_position,false, true)
{
    if(type == BASE_FEU01)
    {
        ParticuleEmitter* newEmitter = new ParticuleEmitter(path, texturePath );
        newEmitter->setPosition(_position);
        int emitterIndice = particuleManager.addEmitter(newEmitter);
        indiceForParticleEmitter = emitterIndice;
    }

}

void Cube::setEmitter(ParticuleManager& particuleManager, const glimac::FilePath& path, std::string texturePath, TYPE_CUBE _type)
{
     if(_type == BASE_FEU01)
     {
         ParticuleEmitter* newEmitter = new ParticuleEmitter(path, texturePath, ParticuleEmitter::TYPE_EMISSION::FIRE );
         newEmitter->setPosition(position);
         int emitterIndice = particuleManager.addEmitter(newEmitter);
         indiceForParticleEmitter = emitterIndice;
     }
}

 Cube::~Cube()
 {
    std::cout<<"destruction chunk"<<std::endl;
    delete body;
 }

 RigidBody* Cube::getRigidBody()
 {
    return body;
 }

 void Cube::setTextureIndice(int indice)
{
    indiceTexture = indice;
    type = (TYPE_CUBE)indice;
}

 int Cube::getTextureIndice()
{
    return indiceTexture;
}


glm::vec3 Cube::getPosition() const
{
    return position;
}

float Cube::getLife()const
{
    return life;
}
void Cube::setLife(float newLife)
{
    life = newLife;
}

void Cube::setIndiceInVbo(int newIndice)
{
    indiceInVbo = newIndice;
}

int Cube::getIndiceInVbo()const
{
    return indiceInVbo;
}

void Cube::setType(TYPE_CUBE newType)
{
    type = newType;
    indiceTexture = newType;
}

TYPE_CUBE Cube::getType() const
{
    return type;
}

void Cube::setIndiceForParticleEmitter(int newIndice)
{
    indiceForParticleEmitter = newIndice;
}

int Cube::getIndiceForParticleEmitter() const
{
    return indiceForParticleEmitter;
}

