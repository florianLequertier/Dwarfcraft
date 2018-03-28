#ifndef ITEM_MODEL_HPP
#define ITEM_MODEL_HPP

#include <GL/glew.h>
#include "Program.hpp"
#include "FilePath.hpp"
#include "item.hpp"
#include "cube.hpp"
#include "glm.hpp"
#include <cstdlib>
#include <string>
#include "model.hpp"


class ItemModel: public Item
{
private:

Model model;
GLuint vbo_model_2D;
GLuint ibo2D;


///Arguments nécessaires pour recréer un model :
std::string modelPathGL;
glm::vec3 position;


public:
ItemModel(Model& _model, const glimac::FilePath& path, std::string _name = std::string("no name"), int _quantity = 0, int NBICONES=1);

virtual ~ItemModel();
 virtual void draw2D(glm::mat4 const& Projection, glm::mat4 const& View, GLuint texture);
 virtual void draw3D(glm::mat4 const& Projection, glm::mat4 const& View, GLuint texture, float time);


};

#endif // ITEM_MODEL_HPP
