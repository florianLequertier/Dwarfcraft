#include <cstdlib>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "glimac/common.hpp"
#include "glimac/glm.hpp"
#include "glimac/itemCube.hpp"
#include "glimac/item.hpp"
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/model.hpp>
#include <glimac/itemModel.hpp>



using namespace glimac;


ItemModel::ItemModel(Model& _model, const glimac::FilePath& path, std::string _name, int _quantity, int NBICONES): Item(path, _name, _quantity)
{

    model = Model(path, (GLchar*)_model.getModelPath().c_str());

     ///VAO 2D :

    ShapeVertex* tab_data_vertexModel_2D = new ShapeVertex [24];

         /*ShapeVertex(glm::vec3(-0.5, 0.5, 0.f), glm::vec3(0,1,0), glm::vec2(1/3.f, 2/4.f)),
         ShapeVertex(glm::vec3(0.5, 0.5, 0.f), glm::vec3(0,1,0), glm::vec2(2/3.f, 2/4.f)),
         ShapeVertex(glm::vec3(0.5, 0.5, 0.f), glm::vec3(0,1,0), glm::vec2(2/3.f, 1/4.f)),
         ShapeVertex(glm::vec3(-0.5, 0.5, 0.f), glm::vec3(0,1,0), glm::vec2(1/3.f, 1/4.f))*/

    tab_data_vertexModel_2D[0] =ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0,0,1), glm::vec2(2/3.f, 1/4.f));
   tab_data_vertexModel_2D[1] = ShapeVertex(glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0,0,1), glm::vec2(1/3.f, 1/4.f));
   tab_data_vertexModel_2D[2] = ShapeVertex(glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0,0,1), glm::vec2(1/3.f, 0.f));
   tab_data_vertexModel_2D[3] = ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0,0,1), glm::vec2(2/3.f, 0.f));



     for(int i=0; i< 4; i++)
     {
         tab_data_vertexModel_2D[i].texCoords = glm::vec2(tab_data_vertexModel_2D[i].texCoords.x/NBICONES, tab_data_vertexModel_2D[i].texCoords.y);
     }


      glGenBuffers(1, &vbo_model_2D);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_model_2D);
      glBufferData(GL_ARRAY_BUFFER, 4*sizeof(ShapeVertex), tab_data_vertexModel_2D, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0);



     glGenBuffers(1, &ibo2D);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2D);

     uint32_t indices_2D[] = {
         0, 1, 2, 2, 3, 0
     };


     glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), indices_2D, GL_STATIC_DRAW);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



      glGenVertexArrays(1, &vao2D);
      glBindVertexArray(vao2D);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2D);

      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);


      glBindBuffer(GL_ARRAY_BUFFER, vbo_model_2D);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,position) );
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,normal) );
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords) );
      glBindBuffer(GL_ARRAY_BUFFER, 0);


      glBindVertexArray(0);

}




ItemModel::~ItemModel()
{

}

void ItemModel::draw2D(glm::mat4 const& Projection, glm::mat4 const& View, GLuint texture)
{
    program2D.m_program.use();

        glBindVertexArray(vao2D);

            ///Gestion du scale si les icones sont trop grandes :
            glm::mat4 model_matrix = glm::scale(glm::mat4(1), glm::vec3(0.5f,0.5f,0.5f));
            ///Gestion de la position des items :
            model_matrix = glm::translate(model_matrix, glm::vec3(position2D.x, position2D.y, 0) );

                glUniformMatrix4fv(program2D.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
                glUniform1i(program2D.uniform_uTexture, 0 );
                //glUniform1i(program2D.uniform_uIndiceTexture, decalTexture);


                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

}


void ItemModel::draw3D(glm::mat4 const& Projection, glm::mat4 const& View, GLuint texture, float time)
{

    model.setScale(glm::vec3(0.1,0.1,0.1));
    glm::vec3 positionFinale = position3D;

    if( decalPosition < 1)
    positionFinale += glm::vec3(0, decalPosition+=0.01 ,0);
    else if(decalPosition >=1 && decalPosition <2)
    positionFinale += glm::vec3(0, (2 - (decalPosition+=0.01) ) ,0);
    else
    {
        decalPosition = 0;
       positionFinale += glm::vec3(0, decalPosition+=0.01 ,0);
    }

    model.setRotation(glm::vec3(0,(float)time,0));
    model.setPosition(positionFinale);

    model.Draw(Projection, View);
}
