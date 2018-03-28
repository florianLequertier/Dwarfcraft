#include <cstdlib>

#include <iostream>
#include "glimac/glm.hpp"
#include "glimac/character.hpp"
#include "glimac/map.hpp"
#include "glimac/force.hpp"

Character::Character(): body(glm::vec3(0.f,0.f,0.f), false, true), head(), walkSpeed(1.f), runSpeed(1.f), direction(0,0,0), foot_forward(0.f,0.f,1.f), foot_right(-1.f,0.f,0.f), isRunning(false), jumpInit(true), canJump(true), tpPreJump(0), isSwimming(false)
{


}

Character::Character(int time): body(glm::vec3(0.f,0.f,0.f), true, true), head(), walkSpeed(1.f), runSpeed(1.f), direction(0,0,0), foot_forward(0.f,0.f,1.f), foot_right(-1.f,0.f,0.f), isRunning(false), jumpInit(true), canJump(true), tpPreJump(0), isSwimming(false)
{
    if(body.getBoolGravity())
    {
        Force* gravityForce = new Force(Force::TYPE_ATTENUATION::CONSTANTE, time,0.1f, glm::vec3(0.f,-1.f,0.f));
        body.addForce(gravityForce);
    }

}

void Character::setUseGravity(bool newState)
{

    body.setBoolGravity(newState);

}


void Character::setUseGravity(bool newState, int time)
{

    body.setBoolGravity(newState);

    if(newState)
    {
        Force* gravityForce = new Force(Force::TYPE_ATTENUATION::CONSTANTE, time,0.2f, glm::vec3(0.f,-1.f,0.f));
        body.addForce(gravityForce);
    }
}

void Character::prepareMove(CHARACTER_MOVEMENT mouvement)
{
    if(!isSwimming)
    {
        	switch(mouvement)
            {
            case FORWARD :
                direction += foot_forward;
                break;
            case BACKWARD :
                direction -= foot_forward;
                break;
            case RIGHT:
                direction += foot_right;
                break;
            case LEFT:
                direction -= foot_right;
                break;

            }
    }
    else
    {
            switch(mouvement)
            {
            case FORWARD :
                direction += getHead().getForawrd();
                break;
            case BACKWARD :
                direction -= getHead().getForawrd();
                break;
            case RIGHT:
                direction += getHead().getRight();
                break;
            case LEFT:
                direction -= getHead().getRight();
                break;

            }
    }

}

void Character::toogleIsSwimming(bool newState)
{
    isSwimming = newState;
    if(isSwimming == true)
    {
        setUseGravity(false);
    }
    else
    {
        setUseGravity(true);
    }
}



void Character::setSpeed(float _walkSpeed, float _runSpeed)
{
    walkSpeed = _walkSpeed;
    runSpeed = _runSpeed;

}

void Character::updateLookAt(float x, float y)
{
	/*m_alpha -= y*glm::pi<float>() / 180;
	m_beta += x*glm::pi<float>() / 180;

	m_forward = glm::vec3(-cosf(m_alpha)*sinf(m_beta), sinf(m_alpha), cosf(m_alpha)*cosf(m_beta));
	m_right = glm::cross(m_forward, glm::vec3(0.f,1.f,0.f));*/

	head.updateLookAt(x,y);

	glm::vec3 head_forward = head.getForawrd();

	foot_forward = glm::vec3( head_forward.x,0.f, head_forward.z);
	glm::vec3 temp_footRight3D =  glm::cross(glm::vec3(foot_forward.x, 0.f, foot_forward.z), glm::vec3(0.f,1.f,0.f)) ;
	foot_right = glm::vec3( temp_footRight3D.x,0.f, temp_footRight3D.z );
}

Camera& Character::getHead()
{
    return head;
}

//se charge aussi de la rotation de la vue :
void Character::updatePosition()
{
    //updateLookAt(x, y);
    //if pas d'obstacles :
    position += glm::vec3(direction.x*walkSpeed,0,direction.y*walkSpeed);
    head.setPosition(position);
    direction = glm::vec3(0,0,0);
    body.setPosition(position);

}

