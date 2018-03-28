#include "glimac/SDLWindowManager.hpp"
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <vector>
#include <cmath>
#include <ctime>
#include <cmath>

#include "glimac/Program.hpp"
#include "glimac/vertexType.hpp"
#include "glimac/chunk.hpp"
#include "glimac/camera.h"
#include "glimac/perlin.h"
#include "glimac/common.hpp"
#include "glimac/itemCube.hpp"
#include "glimac/itemManager.hpp"
#include "glimac/tinyxml.hpp"

using namespace glimac;

int MAX_BIOME = 2;

Chunk::~Chunk()
{
    std::cout<<"destruction chunk"<<std::endl;

    for(int i=0; i<tab_Cube.size(); i++ )
    {
        delete tab_Cube[i];
        tab_Cube[i] = NULL;
    }
    tab_Cube.clear();

    glDeleteBuffers(1, &vbo_hasFocus);
    glDeleteBuffers(1, &vbo_indiceTexture);
    glDeleteBuffers(1, &vbo_model);
    glDeleteBuffers(1, &vbo_position);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ibo);
}


void Chunk::init_addNewCube(Cube* newCube, int indice)
{
    tab_Cube[indice] = newCube;
    //updateChunkVBO();
}

void Chunk::updateChunkVBO()
{

    nbCubeAffichage = 0;
    ///position des cubes :
    glm::vec3 pos_temp[nbCube*nbCube*nbCube];
    ///decalage de texture des cubes :
    float indice_texture[nbCube*nbCube*nbCube];
    ///focus des cubes :
    float focus_cube[nbCube*nbCube*nbCube];

    for(int i=0; i<nbCube; i++)
     {
         for(int j=0; j<nbCube; j++)
         {
              for(int k=0; k<nbCube; k++)
              {
                if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i] != NULL)
                {

                    if(testCubeIsVisible(i,j,k))
                    {
                        pos_temp[ nbCubeAffichage] = glm::vec3(positionOrigineX + i,positionOrigineY + k ,positionOrigineZ + j);
                        indice_texture[ nbCubeAffichage ] = tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getTextureIndice()/(float)(nbTextureCube);
                        if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getType() == TYPE_CUBE::BASE_FEU01)
                        focus_cube[nbCubeAffichage] = -1.f;
                        else
                        focus_cube[nbCubeAffichage] = 0.f;
                        tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(nbCubeAffichage);
                        nbCubeAffichage++;
                    }
                    else
                    {
                        tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(-1);
                    }


                }

              }
        }
    }
    ///Mise à jour des vbos :
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0, nbCubeAffichage * sizeof(glm::vec3), pos_temp );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nbCubeAffichage * sizeof(float), indice_texture );
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
        glBufferData(GL_ARRAY_BUFFER,nbCube*nbCube*nbCube*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nbCubeAffichage*sizeof(float), focus_cube);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


}


//Ancienne fonction de spawn des arbres.
void Chunk::spawnTree(glm::vec3 position ,int textureId_feuille, int textureId_tronc)
{

    int taille = rand()%2 + 8;
    int sommetPosition = position.y;

    for(int k=position.y; k<taille; k++)
    {
        sommetPosition++;
        tab_Cube[ k*nbCube*nbCube + position.z*nbCube + position.x] = new Cube( glm::vec3(position.x + positionOrigineX, k + positionOrigineY,position.z + positionOrigineZ) );
        tab_Cube[ k*nbCube*nbCube + position.z*nbCube + position.x]->setTextureIndice(textureId_tronc);
    }

    for(int k=sommetPosition-1; k>=fmax(sommetPosition-3, 0); k--)
    {
        if(k<0 || k>=nbCube-1)
            continue;
        for(int j=-2 + position.z; j<2  + position.z; j++)
        {
            if(j<0 || j>=nbCube-1)
                continue;
            for(int i=-2 + position.x; i<2 + position.x; i++)
            {
                if(i<0 || i>=nbCube-1)
                    continue;

                if(i!= position.x && j!=  position.z)
                {
                    tab_Cube[ k*nbCube*nbCube + j*nbCube + i] = new Cube( glm::vec3(i + positionOrigineX, k + positionOrigineY, j + positionOrigineZ) );
                    tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->setTextureIndice(0);
                }

            }
        }
    }
}



