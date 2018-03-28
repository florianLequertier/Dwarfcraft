#include <cstdlib>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "glimac/common.hpp"
#include "glimac/glm.hpp"
#include "glimac/itemCube.hpp"
#include "glimac/item.hpp"
#include "glimac/itemManager.hpp"
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>

ItemManager::ItemManager()
{

}

ItemManager::~ItemManager()
{
    for(int i=0; i<tabItem.size(); i++)
    {
        delete tabItem[i];
    }
}

void ItemManager::addItem(Item* item_ptr)
{
    tabItem.push_back(item_ptr);
}

void ItemManager::deleteItem(int indice)
{
    tabItem[indice] = NULL;
}

void ItemManager::draw(glm::mat4 const& Projection, glm::mat4 const& View, GLuint textureCube, float time)
{
    for(int i=0; i<tabItem.size(); i++)
    {
        tabItem[i]->draw3D(Projection, View, textureCube, time);
    }
}