void Character::updatePosition( Map& world)
{
    //updateLookAt(x, y);
    //if pas d'obstacles :
    ///On stock la résultante des forces appliqué au personnage
    glm::vec3 tempPosition;
    Force resultante = /*body.getForce(0);*/body.forceResulting();
    //std::cout<<"FORCE actuelle : norme "<<resultante.getNorme()<<" direction : "<< resultante.getDirection()<<std::endl;
    tempPosition = position;
    ///Gestion de la vitesse de marche ou de course pour le deplacement
    float vitesse = 0.f;
    if(isRunning)
    {
        vitesse = runSpeed;
    }
    else
    {
        vitesse = walkSpeed;
    }
    /*if(isRunning)
    setPosition( position + glm::vec3(direction.x*runSpeed,0,direction.y*runSpeed) + glm::vec3(resultante.getDirection().x*resultante.getNorme(), resultante.getDirection().y*resultante.getNorme(), resultante.getDirection().z*resultante.getNorme()) );
    else
    setPosition( position + glm::vec3(direction.x*walkSpeed,0,direction.y*walkSpeed) + glm::vec3(resultante.getDirection().x*resultante.getNorme(), resultante.getDirection().y*resultante.getNorme(), resultante.getDirection().z*resultante.getNorme()) );
    */


    //std::cout<<"coucou"<<std::endl;
    if(body.getCollider()!=NULL) ///On detecte quand même que le character possède bien un collider
    {
        glm::vec3 gravity(0.f,0.f,0.f);
        if(body.getBoolGravity())
        {
            gravity = glm::vec3(0.f, body.gravityValue, 0.f);
        }
        //glm::vec3 tempPosition = position + glm::vec3(direction.x*runSpeed,0,direction.y*runSpeed) + glm::vec3(resultante.getDirection().x*resultante.getNorme(), resultante.getDirection().y*resultante.getNorme(), resultante.getDirection().z*resultante.getNorme());
        //glm::vec3 positionBoundingBoxInActualChunk = world.getPositionInActiveChunk(tempPosition);
        //std::cout<<"position de la chunk actuelle : "<<world.getActiveChunkPosition(tempPosition)<<std::endl;
        //std::cout<<"position du personnage dans la chunk : "<<positionBoundingBoxInActualChunk<<std::endl;
        //Cube* actualCube = world.getActiveChunk(tempPosition).getCube(positionBoundingBoxInActualChunk);

        bool collisionDetectee = false;
        /*
        if(actualCube != NULL)
        {
            collisionDetectee = true;
        }*/
        bool praticable[] = {true, true, true}; ///Tableau stockant un booleen en fonction de l'axe de deplacement qui est bloqué par un obstacle

        ///Pour chaque case entourant le personnage :
        for(int k=(int)floor(position.y+0.5f)-1; k<=(int)floor(position.y+0.5f)+1; k++)
        {
            if(k<0 || k>world.getNbCubeByRowByChunk()*world.getNbChunk().y-1)
            continue;

            for(int j=(int)floor(position.z+0.5f)-1; j<=(int)floor(position.z+0.5f)+1; j++)
            {
                if(j<0 || j>world.getNbCubeByRowByChunk()*world.getNbChunk().z-1)
                continue;

                for(int i=(int)floor(position.x+0.5f)-1; i<=(int)floor(position.x+0.5f)+1; i++)
                {
                    if(i<0 || i>world.getNbCubeByRowByChunk()*world.getNbChunk().x-1)
                    continue;
                    //std::cout<<"coucou"<<std::endl;

                    ///Selection d'un cube entourant le joueur :
                    glm::vec3 tempOtherPosition = glm::vec3( i, k, j );
                    glm::vec3 positionBoundingBoxOtherCube = world.getPositionInActiveChunk(tempOtherPosition);
                    //std::cout<<positionBoundingBoxOtherCube<<std::endl;
                    Cube* otherCube = world.getActiveChunk(tempOtherPosition).getCube(positionBoundingBoxOtherCube);


                    if( otherCube != NULL && otherCube->getRigidBody()!=NULL ) ///Si le cube considéré n'est pas vide
                    {
                    //std::cout<<"position de l'autre rigidbody : "<<(otherCube->getRigidBody()->getPosition() )<<std::endl;
                    //std::cout<<"position de notre collider : "<<body.getPosition()<<std::endl;
                    //std::cout<<"position du joueur : "<<position<<std::endl;
                    //std::cout<<"tempPosition : "<<tempPosition<<std::endl;
                        //std::cout<<"otherCube possede bien un rigidBody"<<std::endl;

                        for(int m = 0; m<3; m++)///Pour eviter les bugs, je dois simuler le deplacement celon chaque axe separement
                        {
                            ///Je simule l'avancée du personnage :
                            if(m==0)
                            body.setPosition(position + glm::vec3(direction.x*runSpeed + resultante.getDirection().x*resultante.getNorme()+ gravity.x,0,0)  );
                            else if(m==1)
                            body.setPosition(position + glm::vec3(0,direction.y*runSpeed + resultante.getDirection().y*resultante.getNorme()+ gravity.y,0) );
                            else if(m==2)
                            body.setPosition(position + glm::vec3(0,0, direction.z*runSpeed + resultante.getDirection().z*resultante.getNorme()+ gravity.z) );

                            ///Je teste enssuite, à la nouvelle position du personnage, les collisions
                            if(body.testCollision( otherCube->getRigidBody()->getCollider() )) ///S'il y a collision avec un des cubes entourant le joueur
                            {
                                ///Gestion du saut :
                                if(k==(int)floor(position.y+0.5f)-1)///Si il y a des collisions avec un des cubes qui se trouve sous le personnage
                                {
                                    canJump = true;
                                    jumpInit = true;
                                }

                                ///Suite gestion collision :
                                praticable[m] = false; ///Si une collision celon un axe est detecté, un met false dans le tableau
                                collisionDetectee = true;

                                /*
                                if(i>position.x || i<position.x)
                                {
                                    position = glm::vec3(tempPosition.x, position.y, position.z);
                                }
                                if(j>position.z || j<position.z)
                                {
                                    position = glm::vec3(position.x, position.y, tempPosition.z);
                                }
                                if(k>(int)floor(position.y+0.5f) || k<(int)floor(position.y+0.5f))
                                {
                                   position = glm::vec3(position.x, tempPosition.y, position.z);
                                }*/



                                break;
                            }
                            body.setPosition(position ); ///On réinitialise la position du character comme elle était avant les tests.


                        }

                    }


                }
            }
        }

        ///On met à jour la position du character celon chaque axe, seulement si l'axe n'est pas encombré par un obstacle.
        if(praticable[0])
        position = (position + glm::vec3(direction.x*runSpeed + resultante.getDirection().x*resultante.getNorme()+gravity.x,0,0) );
        if(praticable[1])
        position = (position + glm::vec3(0, direction.y*runSpeed + resultante.getDirection().y*resultante.getNorme()+gravity.y,0) );
        if(praticable[2])
        position = (position + glm::vec3(0,0, direction.z*runSpeed + resultante.getDirection().z*resultante.getNorme()+gravity.z) );



        //if(!collisionDetectee)
        {
            setPosition(position);
            //position += glm::vec3(direction.x*walkSpeed,0,direction.y*walkSpeed);
            //head.setPosition(position);
            //body.setPosition(position);
        }

    }
    else
    {
        setPosition(position);
        //position += glm::vec3(direction.x*walkSpeed,0,direction.y*walkSpeed);
        //head.setPosition(position);
        //body.setPosition(position);
    }


    direction = glm::vec3(0,0,0);
}

void Character::updateForces(int time)
{
    body.updateForce(time);
}

glm::vec3 Character::getPosition()
{
    return position;
}

void Character::setPosition(glm::vec3 newPosition)
{
    head.setPosition(newPosition);
    position = newPosition;
    body.setPosition(position);
}

void Character::setIsRunning(bool newState)
{
    isRunning = newState;
}
bool Character::getIsRunning()const
{
    return isRunning;
}
void Character::jump(int time)
{
    if(jumpInit)
    {
        tpPreJump = time;
    }
    //(canJump && jumpInit)?(jumpInit = false):(jumpInit = true);
    if(canJump && jumpInit)
    {
        jumpInit = false;
    }
    if(time - tpPreJump > 100 )
    {
        canJump = false;
    }
    if(canJump)
    {
        //std::cout<<"JUMP01 !!! "<<std::endl;
        Force* jumpForce = new Force(Force::TYPE_ATTENUATION::LINEAIRE, time,0.2f, glm::vec3(0.f,1.f,0.f));
        body.addForce( jumpForce );
    }

}