Chunk::Chunk(int _nbTextureCube, int longueur_chunk, int origineX, int origineY, int origineZ ) : positionOrigineX(origineX), positionOrigineY(origineY), positionOrigineZ(origineZ)
{
    //program.use();
    nbTextureCube = _nbTextureCube;



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
        tab_data_vertexModel[i].texCoords = glm::vec2(tab_data_vertexModel[i].texCoords.x/nbTextureCube, tab_data_vertexModel[i].texCoords.y);
    }


     glGenBuffers(1, &vbo_model);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_model);
     glBufferData(GL_ARRAY_BUFFER, 24*sizeof(ShapeVertex), tab_data_vertexModel, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);



    nbCube = longueur_chunk;
    nbCubeAffichage = 0;


    for(int i=0; i<=nbCube; i++)
     {
         for(int j=0; j<=nbCube; j++)
         {
            for(int k=0; k<=nbCube; k++)
             {
                tab_Cube.push_back(NULL);
             }
         }
    }






        glGenBuffers(1, &vbo_position);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
     glBufferData(GL_ARRAY_BUFFER,nbCube*nbCube*nbCube*sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);


        // GLuint vbo_indiceTexture;
        glGenBuffers(1, &vbo_indiceTexture);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
    glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(float), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &vbo_hasFocus);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
     glBufferData(GL_ARRAY_BUFFER,nbCube*nbCube*nbCube*sizeof(float), NULL, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);


        glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



        glGenVertexArrays(1, &vao);
     glBindVertexArray(vao);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

      glEnableVertexAttribArray(0);
     glEnableVertexAttribArray(1);
     glEnableVertexAttribArray(2);
     glEnableVertexAttribArray(3);
     glEnableVertexAttribArray(4);
     glEnableVertexAttribArray(5);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_model);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,position) );
         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,normal) );
         glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords) );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE,  sizeof(float), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE,  sizeof(float), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindVertexArray(0);




}


















Chunk::Chunk(int _nbTextureCube, int longueur_chunk, int origineX, int origineY, int origineZ, int test ) : positionOrigineX(origineX), positionOrigineY(origineY), positionOrigineZ(origineZ)
{
    //program.use();
    nbTextureCube = 5;//_nbTextureCube;



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
        tab_data_vertexModel[i].texCoords = glm::vec2(tab_data_vertexModel[i].texCoords.x/nbTextureCube, tab_data_vertexModel[i].texCoords.y);
    }


     glGenBuffers(1, &vbo_model);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_model);
     glBufferData(GL_ARRAY_BUFFER, 24*sizeof(ShapeVertex), tab_data_vertexModel, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);



     nbCube = longueur_chunk;
     nbCubeAffichage = 0;


     for(int i=0; i<=nbCube; i++)
      {
          for(int j=0; j<=nbCube; j++)
          {
             for(int k=0; k<=nbCube; k++)
              {
                 tab_Cube.push_back(NULL);
              }
          }
     }



     glGenBuffers(1, &vbo_position);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
     glBufferData(GL_ARRAY_BUFFER,nbCube*nbCube*nbCube*sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);


        // GLuint vbo_indiceTexture;
        glGenBuffers(1, &vbo_indiceTexture);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
    glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(float), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &vbo_hasFocus);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
     glBufferData(GL_ARRAY_BUFFER,nbCube*nbCube*nbCube*sizeof(float), NULL, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);


        glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



        glGenVertexArrays(1, &vao);
     glBindVertexArray(vao);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

      glEnableVertexAttribArray(0);
     glEnableVertexAttribArray(1);
     glEnableVertexAttribArray(2);
     glEnableVertexAttribArray(3);
     glEnableVertexAttribArray(4);
     glEnableVertexAttribArray(5);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_model);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,position) );
         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,normal) );
         glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords) );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE,  sizeof(float), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE,  sizeof(float), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindVertexArray(0);

    tab_Cube.clear();


}













