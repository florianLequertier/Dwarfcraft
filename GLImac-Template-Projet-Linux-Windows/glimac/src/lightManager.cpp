#include <cstdlib>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "glimac/common.hpp"
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include "glimac/light.hpp"
#include "glimac/pointLight.hpp"
#include "glimac/directionnalLight.hpp"
#include "glimac/lightManager.hpp"

LightManager::LightManager(int _MaxLight):MaxLight(_MaxLight)
{
    PointlightIntensityTab = new float[MaxLight*3];
    PointlightPositionTab = new float[MaxLight*3];
    DirectionnalLightIntensityTab = new float[MaxLight*3];
    DirectionnalLightDirectionTab = new float[MaxLight*3];
}

void LightManager::addDirectionnalLight( DirectionnalLight* newLight)
{
    tabDirectionnal.push_back(newLight);


   /*std::vector<glm::vec3> DirectionnalLightDirection;
    for(int i=0; i<tabDirectionnal.size(); i++)
    DirectionnalLightDirection.push_back( tabDirectionnal[i]->getDirection() );

    std::vector<glm::vec3> DirectionnalLightIntentity;
    for(int i=0; i<tabDirectionnal.size(); i++)
    DirectionnalLightIntentity.push_back( tabDirectionnal[i]->getIntensity() );

    const int directionnalLightNb = tabDirectionnal.size()*3;

    for(int i=0, k=0; i< tabDirectionnal.size(); i++, k+=3)
    {
        DirectionnalLightDirectionTab[k] = DirectionnalLightDirection[i].x;
        DirectionnalLightDirectionTab[k+1] = DirectionnalLightDirection[i].y;
        DirectionnalLightDirectionTab[k+2] = DirectionnalLightDirection[i].z;
    }

    for(int i=0, k=0; i<tabDirectionnal.size(); i++, k+=3)
    {
        DirectionnalLightIntensityTab[k] = DirectionnalLightIntentity[i].x;
        DirectionnalLightIntensityTab[k+1] = DirectionnalLightIntentity[i].y;
        DirectionnalLightIntensityTab[k+2] = DirectionnalLightIntentity[i].z;
    }*/
}

void LightManager::addPointLight(PointLight* newLight)
{
    tabPoint.push_back(newLight);
}


void LightManager::computeLights(glm::mat4& View)
{
    ///Directionnal :
    std::vector<glm::vec3> DirectionnalLightDirection;
    for(int i=0; i<tabDirectionnal.size(); i++)
    DirectionnalLightDirection.push_back( glm::vec3( View * glm::vec4(tabDirectionnal[i]->getDirection(),0)) /*glm::vec3( View * glm::vec4( tabDirectionnal[i]->getDirection(),0) )*/  ); ///DEBUG !!!!

    std::vector<glm::vec3> DirectionnalLightIntentity;
    for(int i=0; i<tabDirectionnal.size(); i++)
    DirectionnalLightIntentity.push_back( tabDirectionnal[i]->getIntensity() );

    const int directionnalLightNb = tabDirectionnal.size()*3;

    for(int i=0, k=0; i< tabDirectionnal.size(); i++, k+=3)
    {
        DirectionnalLightDirectionTab[k] = DirectionnalLightDirection[i].x;
        DirectionnalLightDirectionTab[k+1] = DirectionnalLightDirection[i].y;
        DirectionnalLightDirectionTab[k+2] = DirectionnalLightDirection[i].z;
    }

    for(int i=0, k=0; i<tabDirectionnal.size(); i++, k+=3)
    {
        DirectionnalLightIntensityTab[k] = DirectionnalLightIntentity[i].x;
        DirectionnalLightIntensityTab[k+1] = DirectionnalLightIntentity[i].y;
        DirectionnalLightIntensityTab[k+2] = DirectionnalLightIntentity[i].z;
    }


    ///Point :
    std::vector<glm::vec3> PointlightPosition;
    for(int i=0; i<tabPoint.size(); i++)
    PointlightPosition.push_back( glm::vec3( View * glm::vec4(tabPoint[i]->getPosition(),1)) );
    std::vector<glm::vec3> PointlightIntentity;
    for(int i=0; i<tabPoint.size(); i++)
    PointlightIntentity.push_back( tabPoint[i]->getIntensity() );



    for(int i=0, k=0; i<tabPoint.size(); i++, k+=3)
    {
        PointlightPositionTab[k] = PointlightPosition[i].x;
        PointlightPositionTab[k+1] = PointlightPosition[i].y;
        PointlightPositionTab[k+2] = PointlightPosition[i].z;
    }

    for(int i=0, k=0; i<tabPoint.size(); i++, k+=3)
    {
        PointlightIntensityTab[k] = PointlightIntentity[i].x;
        PointlightIntensityTab[k+1] = PointlightIntentity[i].y;
        PointlightIntensityTab[k+2] = PointlightIntentity[i].z;
    }
    /*
    ///Proprietes du materiau :
    glUniform3f(program.uniform_uKd, 0.2f,0.2f,0.2f);
    glUniform3f(program.uniform_uKs, 0.2f,0.2f,0.2f);
    glUniform1f(program.uniform_uShininess, 0.1f);

    ///Propriete de la lumiere :
    glUniform1i(program.uniform_uNbPointLight,  tabPoint.size());
    glUniform1i(program.uniform_uNbDirectionnalLight, tabDirectionnal.size());

    glUniform3fv(program.uniform_uPointLightIntensity, tabPoint.size(), tabPoint);
    glUniform3fv(program.uniform_uLightPos_vs, tabPoint.size(), tabPoint);

    glUniform3fv(program.uniform_uDirectionnalLightIntensity, tabDirectionnal.size(), tabDirectionnal);
    glUniform3fv(program.uniform_uLightDir_vs, tabDirectionnal.size(), tabDirectionnal );


    glUniform1i(program.uniform_textureCube, 0 ); */

}



