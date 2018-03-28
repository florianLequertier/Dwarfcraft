#include <cstdlib>
#include <iostream>
#include "glimac/player.hpp"


Player::Player(): Character(),life(100.f)
{

}

 void Player::swingSword(Map& environment, glimac::FilePath& filePath, ItemManager& itemManager, ParticuleManager& particuleManager )
 {
    //if weapon -> attaque with weapon
    //if animations -> play animations

    //gestion lancé de rayon perso / cubes :
    RayInfo cubeTargetInfo;
    cubeTargetInfo = body.testRayTracerWithEnvironment( environment, head.getPosition(),  head.getForawrd(),  0.1f,  50);
    if(cubeTargetInfo.intersection) ///On a intersecté un cube
    {
        std::cout<<"coucou"<<std::endl;
        std::cout<<"position hit :"<< cubeTargetInfo.position <<std::endl;
        glm::vec3 positionRelative = environment.getPositionInActiveChunk(cubeTargetInfo.position);
        std::cout<<"position hit :"<< positionRelative <<std::endl;
        environment.getActiveChunk(cubeTargetInfo.position).sendDammage(50, positionRelative.x, positionRelative.z, positionRelative.y, filePath, itemManager, particuleManager );
        std::cout<<"coucou"<<std::endl;
    }

 }



 void Player::dropItem(Map& environment, ParticuleManager& particuleManager, const glimac::FilePath& path, std::string texturePath, TYPE_CUBE _type )
 {
    RayInfo cubeTargetInfo;
    if ( body.rayTracerWithEnvironment(cubeTargetInfo, environment, head.getPosition(),  head.getForawrd(),  0.1f,  50) ) ///Intersection avec le cube ?
    {
        ///Position du nouveau cube, collé comme il faut au premier cube
        //float decalage = cubeTargetInfo.position + cubeTargetInfo.normale;
        cubeTargetInfo.position += cubeTargetInfo.normale;

        if(cubeTargetInfo.intersection) ///On a intersecté un cube
        {
            std::cout<<"coucou"<<std::endl;
            std::cout<<"position hit :"<< cubeTargetInfo.position <<std::endl;
            glm::vec3 positionRelative = environment.getPositionInActiveChunk(cubeTargetInfo.position);
            std::cout<<"position hit :"<< positionRelative <<std::endl;
            environment.getActiveChunk(cubeTargetInfo.position).AddCube(positionRelative.x, positionRelative.z, positionRelative.y, particuleManager, path, texturePath, _type);
            std::cout<<"coucou"<<std::endl;
        }


    }

 }

 void Player::dropItem(Map& environment)
 {
    RayInfo cubeTargetInfo;
    if ( body.rayTracerWithEnvironment(cubeTargetInfo, environment, head.getPosition(),  head.getForawrd(),  0.1f,  50) ) ///Intersection avec le cube ?
    {
        ///Position du nouveau cube, collé comme il faut au premier cube
        //float decalage = cubeTargetInfo.position + cubeTargetInfo.normale;
        cubeTargetInfo.position += cubeTargetInfo.normale;

        if(cubeTargetInfo.intersection) ///On a intersecté un cube
        {

            std::cout<<"coucou"<<std::endl;
            std::cout<<"position hit :"<< cubeTargetInfo.position <<std::endl;
            glm::vec3 positionRelative = environment.getPositionInActiveChunk(cubeTargetInfo.position);
            std::cout<<"position hit :"<< positionRelative <<std::endl;
            environment.getActiveChunk(cubeTargetInfo.position).AddCube(positionRelative.x, positionRelative.z, positionRelative.y);
            std::cout<<"coucou"<<std::endl;
        }


    }



    /*
    cubeTargetInfo = body.testRayTracerWithEnvironment( environment, head.getPosition(),  head.getForawrd(),  0.2f,  20);
    float angleX = 0.f;
    float angleY = 0.f;
    float angleZ = 0.f;
    float _angleX = 0.f;
    float _angleY = 0.f;
    float _angleZ = 0.f;
    angleX = glm::dot( head.getForawrd(), glm::vec3(1.f,0.f,0.f) );
    angleY = glm::dot( head.getForawrd(), glm::vec3(0.f,1.f,0.f) );
    angleZ = glm::dot( head.getForawrd(), glm::vec3(0.f,0.f,1.f) );
    _angleX = glm::dot( head.getForawrd(), glm::vec3(-1.f,0.f,0.f) );
    _angleY = glm::dot( head.getForawrd(), glm::vec3(0.f,-1.f,0.f) );
    _angleZ = glm::dot( head.getForawrd(), glm::vec3(0.f,0.f,-1.f) );

    glm::vec3 decalage(0.f,0.f,0.f);

    if(angleX > -glm::pi<float>()/4.f && angleX <= glm::pi<float>()/4.f )
    {
        decalage = glm::vec3(-1.f,0.f,0.f);
    }
    else if(_angleX > -glm::pi<float>()/4.f && _angleX <= glm::pi<float>()/4.f )
    {
        decalage = glm::vec3(1.f,0.f,0.f);
    }

    if(angleZ > -glm::pi<float>()/4.f && angleZ <= glm::pi<float>()/4.f )
    {
        decalage = glm::vec3(0.f,0.f,-1.f);
    }
    else if(_angleZ > -glm::pi<float>()/4.f && _angleZ <= glm::pi<float>()/4.f )
    {
        decalage = glm::vec3(0.f,0.f,1.f);
    }

    if(angleY > -glm::pi<float>()/4.f && angleY <= glm::pi<float>()/4.f )
    {
        decalage = glm::vec3(0.f,1.f,0.f);
    }
    else if(_angleY > -glm::pi<float>()/4.f && _angleY <= glm::pi<float>()/4.f )
    {
        decalage = glm::vec3(0.f,-1.f,0.f);
    }




    cubeTargetInfo.position += decalage;

    if(cubeTargetInfo.intersection) ///On a intersecté un cube
    {
        std::cout<<"coucou"<<std::endl;
        std::cout<<"position hit :"<< cubeTargetInfo.position <<std::endl;
        glm::vec3 positionRelative = environment.getPositionInActiveChunk(cubeTargetInfo.position);
        std::cout<<"position hit :"<< positionRelative <<std::endl;
        environment.getActiveChunk(cubeTargetInfo.position).AddCube(positionRelative.x, positionRelative.z, positionRelative.y);
        std::cout<<"coucou"<<std::endl;
    }*/
 }


 void Player::updateTarget(Map& environment)
 {
        //gestion lancé de rayon perso / cubes :
    RayInfo cubeTargetInfo;
    cubeTargetInfo = body.testRayTracerWithEnvironment( environment, head.getPosition(),  head.getForawrd(),  0.1f,  50);
    if(cubeTargetInfo.intersection) ///On a intersecté un cube
    {
        std::cout<<"coucou"<<std::endl;
        std::cout<<"position hit :"<< cubeTargetInfo.position <<std::endl;
        glm::vec3 positionRelative = environment.getPositionInActiveChunk(cubeTargetInfo.position);
        std::cout<<"position hit :"<< positionRelative <<std::endl;
        environment.updateTarget(cubeTargetInfo.position);
        //environment.getActiveChunk(cubeTargetInfo.position).updateTarget( positionRelative.x, positionRelative.z, positionRelative.y );
        std::cout<<"coucou"<<std::endl;
    }
 }


 float Player::getLife() /**/
 {
     return life;
 }