Chunk::Chunk(Cube** tabCubeSurface, const FilePath& applicationPath, int _nbTextureCube, int longueur_chunk, int longueur_map, int nbOctave, int persistance,int pas,  int seed, int mini, int maxi, bool surfacique, std::vector<int> hauteurStrates, int origineX, int origineY, int origineZ ) : positionOrigineX(origineX), positionOrigineY(origineY), positionOrigineZ(origineZ)
{
    //program.use();
    nbTextureCube = _nbTextureCube;



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
        tab_data_vertexModel[i].texCoords = glm::vec2(tab_data_vertexModel[i].texCoords.x/nbTextureCube, tab_data_vertexModel[i].texCoords.y);
    }

     GLuint vbo_model;
     glGenBuffers(1, &vbo_model);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_model);
     glBufferData(GL_ARRAY_BUFFER, 24*sizeof(ShapeVertex), tab_data_vertexModel, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///perlin des biomes
    Perlin bruitPerlin_biome(longueur_map, longueur_map, pas, nbOctave, persistance, seed, 0 , 4 );
    ///perlin de la hight map
    Perlin bruitPerlin(longueur_map, longueur_map, pas, nbOctave, persistance, seed, mini , maxi );

    nbCube = longueur_chunk;


    nbCubeAffichage = 0;


    for(int i=0; i<nbCube; i++)
     {
         for(int j=0; j<nbCube; j++)
         {
            for(int k=0; k<nbCube; k++)
             {
                tab_Cube.push_back(NULL);
             }
         }
    }

    //srand(seed);



     for(int i=0; i<nbCube; i++)
     {
         for(int j=0; j<nbCube; j++)
         {

            int valeur_pos_newCube = (int)bruitPerlin.bruit_coherent2D( (double)(origineX + i),(double)(origineZ + j));
            if(!surfacique)
            valeur_pos_newCube = nbCube - 1;

            if(valeur_pos_newCube>=nbCube)
            {
                valeur_pos_newCube = nbCube - 1;
            }

            for(int k=valeur_pos_newCube; k>=0; k--)
            {
                ///Création du cube :
                tab_Cube[ k*nbCube*nbCube + j*nbCube + i] = new Cube(glm::vec3(i+origineX,k+origineY,j+origineZ) );

                ///Gestion surfaces ! Attention : repere global !:
                if(tabCubeSurface[i+origineX + (j+origineZ)*longueur_map ] == NULL || k+origineY > tabCubeSurface[i+origineX + (j+origineZ)*longueur_map ]->getPosition().y)//Si la position du cube placé est au dessus de la position du precedent cube placé sur les mm coordonnée dans le tableau surfacique
                {
                    tabCubeSurface[i+origineX + (j+origineZ)*longueur_map] = tab_Cube[ k*nbCube*nbCube + j*nbCube + i]; //On remplace le cube : celui traité est maintenant consideré comme surfacique.
                }



                if(valeur_pos_newCube == k && surfacique)
                {
                    /*
                    int typeBiome = (int)bruitPerlin.bruit_coherent2D( (double)(origineX + i),(double)(origineZ + j));
                    typeBiome = typeBiome%MAX_BIOME; ///On s'assure que la valeur du biome ne dépasse pas le nombre de biome max.
                    int test_spawnTree = rand()%100;
                    if(typeBiome == 0 && test_spawnTree<20 )
                    {
                        spawnTree(glm::vec3(i,k+1,j) , 3, 2);
                    }*/

                    tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->setTextureIndice(1);
                }
                else
                tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->setTextureIndice(0);

            }


         }
     }

    ///On creuse les grottes :
     int valeur_hauteur_Cave = 0;

    for(int s=0; s<hauteurStrates.size(); s++)
    {
        Perlin bruitPerlin_caves(longueur_map, longueur_map, 16, 1, 0.99, seed+10*s, mini , maxi );

     for(int i=0; i<nbCube; i++)
     {
         for(int j=0; j<nbCube; j++)
         {
                valeur_hauteur_Cave = (int)bruitPerlin_caves.bruit_coherent2D( (double)(origineX + i),(double)(origineZ + j));

                if(valeur_hauteur_Cave>=nbCube/2)
                {
                    valeur_hauteur_Cave = nbCube/2 - 1;
                }
                else if(valeur_hauteur_Cave < maxi/4)
                {
                    valeur_hauteur_Cave = 0;
                }
                int valeur_pos_newCubeCave = hauteurStrates[s] - origineY;//4 * 20 / nbStrates+1;
                for(int k=valeur_pos_newCubeCave + valeur_hauteur_Cave/2; k>valeur_pos_newCubeCave - valeur_hauteur_Cave/2; k--)
                {
                    if(k<0 || k>nbCube-1)
                    continue;

                     delete( tab_Cube[ k*nbCube*nbCube + j*nbCube + i] );
                     tab_Cube[ k*nbCube*nbCube + j*nbCube + i] = NULL;
                }
            }

        }
     }



    ///position des cubes :
    glm::vec3 pos_temp[nbCube*nbCube*nbCube];
    ///decalage de texture des cubes :
    float indice_texture[nbCube*nbCube*nbCube];
    ///focus des cubes :
    float focus_cube[nbCube*nbCube*nbCube];

     for(int i=0; i<nbCube; i++)
     {
         for(int j=0; j<nbCube; j++)
         {
              for(int k=0; k<nbCube; k++)
              {
                if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i] != NULL)
                {
                    pos_temp[ nbCubeAffichage] = glm::vec3(origineX + i,origineY + k ,origineZ + j);
                    indice_texture[ nbCubeAffichage ] = tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getTextureIndice()/(float)(nbTextureCube);
                    focus_cube[ nbCubeAffichage ] = 0.f;
                    tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(nbCubeAffichage);
                    nbCubeAffichage++;
                }

              }
        }
    }




        glGenBuffers(1, &vbo_position);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
     glBufferData(GL_ARRAY_BUFFER,nbCubeAffichage*sizeof(glm::vec3), pos_temp, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);


        // GLuint vbo_indiceTexture;
        glGenBuffers(1, &vbo_indiceTexture);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
    glBufferData(GL_ARRAY_BUFFER, nbCubeAffichage*sizeof(float), indice_texture, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &vbo_hasFocus);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
     glBufferData(GL_ARRAY_BUFFER,nbCubeAffichage*sizeof(float), focus_cube, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);


        glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



        glGenVertexArrays(1, &vao);
     glBindVertexArray(vao);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

      glEnableVertexAttribArray(0);
     glEnableVertexAttribArray(1);
     glEnableVertexAttribArray(2);
     glEnableVertexAttribArray(3);
     glEnableVertexAttribArray(4);
     glEnableVertexAttribArray(5);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_model);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,position) );
         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,normal) );
         glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords) );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE,  sizeof(float), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE,  sizeof(float), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindVertexArray(0);




}


