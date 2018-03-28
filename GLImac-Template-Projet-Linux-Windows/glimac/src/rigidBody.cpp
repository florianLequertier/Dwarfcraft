#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include "glimac/rigidBody.hpp"
#include "glimac/cubeCollider.hpp"
#include "glimac/force.hpp"

float RigidBody::gravityValue = -0.1f;

RigidBody::~RigidBody()
{
    for(int i=0; i<tabForce.size(); i++)
    {
        delete tabForce[i];
        tabForce[i] = NULL;
    }
    tabForce.clear();
}

RigidBody::RigidBody(): collider(NULL), useGravity(false), isSolid(false)
{

}

RigidBody::RigidBody(bool _useGravity, bool _isSolid): collider(NULL), useGravity(_useGravity), isSolid(_isSolid)
{

}

RigidBody::RigidBody(glm::vec3 positionCollider, bool _useGravity, bool _isSolid): collider(NULL), useGravity(_useGravity), isSolid(_isSolid)
{
    collider = new CubeCollider(positionCollider);
}


bool RigidBody::testCollision(Collider& other)
{
    //std::cout<<"entree dans rigidBody testCollision"<<std::endl;
    return collider->testCollision(other);
}

bool RigidBody::testCollision(Collider* other)
{
    //std::cout<<"entree dans rigidBody testCollision"<<std::endl;
    return collider->testCollision(*other);
}

Collider* RigidBody::getCollider() const
{
    return collider;
}

void RigidBody::addForce(Force* newForce)
{
    tabForce.push_back(newForce);
}
void RigidBody::removeForce(int index)
{
    //std::cout<<"entree remove Force"<<std::endl;
    tabForce.erase(tabForce.begin()+index);
}
void RigidBody::updateForce(int time)
{
    for(int i=0; i<tabForce.size(); i++)
    {
        tabForce[i]->update(time);
        if( tabForce[i]->getType()!=Force::TYPE_ATTENUATION::CONSTANTE && abs(tabForce[i]->getNorme()) <= 0.001 )
        {
            removeForce(i);
        }
    }
}

Force RigidBody::forceResulting()
{
    //std::cout<<"nb de force : "<<tabForce.size()<<std::endl;
    Force resultat(Force::TYPE_ATTENUATION::CONSTANTE, 1, 0, glm::vec3(0,0,0));

    for(int i=0; i<tabForce.size(); i++)
    {
        resultat+= *(tabForce[i]);//tabForce[i]->getDirection() * tabForce[i]->getNorme();
    }


    return resultat;

}

void RigidBody::setPosition(glm::vec3 newPosition)
{
    collider->setPosition(newPosition);

}

glm::vec3 RigidBody::getPosition()const
{
    return ( collider->getPosition() );
}

Force RigidBody::getForce(int index) const
{
    //std::cout<<"nb forces : "<<tabForce.size()<<std::endl;
    if(index>=0 && index<tabForce.size())
    return *(tabForce[index]);
    else
    {
        Force erreurForce;
        return erreurForce;
    }

}

bool RigidBody::getBoolGravity() const
{
    return useGravity;
}

void RigidBody::setBoolGravity(bool newState)
{
    useGravity = newState;
}

