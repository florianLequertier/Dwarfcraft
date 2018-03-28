#include <cstdlib>
#include <GL/glew.h>
#include "glimac/glm.hpp"
#include <glimac/Image.hpp>
#include "glimac/particuleManager.hpp"

ParticuleManager::~ParticuleManager()
{
    for(int i=0; i<tabEmitter.size(); i++)
    {
        delete tabEmitter[i];
        tabEmitter[i] = NULL;
    }
    tabEmitter.clear();
}

ParticuleManager::ParticuleManager(const glimac::FilePath& _path, std::string texturePath )
{
    shaderProgram = Particule_program(_path);
    textureID = glimac::LoadImageFromFile(texturePath);
}

ParticuleManager::ParticuleManager(const glimac::FilePath& _path, std::string texturePath, std::vector<ParticuleEmitter*> const& _tabEmitter): ParticuleManager(_path, texturePath)
{
    tabEmitter = _tabEmitter;
}

int ParticuleManager::getNbEmitter()
{
    return tabEmitter.size() - tab_erase.size();
}

int ParticuleManager::addEmitter(ParticuleEmitter* newEmitter)
{
    if(tab_erase.size()==0)
    {
        tabEmitter.push_back(newEmitter);
        return tabEmitter.size()-1;
    }
    else
    {
        int indexTab = tab_erase[tab_erase.size()-1];
        tab_erase.pop_back();
        tabEmitter[indexTab] = newEmitter;
        return indexTab;
    }
}

void ParticuleManager::eraseEmitter(int index)
{

    std::cout<<"coucou erase 01"<<index<<std::endl;
    ParticuleEmitter* tempPtrPart = tabEmitter[index];
    tabEmitter[index] = nullptr;
    delete(tempPtrPart);
    std::cout<<"coucou erase 02"<<std::endl;
    bool findIndex = false;
    std::vector<int>::iterator it;
    for(it = tab_erase.begin(); it !=  tab_erase.end(); it++)
    {
        if(*it == index)
        {
            findIndex = true;
            break;
        }
    }
    if(findIndex) //Si la position dans le vecteur n'est pas deja à null
    {
        tab_erase.push_back(index);
    }
}

void ParticuleManager::draw(glm::mat4 Projection, glm::mat4 View)
{
    //glDisable(GL_DEPTH_TEST);
    shaderProgram.program.use();

    glm::vec3 CameraRight_worldspace = {View[0][0], View[1][0], View[2][0]};
    glm::vec3 CameraUp_worldspace = {View[0][1], View[1][1], View[2][1]};

    glUniform3f(shaderProgram.uniformID_CameraRight_worldspace, CameraRight_worldspace.x, CameraRight_worldspace.y, CameraRight_worldspace.z );
    glUniform3f(shaderProgram.uniformID_CameraUp_worldspace, CameraUp_worldspace.x, CameraUp_worldspace.y, CameraUp_worldspace.z );
    glUniformMatrix4fv(shaderProgram.uniformID_VP, 1, GL_FALSE, glm::value_ptr(Projection*View) );
    glUniformMatrix4fv(shaderProgram.uniform_View, 1, GL_FALSE, glm::value_ptr(View) );


    glUniform1i(shaderProgram.uniform_uTexture, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    for(int i=0; i<tabEmitter.size();i++)
    {
        if(tabEmitter[i]!=nullptr)
        {
            tabEmitter[i]->drawParticules(); //draw dans le container
        }
    }

   glBindTexture(GL_TEXTURE_2D, 0);
   //glEnable(GL_DEPTH_TEST);
}

 void ParticuleManager::update(float delai, glm::vec3 cameraPosition)
 {
     for(int i=0; i<tabEmitter.size();i++)
     {
         if(tabEmitter[i]!=nullptr)
         {
             tabEmitter[i]->UpdateEmitter(delai, cameraPosition);
         }
     }
 }