bool Chunk::testCubeIsVisible(int i, int j, int k)
{

    bool isVisible = false;
    //Si le cube est sur un des côté de la chunk : Impossibilité de savoir s'il est visible ou non, on le considère comme visible.
    if(i+2>nbCube || j+2>nbCube || k+2>nbCube || i-1<0 || j-1<0 || k-1<0)
    {
        isVisible = true;
        return isVisible;
    }
    //Pour les cubes dans la chunk : On regarde si une de leur face est potentiellement visible par le joueur (non occulté par un autre cube), si c'est le cas, le cube est considéré comme visible.
    if(i+2<=nbCube &&  tab_Cube[ k*nbCube*nbCube + j*nbCube + (i+1)] == NULL)
    {
        isVisible = true;
        return isVisible;
    }
    else if(i-1>=0 &&  tab_Cube[ k*nbCube*nbCube + j*nbCube + (i-1)] == NULL)
    {
        isVisible = true;
        return isVisible;
    }
    else if(j+2<=nbCube &&  tab_Cube[ k*nbCube*nbCube + (j+1)*nbCube + i] == NULL)
    {
        isVisible = true;
        return isVisible;
    }
    else if(j-1>=0 &&  tab_Cube[ k*nbCube*nbCube + (j-1)*nbCube + i] == NULL)
    {
        isVisible = true;
        return isVisible;
    }
    else if(k+2<=nbCube &&  tab_Cube[ (k+1)*nbCube*nbCube + j*nbCube + i] == NULL)
    {
        isVisible = true;
        return isVisible;
    }
    else if(k-1>=0 &&  tab_Cube[ (k-1)*nbCube*nbCube + j*nbCube + i] == NULL)
    {
        isVisible = true;
        return isVisible;
    }
    else
    {
        return isVisible;
    }
}



