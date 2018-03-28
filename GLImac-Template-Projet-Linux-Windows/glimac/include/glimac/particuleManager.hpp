#ifndef PARTICULE_MANAGER_ONCE
#define PARTICULE_MANAGER_ONCE

#include <GL/glew.h>
#include <vector>
#include "glm.hpp"
#include "particule.hpp"
#include "particuleEmitter.hpp"


struct Particule_program{
    glimac::Program program;
    GLuint uniformID_CameraRight_worldspace;
    GLuint uniformID_CameraUp_worldspace;
    GLuint uniform_View;
    GLuint uniformID_VP;
    GLuint uniform_uTexture;


    Particule_program(const glimac::FilePath& applicationPath=""):  program( loadProgram(applicationPath.dirPath() + "shaders/particleTex.vs.glsl", applicationPath.dirPath() + "shaders/particleTex.fs.glsl"))
    {
        uniformID_CameraRight_worldspace = glGetUniformLocation(program.getGLId(), "CameraRight_worldspace");
        uniformID_CameraUp_worldspace = glGetUniformLocation(program.getGLId(), "CameraUp_worldspace");
        uniformID_VP = glGetUniformLocation(program.getGLId(), "VP");
        uniform_View = glGetUniformLocation(program.getGLId(), "View");
        uniform_uTexture = glGetUniformLocation(program.getGLId(), "uTexture");

    }

};


class ParticuleManager{
protected:
    Particule_program shaderProgram;
    GLuint textureID;
    std::vector<int> tab_erase;
    std::vector<ParticuleEmitter*> tabEmitter;


public:
    ParticuleManager(const glimac::FilePath& _path, std::string texturePath);
    ParticuleManager(const glimac::FilePath& _path, std::string texturePath, std::vector<ParticuleEmitter*> const& _tabEmitter);
    ~ParticuleManager();
    int getNbEmitter();
    int addEmitter(ParticuleEmitter* newEmitter);
    void eraseEmitter(int index);
    void draw(glm::mat4 Projection, glm::mat4 View);
    void update(float delai, glm::vec3 cameraPosition);

};


#endif
