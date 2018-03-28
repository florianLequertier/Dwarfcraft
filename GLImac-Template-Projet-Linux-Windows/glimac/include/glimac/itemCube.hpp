#ifndef ITEM_CUBE_HPP
#define ITEM_CUBE_HPP

#include <GL/glew.h>
#include "Program.hpp"
#include "FilePath.hpp"
#include "item.hpp"
#include "cube.hpp"
#include "glm.hpp"
#include <cstdlib>
#include <string>


class ItemCube: public Item
{
private:

GLuint vbo_model_3D;
GLuint vbo_model_2D;
GLuint ibo3D;
GLuint ibo2D;

///Arguments nécessaires pour recréer un cube :
float decalTexture;
glm::vec3 position;
int indiceInVbo;


public:
ItemCube(const glimac::FilePath& path, std::string _name = std::string("no name"), int _quantity = 0, int NBPATRONCUBE = 1, int NBICONES = 1, int _decalTexture = 0 );
ItemCube(Cube& cubeModel, const glimac::FilePath& path, std::string _name = std::string("no name"), int _quantity = 0, int NBPATRONCUBE = 1, int NBICONES = 1  );

virtual ~ItemCube();
 virtual void draw2D(glm::mat4 const& Projection, glm::mat4 const& View, GLuint texture);
 virtual void draw3D(glm::mat4 const& Projection, glm::mat4 const& View, GLuint texture, float time);


};

#endif // ITEM_CUBE_HPP