Chunk::Chunk(const FilePath& applicationPath, int _nbTextureCube, int longueur_chunk, int longueur_map, int nbOctave, int persistance,int pas,  int seed, int mini, int maxi, bool surfacique, std::vector<int> hauteurStrates, int origineX, int origineY, int origineZ ) : positionOrigineX(origineX), positionOrigineY(origineY), positionOrigineZ(origineZ)
{

    nbTextureCube = _nbTextureCube;


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
        tab_data_vertexModel[i].texCoords = glm::vec2(tab_data_vertexModel[i].texCoords.x/nbTextureCube, tab_data_vertexModel[i].texCoords.y);
    }

     GLuint vbo_model;
     glGenBuffers(1, &vbo_model);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_model);
     glBufferData(GL_ARRAY_BUFFER, 24*sizeof(ShapeVertex), tab_data_vertexModel, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);

    ///perlin des biomes
    Perlin bruitPerlin_biome(longueur_map, longueur_map, pas, nbOctave, persistance, seed, 0 , 4 );
    ///perlin de la hight map
    Perlin bruitPerlin(longueur_map, longueur_map, pas, nbOctave, persistance, seed, mini , maxi );

    nbCube = longueur_chunk;


    nbCubeAffichage = 0;


    for(int i=0; i<nbCube; i++)
     {
         for(int j=0; j<nbCube; j++)
         {
            for(int k=0; k<nbCube; k++)
             {
                tab_Cube.push_back(NULL);
             }
         }
    }


     for(int i=0; i<nbCube; i++)
     {
         for(int j=0; j<nbCube; j++)
         {

            int valeur_pos_newCube = (int)bruitPerlin.bruit_coherent2D( (double)(origineX + i),(double)(origineZ + j));
            if(!surfacique)
            valeur_pos_newCube = nbCube - 1;

            if(valeur_pos_newCube>=nbCube)
            {
                valeur_pos_newCube = nbCube - 1;
            }

            for(int k=valeur_pos_newCube; k>=0; k--)
            {

                tab_Cube[ k*nbCube*nbCube + j*nbCube + i] = new Cube(glm::vec3(i+origineX,k+origineY,j+origineZ) );

                if(valeur_pos_newCube == k && surfacique)
                {
                    int typeBiome = (int)bruitPerlin.bruit_coherent2D( (double)(origineX + i),(double)(origineZ + j));
                    typeBiome = typeBiome%MAX_BIOME; ///On s'assure que la valeur du biome ne dépasse pas le nombre de biome max.
                    int test_spawnTree = rand()%100;
                    if(typeBiome == 0 && test_spawnTree<20 )
                    {
                        spawnTree(glm::vec3(i,k+1,j) , 3, 2);
                    }

                    tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->setTextureIndice(1);
                }
                else
                tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->setTextureIndice(0);

            }


         }
     }

    ///On creuse les grottes :
     int valeur_hauteur_Cave = 0;

    for(int s=0; s<hauteurStrates.size(); s++)
    {
        Perlin bruitPerlin_caves(longueur_map, longueur_map, 16, 1, 0.99, seed+10*s, mini , maxi );

     for(int i=0; i<nbCube; i++)
     {
         for(int j=0; j<nbCube; j++)
         {
                valeur_hauteur_Cave = (int)bruitPerlin_caves.bruit_coherent2D( (double)(origineX + i),(double)(origineZ + j));

                if(valeur_hauteur_Cave>=nbCube/2)
                {
                    valeur_hauteur_Cave = nbCube/2 - 1;
                }
                else if(valeur_hauteur_Cave < maxi/4)
                {
                    valeur_hauteur_Cave = 0;
                }
                int valeur_pos_newCubeCave = hauteurStrates[s] - origineY;//4 * 20 / nbStrates+1;
                for(int k=valeur_pos_newCubeCave + valeur_hauteur_Cave/2; k>valeur_pos_newCubeCave - valeur_hauteur_Cave/2; k--)
                {
                    if(k<0 || k>nbCube-1)
                    continue;

                     delete( tab_Cube[ k*nbCube*nbCube + j*nbCube + i] );
                     tab_Cube[ k*nbCube*nbCube + j*nbCube + i] = NULL;
                }
            }

        }
     }



    ///position des cubes :
    glm::vec3 pos_temp[nbCube*nbCube*nbCube];
    ///decalage de texture des cubes :
    float indice_texture[nbCube*nbCube*nbCube];
    ///focus des cubes :
    float focus_cube[nbCube*nbCube*nbCube];

      for(int i=0; i<nbCube; i++)
     {
         for(int j=0; j<nbCube; j++)
         {
              for(int k=0; k<nbCube; k++)
              {
                if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i] != NULL)
                {
                    if(testCubeIsVisible(i,j,k))
                    {
                        pos_temp[ nbCubeAffichage] = glm::vec3(origineX + i,origineY + k ,origineZ + j);
                        indice_texture[ nbCubeAffichage ] = tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getTextureIndice()/(float)(nbTextureCube);
                        focus_cube[ nbCubeAffichage ] = 0.f;
                        tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(nbCubeAffichage);
                        nbCubeAffichage++;
                    }
                    else
                    {
                        tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(-1);
                    }
                }
              }
        }
    }






        glGenBuffers(1, &vbo_position);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
     glBufferData(GL_ARRAY_BUFFER,nbCubeAffichage*sizeof(glm::vec3), pos_temp, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);


        // GLuint vbo_indiceTexture;
        glGenBuffers(1, &vbo_indiceTexture);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
    glBufferData(GL_ARRAY_BUFFER, nbCubeAffichage*sizeof(float), indice_texture, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &vbo_hasFocus);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
     glBufferData(GL_ARRAY_BUFFER,nbCubeAffichage*sizeof(float), focus_cube, GL_STATIC_DRAW);
     glBindBuffer(GL_ARRAY_BUFFER, 0);


        glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



        glGenVertexArrays(1, &vao);
     glBindVertexArray(vao);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

      glEnableVertexAttribArray(0);
     glEnableVertexAttribArray(1);
     glEnableVertexAttribArray(2);
     glEnableVertexAttribArray(3);
     glEnableVertexAttribArray(4);
     glEnableVertexAttribArray(5);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_model);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,position) );
         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,normal) );
         glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords) );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE,  sizeof(float), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE,  sizeof(float), (const GLvoid*)0 );
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindVertexArray(0);


}

void Chunk::Draw(glm::mat4 const& Projection, glm::mat4 const& View, GLuint textureCube)
{

        glBindVertexArray(vao);


                glVertexAttribDivisor(0,0);
                glVertexAttribDivisor(1,0);
                glVertexAttribDivisor(2,0);
                glVertexAttribDivisor(3,1);
                glVertexAttribDivisor(4,1);
                glVertexAttribDivisor(5,1);


                glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 , nbCubeAffichage);


        glBindVertexArray(0);

}


