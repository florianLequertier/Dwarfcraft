#include <cstdlib>
#include <iostream>
#include <cmath>

#include "glimac/cubeCollider.hpp"

CubeCollider::CubeCollider(): Collider(), length(1), width(1), height(1), isVisible(false)
{
    type = Collider::TypeCollider::CUBE;
}
CubeCollider::CubeCollider(glm::vec3 _position): Collider(_position), length(1), width(1), height(1), isVisible(false)
{
    type = Collider::TypeCollider::CUBE;
}
void CubeCollider::toogleVisibility(bool newVisibility)
{

}

bool CubeCollider::testCollision(Collider& other)
{
    if(other.getType() == Collider::TypeCollider::CUBE)
    {
        //std::cout<<"entree dans test collision du cube collider"<<std::endl;
       return testCollisionCube(dynamic_cast<CubeCollider&>(other) );
    }
    //std::cout<<"sortie du test collision du cube collider"<<std::endl;
    /*
    else if(other.type == TypeCollision.SPHERE)
    {
        return CubeCollider::testCollisionSphere(SphereCollider* other);
    }*/


}

bool CubeCollider::testCollisionWithPoint(glm::vec3 point)
{
    if(position.x+length/2.f > point.x &&
        position.x-length/2.f < point.x &&
        position.y+width/2.f > point.y &&
        position.y-width/2.f < point.y &&
        position.z+height/2.f > point.z &&
        position.z-height/2.f < point.z)
        {
            return true;
        }
        else
        {
            return false;
        }
}


bool CubeCollider::testCollision(Collider* other)
{
    if(other->getType() == Collider::TypeCollider::CUBE)
    {
        //std::cout<<"entree dans test collision du cube collider"<<std::endl;
       return testCollisionCube(dynamic_cast<CubeCollider&>(*other) );
    }
    //std::cout<<"sortie du test collision du cube collider"<<std::endl;
    /*
    else if(other.type == TypeCollision.SPHERE)
    {
        return CubeCollider::testCollisionSphere(SphereCollider* other);
    }*/


}
bool CubeCollider::testCollisionCube(CubeCollider& other)
{
    if( position.x+length/2.f <= other.position.x-other.length/2.f ||
        position.x-length/2.f >= other.position.x+other.length/2.f ||
        position.y+width/2.f <= other.position.y-other.width/2.f ||
        position.y-width/2.f >= other.position.y+other.width/2.f ||
        position.z+height/2.f <= other.position.z-other.height/2.f ||
        position.z-height/2.f >= other.position.z+other.height/2.f

        )
        {
            //std::cout<<"pas de collisions"<<std::endl;
            return false;
        }
        else
        {
            //std::cout<<"!!!! collision detectee !!!!"<<std::endl;
            return true;
        }

}

std::vector<glm::vec3> CubeCollider::getVerticesFace()
{
    std::cout<<"entree dans getVerticesFace"<<std::endl;
    std::vector<glm::vec3> vertices;
    glm::vec3 tempVertex;
    ///face01 x positif
    tempVertex = glm::vec3(position.x+length/2.f, position.y-width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y-width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y+width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y+width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    ///face02 x négatif
    tempVertex = glm::vec3(position.x-length/2.f, position.y-width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x-length/2.f, position.y+width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x-length/2.f, position.y+width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x-length/2.f, position.y-width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    ///face03 y positif
    tempVertex = glm::vec3(position.x-length/2.f, position.y+width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y+width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y+width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x-length/2.f, position.y+width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    ///face04 y négatifs
    tempVertex = glm::vec3(position.x-length/2.f, position.y-width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x-length/2.f, position.y-width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y-width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y-width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    ///face05 Z positif
    tempVertex = glm::vec3(position.x-length/2.f, position.y-width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x-length/2.f, position.y+width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y+width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y-width/2.f , position.z+height/2.f );
    vertices.push_back(tempVertex);
    ///face06 Z négatifs
    tempVertex = glm::vec3(position.x-length/2.f, position.y-width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y-width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x+length/2.f, position.y+width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);
    tempVertex = glm::vec3(position.x-length/2.f, position.y+width/2.f , position.z-height/2.f );
    vertices.push_back(tempVertex);


    std::cout<<"sortie de getVerticesFace"<<std::endl;
    return vertices;

}
/*
virtual bool CubeCollider::testCollisionSphere(SphereCollider* other)
{
    if( math.sqrt( position.x*position.x + position.y*position.y + position.z*position.z) < other.radius )
    {
        return true;
    }
    else
    {
        return false;
    }

}*/
