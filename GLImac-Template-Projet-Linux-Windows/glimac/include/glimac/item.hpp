#ifndef ITEM_HPP
#define ITEM_HPP

#include <GL/glew.h>
#include "Program.hpp"
#include "FilePath.hpp"
#include "glm.hpp"
#include <cstdlib>
#include <string>



struct ItemProgram2D{

glimac::Program m_program;
GLuint uniformID_uModelMatrix;
GLuint uniform_uTexture;
GLuint uniform_uIndiceTexture;




ItemProgram2D(const glimac::FilePath& applicationPath): m_program( loadProgram(applicationPath.dirPath() + "shaders/item2D.vs.glsl", applicationPath.dirPath() + "shaders/item2D.fs.glsl"))
    {
        uniformID_uModelMatrix = glGetUniformLocation(m_program.getGLId(), "uModelMatrix");
        ///Sampler de la texture :
        uniform_uTexture = glGetUniformLocation(m_program.getGLId(), "uTexture");
        ///Décalage dans la texture :
        uniform_uIndiceTexture = glGetUniformLocation(m_program.getGLId(), "uIndiceTexture");
    }

};


struct ItemProgram3D{

glimac::Program m_program;
GLuint uniformID_uModelMatrix;
GLuint uniformID_uViewMatrix;
GLuint uniformID_uProjectionMatrix;
GLuint uniformID_uMVMatrix;
GLuint uniformID_uMVP;
GLuint uniformID_uNormalMatrix;
GLuint uniform_uTexture;
GLuint uniform_uIndiceTexture;




ItemProgram3D(const glimac::FilePath& applicationPath):  m_program( loadProgram(applicationPath.dirPath() + "shaders/item3D.vs.glsl", applicationPath.dirPath() + "shaders/item3D.fs.glsl"))
    {
    uniformID_uModelMatrix = glGetUniformLocation(m_program.getGLId(), "uModelMatrix");
    uniformID_uViewMatrix = glGetUniformLocation(m_program.getGLId(), "uViewMatrix");
    uniformID_uMVP = glGetUniformLocation(m_program.getGLId(), "uMVP");
    uniformID_uProjectionMatrix = glGetUniformLocation(m_program.getGLId(), "uProjectionMatrix");
    uniformID_uNormalMatrix = glGetUniformLocation(m_program.getGLId(), "uNormalMatrix");
    uniformID_uMVMatrix = glGetUniformLocation(m_program.getGLId(), "uMVMatrix");

    ///Sampler de la texture :
    uniform_uTexture = glGetUniformLocation(m_program.getGLId(), "uTexture");

    ///Décalage dans la texture :
    uniform_uIndiceTexture = glGetUniformLocation(m_program.getGLId(), "uIndiceTexture");

    }


};


class Item{
protected:
    GLuint vao2D;
    GLuint vao3D;
    //GLuint texId2D;
    //GLuint texId3D;
    std::string name;
    int quantity;
    glm::vec3 position3D;
    glm::vec2 position2D;
    ItemProgram2D program2D;
    ItemProgram3D program3D;
    float decalPosition;
    //Model model;

public:
    Item(const glimac::FilePath& path, std::string _name=std::string("no name"), int _quantity = 1);
    virtual ~Item();
   // virtual catchItem(Inventaire& inventaire) = 0;
    virtual void draw2D(glm::mat4 const& Projection, glm::mat4 const& View, GLuint textureCube) = 0;
    virtual void draw3D(glm::mat4 const& Projection, glm::mat4 const& View, GLuint textureCube, float time) = 0;
     void setName(std::string newName);
     void setQuantity(int newQuantity);
     void set2DPosition(glm::vec2 newPosition);
     void set3DPosition(glm::vec3 newPosition);
     std::string getName() const;
     int getQuantity() const;
     glm::vec3 get3DPosition() const;
     glm::vec2 get2DPosition() const;
     void move2D(glm::vec2 mousePosition);


};

#endif // ITEM_HPP