void LightManager::computeLights()
{
    ///Directionnal :
    std::vector<glm::vec3> DirectionnalLightDirection;
    for(int i=0; i<tabDirectionnal.size(); i++)
    DirectionnalLightDirection.push_back( tabDirectionnal[i]->getDirection() ); ///DEBUG !!!!

    std::vector<glm::vec3> DirectionnalLightIntentity;
    for(int i=0; i<tabDirectionnal.size(); i++)
    DirectionnalLightIntentity.push_back( tabDirectionnal[i]->getIntensity() );

    const int directionnalLightNb = tabDirectionnal.size()*3;

    for(int i=0, k=0; i< tabDirectionnal.size(); i++, k+=3)
    {
        DirectionnalLightDirectionTab[k] = DirectionnalLightDirection[i].x;
        DirectionnalLightDirectionTab[k+1] = DirectionnalLightDirection[i].y;
        DirectionnalLightDirectionTab[k+2] = DirectionnalLightDirection[i].z;
    }

    for(int i=0, k=0; i<tabDirectionnal.size(); i++, k+=3)
    {
        DirectionnalLightIntensityTab[k] = DirectionnalLightIntentity[i].x;
        DirectionnalLightIntensityTab[k+1] = DirectionnalLightIntentity[i].y;
        DirectionnalLightIntensityTab[k+2] = DirectionnalLightIntentity[i].z;
    }


    ///Point :
    std::vector<glm::vec3> PointlightPosition;
    for(int i=0; i<tabPoint.size(); i++)
    PointlightPosition.push_back( tabPoint[i]->getPosition() );
    std::vector<glm::vec3> PointlightIntentity;
    for(int i=0; i<tabPoint.size(); i++)
    PointlightIntentity.push_back( tabPoint[i]->getIntensity() );



    for(int i=0, k=0; i<tabPoint.size(); i++, k+=3)
    {
        PointlightPositionTab[k] = PointlightPosition[i].x;
        PointlightPositionTab[k+1] = PointlightPosition[i].y;
        PointlightPositionTab[k+2] = PointlightPosition[i].z;
    }

    for(int i=0, k=0; i<tabPoint.size(); i++, k+=3)
    {
        PointlightIntensityTab[k] = PointlightIntentity[i].x;
        PointlightIntensityTab[k+1] = PointlightIntentity[i].y;
        PointlightIntensityTab[k+2] = PointlightIntentity[i].z;
    }


}

int LightManager::getNbPointLight() const
{
    return tabPoint.size();
}

int LightManager::getNbDirectionnalLight() const
{
    return tabDirectionnal.size();
}

float* LightManager::getPointLightIntentityTab_ptr() const
{
    return PointlightIntensityTab;
}
float* LightManager::getPointLightPositionTab_ptr() const
{
    return PointlightPositionTab;
}
float* LightManager::getDirectionnalLightIntentityTab_ptr() const
{
    return DirectionnalLightIntensityTab;
}
float* LightManager::getDirectionnalLightDirectionTab_ptr() const
{
    return DirectionnalLightDirectionTab;
}
