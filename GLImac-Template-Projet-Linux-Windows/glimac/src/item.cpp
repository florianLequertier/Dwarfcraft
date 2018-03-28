#include <cstdlib>
#include <GL/glew.h>
#include <vector>
#include "glimac/glm.hpp"
#include "glimac/item.hpp"


Item::Item(const glimac::FilePath& path, std::string _name, int _quantity): name(_name), quantity(_quantity), position3D(glm::vec3(0,0,0)), position2D(0,0), program2D(path), program3D(path), decalPosition(0)
{


}
 Item::~Item()
{

}


 void Item::setName(std::string newName)
{

    name = newName;
}

 void Item::setQuantity(int newQuantity)
{

    quantity = newQuantity;
}

void Item::set2DPosition(glm::vec2 newPosition)
{
    position2D = newPosition;
}

void Item::set3DPosition(glm::vec3 newPosition)
{
    position3D = newPosition;
}


std::string Item::getName() const
{

    return name;
}
 int Item::getQuantity() const
{

    return quantity;
}
 glm::vec3 Item::get3DPosition() const
{
    return position3D;
}
 glm::vec2 Item::get2DPosition() const
{
    return position2D;
}
 void Item::move2D(glm::vec2 mousePosition)
{

}