RayInfo RigidBody::testRayTracerWithEnvironment( Map& environment,glm::vec3 positionBegin, glm::vec3 direction, float pas, int nbIterration)
{
    std::cout<<"entree dans testRayTracerWithEnvironment"<<std::endl;
    ///initialisation de infoHit :
    RayInfo infoHit;
    infoHit.position = glm::vec3(0.f,0.f,0.f);
    infoHit.intersection = false;
    ///Initialisation de la position de depart :
    glm::vec3 actualPosition = glm::vec3((float)(positionBegin.x + direction.x*pas), (float)(positionBegin.y + direction.y*pas), (float)(positionBegin.z + direction.z*pas));
    ///Pour chaque "fragment" de la droite :
    for(int i=0; i<nbIterration; i++)
    {
        ///Vérifie que l'on est bien dans la map :
        if(floor(actualPosition.y+0.5f) <0 || floor(actualPosition.y+0.5f) > environment.getNbCubeByRowByChunk()*environment.getNbChunk().y-1 )
            continue;
                if(floor(actualPosition.x+0.5f) <0 || floor(actualPosition.x+0.5f) > environment.getNbCubeByRowByChunk()*environment.getNbChunk().x-1 )
                continue;
                    if(floor(actualPosition.z+0.5f) <0 || floor(actualPosition.z+0.5f) > environment.getNbCubeByRowByChunk()*environment.getNbChunk().z-1 )
                    continue;

                glm::vec3 positionBoundingBoxOtherCube = environment.getPositionInActiveChunk(actualPosition);
                //std::cout<<positionBoundingBoxOtherCube<<std::endl;
                Cube* otherCube = environment.getActiveChunk(actualPosition).getCube(positionBoundingBoxOtherCube);

            if( otherCube != NULL && otherCube->getRigidBody()!=NULL )
            {
                if( otherCube->getRigidBody()->getCollider()->testCollisionWithPoint(actualPosition))
                {
                    std::cout<<"coucou"<<std::endl;
                    infoHit.position =  otherCube->getPosition();
                    infoHit.intersection = true;
                    std::cout<<"sortie de testRayTracerWithEnvironment"<<std::endl;
                    return infoHit;
                }
            }

        actualPosition+=glm::vec3((float)(pas*direction.x), (float)(pas*direction.y), (float)(pas*direction.z));
    }
    std::cout<<"sortie de testRayTracerWithEnvironment"<<std::endl;
    return infoHit;
}

bool RigidBody::rayTracerWithEnvironment(RayInfo& info, Map& environment,glm::vec3 positionBegin, glm::vec3 direction, float pas, int nbIterration)
{
    std::cout<<"entree dans rayTracerWithEnvironment"<<std::endl;
    RayInfo hitInfo = testRayTracerWithEnvironment( environment, positionBegin, direction, pas, nbIterration);
    ///Premiere iterration : collision ? position du collider
    if(!hitInfo.intersection)
    {
        std::cout<<"sortie de rayTracerWithEnvironment"<<std::endl;
        return false;
    }
    ///On regarde le cube qui a ete touché :  //////// !!!!
    glm::vec3 positionBoundingBoxOtherCube = environment.getPositionInActiveChunk(hitInfo.position);
    Cube* otherCube = environment.getActiveChunk(hitInfo.position).getCube(positionBoundingBoxOtherCube);
    std::vector<glm::vec3> tabVerticesOfFaces = otherCube->getRigidBody()->getCollider()->getVerticesFace();
    ///deuxième iterration : si collision, obtention du point de collision et de la normal à la face collidée.
    float limitDistance = pas*nbIterration;
    std::vector<RayInfo> tabInfoHit;
    for(int i=0; i<6; i++)
    {
        RayInfo tempInfo;
        tempInfo.position = hitInfo.position;
        tabInfoHit.push_back(tempInfo);
    }
    glm::vec3 actualPosition = glm::vec3((float)(positionBegin.x + direction.x*pas), (float)(positionBegin.y + direction.y*pas), (float)(positionBegin.z + direction.z*pas));

    for(int i=0; i<tabVerticesOfFaces.size()/4; i++)
    {
        ///remplis normale et positionHit pour les 6 faces. Si colision remplis intersection avec true, sinon remplis intersection avec false
        rayTracerFace(tabInfoHit[i], limitDistance, tabVerticesOfFaces[4*i], tabVerticesOfFaces[4*i+1], tabVerticesOfFaces[4*i+2], tabVerticesOfFaces[4*i+3], actualPosition, actualPosition + direction);
    }
    int choixFace = 0;
    float lastDistance = limitDistance*1000.f; ///infini (enfin presque)^^
    for(int i=0; i<tabInfoHit.size(); i++)
    {
        if(tabInfoHit[i].intersection)
        {
            if( glm::distance(positionBegin, glm::vec3( (float)tabInfoHit[i].positionHit.x, (float)tabInfoHit[i].positionHit.y, (float)tabInfoHit[i].positionHit.z) ) < lastDistance )
            {
                choixFace = i;
                lastDistance = glm::distance(positionBegin, tabInfoHit[i].positionHit);
            }
        }
    }

    info = tabInfoHit[choixFace];
    std::cout<<"sortie de rayTracerWithEnvironment"<<std::endl;
    return true;

}

