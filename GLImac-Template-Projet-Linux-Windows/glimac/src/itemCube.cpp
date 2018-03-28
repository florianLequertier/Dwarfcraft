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



using namespace glimac;

ItemCube::ItemCube(const glimac::FilePath& path, std::string _name, int _quantity, int NBPATRONCUBE, int NBICONES , int _decalTexture): Item(path, _name, _quantity), decalTexture(_decalTexture/(float)NBPATRONCUBE)
{
    ///VAO 3D :

     ShapeVertex* tab_data_vertexModel = new ShapeVertex [24];




        //face y>0
       tab_data_vertexModel[0] = ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0,1,0), glm::vec2(1/3.f, 2/4.f));
       tab_data_vertexModel[1] =  ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0,1,0), glm::vec2(2/3.f, 2/4.f));
      tab_data_vertexModel[2] =  ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0,1,0), glm::vec2(2/3.f, 1/4.f));
      tab_data_vertexModel[3] =  ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0,1,0), glm::vec2(1/3.f, 1/4.f));

        //face y<0
      tab_data_vertexModel[4] =  ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,-1,0), glm::vec2(1/3.f, 1.f));
     tab_data_vertexModel[5] =   ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0,-1,0), glm::vec2(2/3.f, 1.f));
      tab_data_vertexModel[6] =  ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0,-1,0), glm::vec2(2/3.f, 3/4.f));
      tab_data_vertexModel[7] =  ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0,-1,0), glm::vec2(1/3.f, 3/4.f));

        //face x>0
       tab_data_vertexModel[8] = ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1,0,0), glm::vec2(1/3.f, 1/4.f));
       tab_data_vertexModel[9] = ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1,0,0), glm::vec2(1/3.f, 2/4.f));
      tab_data_vertexModel[10] =  ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1,0,0), glm::vec2(0.f, 2/4.f));
       tab_data_vertexModel[11] = ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1,0,0), glm::vec2(0.f, 1/4.f));

        //face x<0
      tab_data_vertexModel[12] =  ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1,0,0), glm::vec2(2/3.f, 1/4.f));
       tab_data_vertexModel[13] = ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1,0,0), glm::vec2(2/3.f, 2/4.f));
       tab_data_vertexModel[14] = ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1,0,0), glm::vec2(1.f, 2/4.f));
       tab_data_vertexModel[15] = ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1,0,0), glm::vec2(1.f, 1/4.f));


        //face z>0
        tab_data_vertexModel[16] =ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0,0,1), glm::vec2(2/3.f, 1/4.f));
       tab_data_vertexModel[17] = ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0,0,1), glm::vec2(1/3.f, 1/4.f));
       tab_data_vertexModel[18] = ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0,0,1), glm::vec2(1/3.f, 0.f));
       tab_data_vertexModel[19] = ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0,0,1), glm::vec2(2/3.f, 0.f));

        //face z<0
       tab_data_vertexModel[20] = ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(2/3.f, 2/4.f));
       tab_data_vertexModel[21] = ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(1/3.f, 2/4.f));
       tab_data_vertexModel[22] = ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(1/3.f, 3/4.f));
       tab_data_vertexModel[23] = ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(2/3.f, 3/4.f));





    for(int i=0; i< 24; i++)
    {
        tab_data_vertexModel[i].texCoords = glm::vec2(tab_data_vertexModel[i].texCoords.x/NBPATRONCUBE, tab_data_vertexModel[i].texCoords.y);
    }


     glGenBuffers(1, &vbo_model_3D);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_model_3D);
     glBufferData(GL_ARRAY_BUFFER, 24*sizeof(ShapeVertex), tab_data_vertexModel, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);






    glGenBuffers(1, &ibo3D);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo3D);

    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20
    };


    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);







     glGenVertexArrays(1, &vao3D);
     glBindVertexArray(vao3D);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo3D);

     glEnableVertexAttribArray(0);
     glEnableVertexAttribArray(1);
     glEnableVertexAttribArray(2);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_model_3D);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,position) );
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,normal) );
     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords) );
     glBindBuffer(GL_ARRAY_BUFFER, 0);


     glBindVertexArray(0);

     std::cout<<vao3D<<std::endl;



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

