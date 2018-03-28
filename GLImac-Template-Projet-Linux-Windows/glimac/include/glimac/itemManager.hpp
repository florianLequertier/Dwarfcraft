#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP
#include <GL/glew.h>
#include "Program.hpp"
#include "FilePath.hpp"
#include "item.hpp"
#include "cube.hpp"
#include "glm.hpp"
#include <cstdlib>
#include <string>

class ItemManager{
    private:
    std::vector<Item*> tabItem;

    public:
    ItemManager();
    ~ItemManager();
    void addItem(Item* item_ptr);
    void deleteItem(int indice);
    void draw(glm::mat4 const& Projection, glm::mat4 const& View, GLuint textureCube, float time);


};


#endif // ITEMMANAGER_HPP