///points dans le sens horaire !
bool RigidBody::rayTracerFace(RayInfo& info, const float limitDistance, const glm::vec3& fA, const glm::vec3& fB, const glm::vec3& fC, const glm::vec3 fD, const glm::vec3& A, const glm::vec3& B)
{
    std::cout<<"entree dans rayTracerFace"<<std::endl;

    glm::vec3 result; ///Point d'intersection
    bool isIntersecting = isLineIntersectingFace(fA, fB, fC, A, B, result );
    if(!isIntersecting)///pas d'intersection
    {
        info.intersection = false;
        std::cout<<"sortie de rayTracerFace"<<std::endl;
        return false;
    }
    float distanceIntersection = glm::length(result - A);
    if(distanceIntersection > limitDistance )
    {
        info.intersection = false;
        std::cout<<"sortie de rayTracerFace"<<std::endl;
        return false;
    }

    bool isIntoFace = isPointIntoFace(result, fA, fB, fC, fD);
    info.normale = glm::cross(fA-fB, fC-fB); ///!!!!

    if(isIntoFace)
    {
        info.intersection = true;
        info.positionHit = result;
        std::cout<<"sortie de rayTracerFace"<<std::endl;
        return true;
    }
    else
    {
        info.intersection = false;
        std::cout<<"sortie de rayTracerFace"<<std::endl;
        return false;
    }


}

bool RigidBody::isPointIntoFace(const glm::vec3& point, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C, const glm::vec3& D)
{
    bool isIntoTriangle01 = false;
    bool isIntoTriangle02 = false;
    isIntoTriangle01 = isPointIntoTriangle(point, A,B,C);
    isIntoTriangle02 = isPointIntoTriangle(point, A,C,D);
    if(isIntoTriangle01 || isIntoTriangle02)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool RigidBody::isPointIntoTriangle(const glm::vec3& point, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C)
{
    glm::vec3 vecPToA = glm::normalize(A-point);
    glm::vec3 vecPToB = glm::normalize(B-point);
    glm::vec3 vecPToC = glm::normalize(C-point);


    float angleAB = (float)acosf( glm::dot(vecPToA, vecPToB) );
    float angleBC = (float)acosf( glm::dot(vecPToB, vecPToC) );
    float angleCA = (float)acosf( glm::dot(vecPToC, vecPToA) );

    float angleTot = angleAB + angleBC + angleCA;

    if(angleTot >= 6.28f)
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool RigidBody::isLineIntersectingFace(const glm::vec3& fA, const glm::vec3& fB, const glm::vec3& fC, const glm::vec3& A, const glm::vec3& B, glm::vec3& result )
{
    glm::vec3 coeffDir = B-A;
    glm::vec3 normal = glm::cross(fA-fB, fC-fB); ///!!!!
    float distance = -glm::dot(fA, normal);

    float denominateur = glm::dot(coeffDir, normal);
    if(denominateur == 0.f)
    {
        return false;
    }

    float k = (-1.f/denominateur)*(A.x*normal.x + A.y*normal.y + A.z*normal.z + distance );
    if(k<0)///On pointe du mauvais coté
    {
        return false;
    }

    result = A + k*coeffDir;
    return true;

}