ItemCube::ItemCube( Cube &cubeModel, const glimac::FilePath& path, std::string _name , int _quantity , int NBPATRONCUBE , int NBICONES ): Item(path, _name, _quantity), decalTexture(cubeModel.getTextureIndice()/(float)NBPATRONCUBE), position(cubeModel.getPosition()), indiceInVbo(cubeModel.getIndiceInVbo())
{
    ///VAO 3D :
    position3D = cubeModel.getPosition();

     ShapeVertex* tab_data_vertexModel = new ShapeVertex [24];




        //face y>0
       tab_data_vertexModel[0] = ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0,1,0), glm::vec2(1/3.f, 2/4.f));
       tab_data_vertexModel[1] =  ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0,1,0), glm::vec2(2/3.f, 2/4.f));
      tab_data_vertexModel[2] =  ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0,1,0), glm::vec2(2/3.f, 1/4.f));
      tab_data_vertexModel[3] =  ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0,1,0), glm::vec2(1/3.f, 1/4.f));

        //face y<0
      tab_data_vertexModel[4] =  ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,-1,0), glm::vec2(1/3.f, 1.f));
     tab_data_vertexModel[5] =   ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0,-1,0), glm::vec2(2/3.f, 1.f));
      tab_data_vertexModel[6] =  ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0,-1,0), glm::vec2(2/3.f, 3/4.f));
      tab_data_vertexModel[7] =  ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0,-1,0), glm::vec2(1/3.f, 3/4.f));

        //face x>0
       tab_data_vertexModel[8] = ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1,0,0), glm::vec2(1/3.f, 1/4.f));
       tab_data_vertexModel[9] = ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1,0,0), glm::vec2(1/3.f, 2/4.f));
      tab_data_vertexModel[10] =  ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1,0,0), glm::vec2(0.f, 2/4.f));
       tab_data_vertexModel[11] = ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1,0,0), glm::vec2(0.f, 1/4.f));

        //face x<0
      tab_data_vertexModel[12] =  ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1,0,0), glm::vec2(2/3.f, 1/4.f));
       tab_data_vertexModel[13] = ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1,0,0), glm::vec2(2/3.f, 2/4.f));
       tab_data_vertexModel[14] = ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1,0,0), glm::vec2(1.f, 2/4.f));
       tab_data_vertexModel[15] = ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1,0,0), glm::vec2(1.f, 1/4.f));


        //face z>0
        tab_data_vertexModel[16] =ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0,0,1), glm::vec2(2/3.f, 1/4.f));
       tab_data_vertexModel[17] = ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0,0,1), glm::vec2(1/3.f, 1/4.f));
       tab_data_vertexModel[18] = ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0,0,1), glm::vec2(1/3.f, 0.f));
       tab_data_vertexModel[19] = ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0,0,1), glm::vec2(2/3.f, 0.f));

        //face z<0
       tab_data_vertexModel[20] = ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(2/3.f, 2/4.f));
       tab_data_vertexModel[21] = ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(1/3.f, 2/4.f));
       tab_data_vertexModel[22] = ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(1/3.f, 3/4.f));
       tab_data_vertexModel[23] = ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,0,-1), glm::vec2(2/3.f, 3/4.f));





    for(int i=0; i< 24; i++)
    {
        tab_data_vertexModel[i].texCoords = glm::vec2(tab_data_vertexModel[i].texCoords.x/NBPATRONCUBE, tab_data_vertexModel[i].texCoords.y);
    }


     glGenBuffers(1, &vbo_model_3D);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_model_3D);
     glBufferData(GL_ARRAY_BUFFER, 24*sizeof(ShapeVertex), tab_data_vertexModel, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);






    glGenBuffers(1, &ibo3D);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo3D);

    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20
    };


    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);







     glGenVertexArrays(1, &vao3D);
     glBindVertexArray(vao3D);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo3D);

     glEnableVertexAttribArray(0);
     glEnableVertexAttribArray(1);
     glEnableVertexAttribArray(2);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_model_3D);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,position) );
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,normal) );
     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords) );
     glBindBuffer(GL_ARRAY_BUFFER, 0);


     glBindVertexArray(0);

     std::cout<<vao3D<<std::endl;



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




ItemCube::~ItemCube()
{

}

void ItemCube::draw2D(glm::mat4 const& Projection, glm::mat4 const& View, GLuint texture)
{
    program2D.m_program.use();

        glBindVertexArray(vao2D);

            ///Gestion du scale si les icones sont trop grandes :
            glm::mat4 model_matrix = glm::scale(glm::mat4(1), glm::vec3(0.5f,0.5f,0.5f));
            ///Gestion de la position des items :
            model_matrix = glm::translate(model_matrix, glm::vec3(position2D.x, position2D.y, 0) );

                glUniformMatrix4fv(program2D.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
                glUniform1i(program2D.uniform_uTexture, 0 );
                glUniform1i(program2D.uniform_uIndiceTexture, decalTexture);


                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

}


void ItemCube::draw3D(glm::mat4 const& Projection, glm::mat4 const& View, GLuint texture, float time)
{
    program3D.m_program.use();

        glBindVertexArray(vao3D);

             ///Gestion du scale :
             glm::mat4 model_matrix = glm::scale( glm::mat4(1.0f), glm::vec3(1,1,1) );
            ///Gestion de la position :
             model_matrix = glm::translate( model_matrix, position3D );
             model_matrix = glm::scale( model_matrix, glm::vec3(0.2,0.2,0.2) );



            if( decalPosition < 1)
            model_matrix = glm::translate( model_matrix,  glm::vec3(0, decalPosition+=0.01 ,0) );
            else if(decalPosition >=1 && decalPosition <2)
            model_matrix = glm::translate( model_matrix, glm::vec3(0, (2 - (decalPosition+=0.01) ) ,0) );
            else
            {
                decalPosition = 0;
                model_matrix = glm::translate( model_matrix,  glm::vec3(0, decalPosition+=0.01 ,0) );
            }

             model_matrix = glm::rotate( model_matrix, (float)time , glm::vec3(0.f, 1.f, 0.f) );




                glUniformMatrix4fv(program3D.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
                glUniformMatrix4fv(program3D.uniformID_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View));
                glUniformMatrix4fv(program3D.uniformID_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection));
                glUniformMatrix4fv(program3D.uniformID_uMVP, 1, GL_FALSE, glm::value_ptr(Projection*View*model_matrix));
                glUniform1i(program3D.uniform_uTexture, 0 );
                glUniform1f(program3D.uniform_uIndiceTexture, decalTexture);


                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);

                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);

}