Cube* Chunk::getCube(glm::vec3 positionInChunk)const
{
    return ( tab_Cube[ (int)positionInChunk.y*nbCube*nbCube + (int)positionInChunk.z*nbCube + (int)positionInChunk.x] );
}

void Chunk::sendDammage(float dammageCount, int i, int j, int k, FilePath& filePath, ItemManager& itemManager, ParticuleManager& particuleManager)
{


    Cube* ptrCube = tab_Cube[k*nbCube*nbCube + j*nbCube + i];

    if(ptrCube == NULL)
    return;

    ptrCube->setLife( ptrCube->getLife() - dammageCount);

    if(ptrCube->getLife() <= 0)
    {
        ptrCube->setLife(0);
        destroyCube(i,j,k, filePath, itemManager, particuleManager );
    }
}

void Chunk::destroyCube(int i, int j, int k, FilePath& filePath, ItemManager& itemManager, ParticuleManager& particuleManager)
{
    if( tab_Cube[k*nbCube*nbCube + j*nbCube + i] == NULL)
    return;




        //create item :
        ItemCube* newItem = new ItemCube( *tab_Cube[k*nbCube*nbCube + j*nbCube + i], filePath, std::string("coucou") , 1, 5, 5);
        //std::cerr<<"creation de l'item"<<std::endl;
        itemManager.addItem(newItem);
        //std::cerr<<"ajout de l'item"<<std::endl;


        //Destruction du cube :
        if(tab_Cube[k*nbCube*nbCube + j*nbCube + i]->getType() == BASE_FEU01)
        particuleManager.eraseEmitter( tab_Cube[k*nbCube*nbCube + j*nbCube + i]->getIndiceForParticleEmitter() );
        //int indiceDeleteCube = tab_Cube[k*nbCube*nbCube + j*nbCube + i]->getIndiceInVbo();
        updateTarget(glm::vec3(i,j,k), false);
        delete ( tab_Cube[k*nbCube*nbCube + j*nbCube + i] );
        tab_Cube[k*nbCube*nbCube + j*nbCube + i] = NULL;
        nbCubeAffichage--;



    updateChunkVBO();

//        nbCubeAffichage = 0;
//    ///position des cubes :
//    glm::vec3 pos_temp[nbCube*nbCube*nbCube];
//    ///decalage de texture des cubes :
//    float indice_texture[nbCube*nbCube*nbCube];
//    ///hover :
//    float focus_cube[nbCube*nbCube*nbCube] = {0.f};

//    for(int i=0; i<nbCube; i++)
//     {
//         for(int j=0; j<nbCube; j++)
//         {
//              for(int k=0; k<nbCube; k++)
//              {
//                if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i] != NULL)// && tab_Cube[k*nbCube*nbCube + j*nbCube + i]->getType() != BASE_FEU01)
//                {

//                    if(testCubeIsVisible(i,j,k))
//                    {
//                        pos_temp[ nbCubeAffichage] = glm::vec3(positionOrigineX + i,positionOrigineY + k ,positionOrigineZ  + j);
//                        indice_texture[ nbCubeAffichage ] = tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getTextureIndice()/(float)(nbTextureCube);
//                        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(nbCubeAffichage);

//                        if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getType() == TYPE_CUBE::BASE_FEU01)
//                        focus_cube[nbCubeAffichage] = -1.f;
//                        else
//                        focus_cube[nbCubeAffichage] = 0.f;

//                        nbCubeAffichage++;
//                    }
//                    else
//                    {
//                        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(-1);
//                    }


//                }

//              }
//        }
//    }
//    ///Mise à jour des vbos :
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER,0, nbCubeAffichage * sizeof(glm::vec3), pos_temp );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, nbCubeAffichage * sizeof(float), indice_texture );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, nbCubeAffichage * sizeof(float), focus_cube );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);


}


