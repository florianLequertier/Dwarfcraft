#include <cstdlib>
#include <GL/glew.h>
#include <iostream>
#include "glimac/Program.hpp"
#include "glimac/FilePath.hpp"
#include "glimac/Image.hpp"
#include "glimac/glm.hpp"
#include "glimac/skybox.hpp"
#include "glimac/common.hpp"

using namespace glimac;


Skybox::Skybox(const FilePath& applicationPath)
{

    skyProgram = Skybox_program(applicationPath);



    ShapeVertex tab_data_vertexModel[] = {

        //face y>0
        ShapeVertex(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0,1,0), glm::vec2(2/4.f, 0.f) ),
        ShapeVertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0,1,0), glm::vec2(1/4.f, 0.f) ),
        ShapeVertex(glm::vec3(-0.5, 0.5, -0.5),glm::vec3(0,1,0), glm::vec2(1/4.f, 1/3.f) ),
        ShapeVertex(glm::vec3(0.5, 0.5, -0.5),  glm::vec3(0,1,0), glm::vec2(2/4.f, 1/3.f)),

        //face y<0
        ShapeVertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0,-1,0),  glm::vec2(1/4.f, 2/3.f)),
        ShapeVertex(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0,-1,0),  glm::vec2(2/4.f, 2/3.f)),
        ShapeVertex(glm::vec3(0.5, -0.5, 0.5), glm::vec3(0,-1,0), glm::vec2(1/4.f, 1.f)),
        ShapeVertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0,-1,0), glm::vec2(2/4.f, 1.f)),

        //face x>0
        ShapeVertex(glm::vec3(0.5, 0.5, -0.5), glm::vec3(1,0,0),  glm::vec2(2/4.f, 1/3.f) ),
        ShapeVertex(glm::vec3(0.5, 0.5, 0.5), glm::vec3(1,0,0),  glm::vec2(3/4.f, 1/3.f)),
        ShapeVertex(glm::vec3(0.5, -0.5, 0.5), glm::vec3(1,0,0), glm::vec2(3/4.f, 2/3.f)),
        ShapeVertex(glm::vec3(0.5, -0.5, -0.5), glm::vec3(1,0,0), glm::vec2(2/4.f, 2/3.f)),

        //face x<0
        ShapeVertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(-1,0,0),  glm::vec2(1/4.f, 1/3.f) ),
        ShapeVertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-1,0,0), glm::vec2(0, 1/3.f)),
        ShapeVertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(-1,0,0), glm::vec2(0.f, 2/3.f)),
        ShapeVertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1,0,0), glm::vec2(1/4.f, 2/3.f)),


        //face z>0
        ShapeVertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0,0,1), glm::vec2(1.f, 1/3.f)),
        ShapeVertex(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0,0,1),  glm::vec2(3/4.f, 1/3.f)),
        ShapeVertex(glm::vec3(0.5, -0.5, 0.5), glm::vec3(0,0,1), glm::vec2(3/4.f, 2/3.f)),
        ShapeVertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0,0,1), glm::vec2(1.f, 2/3.f)),

        //face z<0
        ShapeVertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0,0,-1), glm::vec2(1/4.f, 1/3.f)),
        ShapeVertex(glm::vec3(0.5, 0.5, -0.5), glm::vec3(0,0,-1),  glm::vec2(2/4.f, 1/3.f)),
        ShapeVertex(glm::vec3(0.5, -0.5, -0.5), glm::vec3(0,0,-1), glm::vec2(2/4.f, 2/3.f) ),
        ShapeVertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0,0,-1), glm::vec2(1/4.f, 2/3.f) ),


    };


     glGenBuffers(1, &vbo_model);
     glBindBuffer(GL_ARRAY_BUFFER, vbo_model);
     glBufferData(GL_ARRAY_BUFFER, 24*sizeof(ShapeVertex)/*sizeof(glm::vec3)*/, tab_data_vertexModel, GL_STATIC_DRAW);
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


      glBindBuffer(GL_ARRAY_BUFFER, vbo_model);
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,position) );
          glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex,normal) );
          glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords) );
      glBindBuffer(GL_ARRAY_BUFFER, 0);





     glBindVertexArray(0);

}
/*

void Skybox::setupCubeMap() {
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}*/

