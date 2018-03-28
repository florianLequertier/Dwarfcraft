#ifndef SKYBOX_HPP_INCLUDED
#define SKYBOX_HPP_INCLUDED

#include <cstdlib>
#include <GL/glew.h>
#include "glm.hpp"
#include "Program.hpp"
#include "FilePath.hpp"
#include "Image.hpp"

struct Skybox_program{

glimac::Program program;

GLuint uniformID_uModelMatrix;
GLuint uniformID_uViewMatrix;
GLuint uniformID_uProjectionMatrix;
GLuint uniformID_cubemap_texture;


Skybox_program()
{
    uniformID_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    uniformID_uViewMatrix = glGetUniformLocation(program.getGLId(), "uViewMatrix");
    uniformID_uProjectionMatrix = glGetUniformLocation(program.getGLId(), "uProjectionMatrix");
    uniformID_cubemap_texture = glGetUniformLocation(program.getGLId(), "cubemapTexture");
}

Skybox_program(const glimac::FilePath& applicationPath): program(loadProgram(applicationPath.dirPath() + "shaders/skyShader.vs.glsl", applicationPath.dirPath() + "shaders/skyShader.fs.glsl"))
{
    uniformID_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    uniformID_uViewMatrix = glGetUniformLocation(program.getGLId(), "uViewMatrix");
    uniformID_uProjectionMatrix = glGetUniformLocation(program.getGLId(), "uProjectionMatrix");
    uniformID_cubemap_texture = glGetUniformLocation(program.getGLId(), "cubemapTexture");
}

};


class Skybox{
private:
Skybox_program skyProgram;
GLuint textureID;
GLuint vbo_model;
GLuint vao;
GLuint ibo;
GLuint vbo_coordTex;

public:
Skybox(const glimac::FilePath& applicationPath);
~Skybox();
void setupCubeMap();
void setupCubeMap( const glimac::FilePath& imgName);
//void setupCubeMap( /*glimac::Image *xpos, glimac::Image *xneg, glimac::Image *ypos, glimac::Image *yneg, glimac::Image *zpos, glimac::Image *zneg*/);
void draw(glm::mat4 Projection, glm::mat4 View);




};


#endif // SKYBOX_HPP_INCLUDED