void Chunk::AddCube(int i, int j, int k)
{

    //delete ( tab_Cube[k*nbCube*nbCube + j*nbCube + i] );

    if( tab_Cube[k*nbCube*nbCube + j*nbCube + i] != NULL )
    return;

    //new Cube(glm::vec3(i+origineX,k+origineY,j+origineZ) );
    tab_Cube[k*nbCube*nbCube + j*nbCube + i] = new Cube(glm::vec3(i+positionOrigineX, k+positionOrigineY, j+positionOrigineZ));
    tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setTextureIndice(0);
    nbCubeAffichage++;
    tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(nbCubeAffichage);


updateChunkVBO();


//    nbCubeAffichage = 0;
//    ///position des cubes :
//    glm::vec3 pos_temp[nbCube*nbCube*nbCube];
//    ///decalage de texture des cubes :
//    float indice_texture[nbCube*nbCube*nbCube];
//    ///hover
//    float focus_cube[nbCube*nbCube*nbCube];

//    for(int i=0 ; i<nbCube; i++)
//     {
//         for(int j=0; j<nbCube; j++)
//         {
//              for(int k=0; k<nbCube; k++ )
//              {
//                if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i] != NULL)
//                {

//                    if(testCubeIsVisible(i,j,k))
//                    {
//                        pos_temp[ nbCubeAffichage ] = glm::vec3(positionOrigineX + i,positionOrigineY + k ,positionOrigineZ  + j);
//                        indice_texture[ nbCubeAffichage ] = tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getTextureIndice()/(float)(nbTextureCube);
//                        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(nbCubeAffichage);
//                        if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getType() == TYPE_CUBE::BASE_FEU01)
//                        focus_cube[nbCubeAffichage] = -1.f;
//                        else
//                        focus_cube[nbCubeAffichage] = 0.f;

//                        nbCubeAffichage++;
//                    }
//                    else
//                    {
//                        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(-1);
//                    }
//                }

//              }
//        }
//    }
//    ///Mise à jour des vbos :
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER,0, nbCubeAffichage * sizeof(glm::vec3), pos_temp );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, nbCubeAffichage * sizeof(float), indice_texture );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, nbCubeAffichage * sizeof(float), focus_cube );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

}



void Chunk::AddCube(int i, int j, int k, ParticuleManager& particuleManager, const FilePath& path, std::string texturePath, TYPE_CUBE _type )
{

    //delete ( tab_Cube[k*nbCube*nbCube + j*nbCube + i] );

    if( tab_Cube[k*nbCube*nbCube + j*nbCube + i] != NULL )
    return;

    if(_type != TYPE_CUBE::BASE_FEU01)
    {
        tab_Cube[k*nbCube*nbCube + j*nbCube + i] = new Cube(glm::vec3(i+positionOrigineX, k+positionOrigineY, j+positionOrigineZ));
        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setTextureIndice(0);
        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setType(_type);
        nbCubeAffichage++;
        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(nbCubeAffichage);
    }
    else
    {
        tab_Cube[k*nbCube*nbCube + j*nbCube + i] = new Cube(glm::vec3(i+positionOrigineX, k+positionOrigineY, j+positionOrigineZ));
        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setTextureIndice(5);
        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setType(_type);
        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setEmitter( particuleManager, path, texturePath, _type); //Spawn + attachement de l'emitter au manager
        nbCubeAffichage++;
        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(nbCubeAffichage);
    }


    updateChunkVBO();

//    nbCubeAffichage = 0;
//    ///position des cubes :
//    glm::vec3 pos_temp[nbCube*nbCube*nbCube];
//    ///decalage de texture des cubes :
//    float indice_texture[nbCube*nbCube*nbCube];
//    ///focus
//    float focus_cube[nbCube*nbCube*nbCube];

//    for(int i=0 ; i<nbCube; i++)
//     {
//         for(int j=0; j<nbCube; j++)
//         {
//              for(int k=0; k<nbCube; k++ )
//              {
//                if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i] != NULL)
//                {

//                    if(testCubeIsVisible(i,j,k))
//                    {
//                        pos_temp[nbCubeAffichage] = glm::vec3(positionOrigineX + i,positionOrigineY + k ,positionOrigineZ  + j);
//                        indice_texture[ nbCubeAffichage ] = tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getTextureIndice()/(float)(nbTextureCube);
//                        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(nbCubeAffichage);

//                        if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getType() == TYPE_CUBE::BASE_FEU01)
//                        focus_cube[nbCubeAffichage] = -1.f;
//                        else
//                        focus_cube[nbCubeAffichage] = 0.f;

//                        nbCubeAffichage++;
//                    }
//                    else
//                    {
//                        tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(-1);
//                    }



//                }

//              }
//        }
//    }
//    ///Mise à jour des vbos :
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER,0, nbCubeAffichage * sizeof(glm::vec3), pos_temp );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, nbCubeAffichage * sizeof(float), indice_texture );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, nbCubeAffichage * sizeof(float), focus_cube );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);



}


//void Chunk::AddCube(int i, int j, int k, TYPE_CUBE _type)
//{

//    //delete ( tab_Cube[k*nbCube*nbCube + j*nbCube + i] );

//    if( tab_Cube[k*nbCube*nbCube + j*nbCube + i] != NULL )
//    return;

//    //new Cube(glm::vec3(i+origineX,k+origineY,j+origineZ) );
//    tab_Cube[k*nbCube*nbCube + j*nbCube + i] = new Cube(glm::vec3(i+positionOrigineX, k+positionOrigineY, j+positionOrigineZ));
//    tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setTextureIndice(0);
//    tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setType(_type);
//    nbCubeAffichage++;
//    tab_Cube[k*nbCube*nbCube + j*nbCube + i]->setIndiceInVbo(nbCubeAffichage);