void Skybox::setupCubeMap( const glimac::FilePath& imgName)
{

    std::unique_ptr<Image> texImg = glimac::loadImage(imgName);
    if(texImg == NULL)
    {
       std::cout<<"coucou je suis une erreur de chargement de texture !!! "<<std::cout;
    }


    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImg->getWidth(), texImg->getHeight(), 0, GL_RGBA , GL_FLOAT, texImg->getPixels());

    glBindTexture(GL_TEXTURE_2D, 0);

}


void Skybox::setupCubeMap( )
{
    /*

     std::unique_ptr<Image> texImg[6];
     texImg[0] = loadImage("D:\\prog_opengl\\dwarfcraft\\GLImac-Template-Dwarfcraft\\assets\\textures\\skybox_texture_xpos.png");
     texImg[1] = loadImage("D:\\prog_opengl\\dwarfcraft\\GLImac-Template-Dwarfcraft\\assets\\textures\\skybox_texture_xneg.png");
     texImg[2] = loadImage("D:\\prog_opengl\\dwarfcraft\\GLImac-Template-Dwarfcraft\\assets\\textures\\skybox_texture_ypos.png");
     texImg[3] = loadImage("D:\\prog_opengl\\dwarfcraft\\GLImac-Template-Dwarfcraft\\assets\\textures\\skybox_texture_yneg.png");
     texImg[4] = loadImage("D:\\prog_opengl\\dwarfcraft\\GLImac-Template-Dwarfcraft\\assets\\textures\\skybox_texture_zpos.png");
     texImg[5] = loadImage("D:\\prog_opengl\\dwarfcraft\\GLImac-Template-Dwarfcraft\\assets\\textures\\skybox_texture_zneg.png");

    if(texImg[0] == NULL)
     {
        std::cout<<"coucou je suis une erreur de chargement de texture niosdvniusnv !!! "<<std::cout;
     }

    glGenTextures(6, textureID);
    for(int i=0; i<6; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textureID[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImg[i]->getWidth(), texImg[i]->getHeight(), 0, GL_RGBA , GL_UNSIGNED_BYTE, texImg[i]->getPixels());

         glBindTexture(GL_TEXTURE_2D, 0);
    }

    */

    /*
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //setupCubeMap();        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ptr_texture->getWidth(), ptr_texture->getHeight(), 0, GL_RGBA, GL_FLOAT, ptr_texture->getPixels());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos->getWidth(), xpos->getHeight(), 0, GL_RGBA , GL_UNSIGNED_BYTE, xpos->getPixels());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg->getWidth(), xneg->getHeight(), 0, GL_RGBA , GL_UNSIGNED_BYTE, xneg->getPixels());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos->getWidth(), ypos->getHeight(), 0, GL_RGBA , GL_UNSIGNED_BYTE, ypos->getPixels());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg->getWidth(), yneg->getHeight(), 0, GL_RGBA , GL_UNSIGNED_BYTE, yneg->getPixels());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos->getWidth(), zpos->getHeight(), 0, GL_RGBA , GL_UNSIGNED_BYTE, zpos->getPixels());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg->getWidth(), zneg->getHeight(), 0, GL_RGBA , GL_UNSIGNED_BYTE, zneg->getPixels());

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);*/

}



 Skybox::~Skybox() {
    glDeleteTextures(1, &textureID);
}

void Skybox::draw(glm::mat4 Projection, glm::mat4 View )
{


/*
    GLint OldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
    GLint OldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);*/

    glDepthMask (GL_FALSE);
    skyProgram.program.use();

    glBindVertexArray(vao);

        //glm::mat4 mat_scale = scale(0.25, 0.25, 0.25);
           glm::mat4 model_matrix =  glm::scale(glm::mat4(1.0f),glm::vec3(3,3,3));

            glActiveTexture(GL_TEXTURE0);

            glUniformMatrix4fv(skyProgram.uniformID_uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
            glUniformMatrix4fv(skyProgram.uniformID_uViewMatrix, 1, GL_FALSE, glm::value_ptr(View));
            glUniformMatrix4fv(skyProgram.uniformID_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(Projection));
            glUniform1i(skyProgram.uniformID_cubemap_texture, 0 );



            glBindTexture(GL_TEXTURE_2D, textureID);
             glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            /*
            for(int i=0; i<6; i++)
            {
               glBindTexture(GL_TEXTURE_2D, textureID[i]);
                    glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
               glBindTexture(GL_TEXTURE_2D, 0);
            }*/
            /*
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);*/

        glBindVertexArray(0);
        glDepthMask (GL_TRUE);



//    glCullFace(OldCullFaceMode);
//    glDepthFunc(OldDepthFuncMode);
}