//    nbCubeAffichage = 0;
//    ///position des cubes :
//    glm::vec3 pos_temp[nbCube*nbCube*nbCube];
//    ///decalage de texture des cubes :
//    float indice_texture[nbCube*nbCube*nbCube];

//    for(int i=0 ; i<nbCube; i++)
//     {
//         for(int j=0; j<nbCube; j++)
//         {
//              for(int k=0; k<nbCube; k++ )
//              {
//                if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i] != NULL)
//                {

//                    pos_temp[ nbCubeAffichage ] = glm::vec3(positionOrigineX + i,positionOrigineY + k ,positionOrigineZ  + j);
//                    indice_texture[ nbCubeAffichage ] = tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getTextureIndice()/(float)(nbTextureCube);
//                    nbCubeAffichage++;


//                }

//              }
//        }
//    }
//    ///Mise à jour des vbos :
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER,0, nbCubeAffichage * sizeof(glm::vec3), pos_temp );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glBindBuffer(GL_ARRAY_BUFFER, vbo_indiceTexture);
//        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, nbCubeAffichage * sizeof(float), indice_texture );
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

//}



Cube* Chunk::updateTarget(glm::vec3 position,  bool isHover)
{

    //delete ( tab_Cube[k*nbCube*nbCube + j*nbCube + i] );

    if( tab_Cube[position.y*nbCube*nbCube + position.z*nbCube + position.x] == NULL )
    return NULL;
    else if(tab_Cube[position.y*nbCube*nbCube + position.z*nbCube + position.x]->getIndiceInVbo() < 0)
    return NULL;

    else
    {
        Cube* cubeActuel = tab_Cube[position.y*nbCube*nbCube + position.z*nbCube + position.x];
        float newState = 0.f;
        if(isHover)
            newState = 1.f;
        else
        {
            if(cubeActuel->getType() == TYPE_CUBE::BASE_FEU01)
            newState = -1.f;
            else
            newState = 0.f;
        }


        ///Mise à jour des vbos :
        glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
            //glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(float), NULL, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, cubeActuel->getIndiceInVbo()*sizeof(float) , sizeof(float), &newState );
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return cubeActuel;
    }

}


void Chunk::updateTarget( int i, int j, int k)
{

    //delete ( tab_Cube[k*nbCube*nbCube + j*nbCube + i] );

    if( tab_Cube[k*nbCube*nbCube + j*nbCube + i] == NULL )
    return;


    int positionTarget = k*nbCube*nbCube + j*nbCube + i;
    ///focus des cubes :
    float focus_cube[nbCube*nbCube*nbCube];
    nbCubeAffichage = 0;

    for(int i=0; i<nbCube; i++)
     {
         for(int j=0; j<nbCube; j++)
         {
              for(int k=0; k<nbCube; k++)
              {
                if(tab_Cube[ k*nbCube*nbCube + j*nbCube + i] != NULL) // && tab_Cube[ k*nbCube*nbCube + j*nbCube + i]->getType() != BASE_FEU01) /// DEBUG transparence
                {
                    if(positionTarget != k*nbCube*nbCube + j*nbCube + i)
                    {
                            focus_cube[ nbCubeAffichage] = 0.f;
                    }
                    else
                    {
                            focus_cube[ nbCubeAffichage] = 1.f;
                    }
                    nbCubeAffichage++;

                }

              }
        }
    }
    ///Mise à jour des vbos :
    glBindBuffer(GL_ARRAY_BUFFER, vbo_hasFocus);
        glBufferData(GL_ARRAY_BUFFER, nbCube*nbCube*nbCube*sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0, nbCubeAffichage * sizeof(float), focus_cube );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

glm::vec3 Chunk::getPosition() const
{
    glm::vec3 retour;
    retour.x = (float)positionOrigineX;
    retour.y = (float)positionOrigineY;
    retour.z = (float)positionOrigineZ;

    return retour;

}



std::vector<Cube*> Chunk::getAllCube()const
{
    return tab_Cube;
}

void Chunk::setTabCube(std::vector<Cube*> tabCube){
    tab_Cube=tabCube;
}


int Chunk::getNbCube() const /***/
{
    return nbCube;
}
int Chunk::getNbCubeAff() const /***/
{
    return nbCubeAffichage;
}
int Chunk::getNbTextureCube() const /***/
{
    return nbTextureCube;
}

void Chunk::setNbCube(int NbCube)/**/
{
    nbCube=NbCube;
}

void Chunk::setNbCubeAff(int NbCubeAff)/**/
{
    nbCubeAffichage=NbCubeAff;
}

void Chunk::setNbTextureCube(int NbTextCube)/**/
{
    nbTextureCube=NbTextCube;
}
