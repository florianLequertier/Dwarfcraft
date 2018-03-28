#ifndef MESH_HPP
#define MESH_HPP


#include <cstdlib>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "glimac/common.hpp"
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Image.hpp>
#include <glimac/lightManager.hpp>
#include <glimac/light.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct MeshProgram{


    glimac::Program program;

    GLuint uniform_uModelMatrix;
    GLuint uniform_uViewMatrix;
    GLuint uniform_uProjectionMatrix;
    GLuint uniform_uMVP;
    GLuint uniform_uNormalMatrix;

    ///position dans le repere global :
    GLuint uniform_uPosition_glob;

    ///uniform pour la gestion du materiel :
    GLuint uniform_uKd;
    GLuint uniform_uKs;
    GLuint uniform_uShininess;
    ///uniform pour la gestion de la lumiere :
    GLuint uniform_uLightDir_vs;
    GLuint uniform_uLightPos_vs;
    GLuint uniform_uPointLightIntensity;
    GLuint uniform_uDirectionnalLightIntensity;
    GLuint uniform_textureCube;
    GLuint uniformID_uMVMatrix;
    GLuint uniform_uNbPointLight;
    GLuint uniform_uNbDirectionnalLight;

    MeshProgram(const glimac::FilePath& applicationPath = ""): program(loadProgram(applicationPath.dirPath() + "shaders/mesh_light.vs.glsl", applicationPath.dirPath() + "shaders/mesh_light.fs.glsl"))
    {
         uniform_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix" );
         uniform_uViewMatrix = glGetUniformLocation(program.getGLId(), "uViewMatrix" );
         uniform_uProjectionMatrix = glGetUniformLocation(program.getGLId(), "uProjectionMatrix" );
         uniform_uMVP = glGetUniformLocation(program.getGLId(), "uMVP");
         uniform_uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
         uniform_uPosition_glob = glGetUniformLocation(program.getGLId(), "uPosition_glob");

         uniform_uKd = glGetUniformLocation(program.getGLId(), "uKd");
         uniform_uKs = glGetUniformLocation(program.getGLId(), "uKs");
         uniform_uShininess = glGetUniformLocation(program.getGLId(), "uShininess");

         uniform_uLightDir_vs = glGetUniformLocation(program.getGLId(), "uLightDir_vs");
         uniform_uLightPos_vs = glGetUniformLocation(program.getGLId(), "uLightPos_vs");
         uniform_uPointLightIntensity = glGetUniformLocation(program.getGLId(), "uPointLightIntensity");
         uniform_uDirectionnalLightIntensity = glGetUniformLocation(program.getGLId(), "uDirectionnalLightIntensity");
         uniform_uNbPointLight = glGetUniformLocation(program.getGLId(), "uNbPointLight");
         uniform_uNbDirectionnalLight = glGetUniformLocation(program.getGLId(), "uNbDirectionnalLight");


    }

};

struct MeshProgram_unlit{


    glimac::Program program;

    GLuint uniform_uModelMatrix;
    GLuint uniform_uViewMatrix;
    GLuint uniform_uProjectionMatrix;
    GLuint uniform_uMVP;
    GLuint uniform_uNormalMatrix;

    ///position dans le repere global :
    GLuint uniform_uPosition_glob;



    MeshProgram_unlit(const glimac::FilePath& applicationPath = ""): program(loadProgram(applicationPath.dirPath() + "shaders/mesh_shader.vs.glsl", applicationPath.dirPath() + "shaders/mesh_shader.fs.glsl"))
    {
         uniform_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix" );
         uniform_uViewMatrix = glGetUniformLocation(program.getGLId(), "uViewMatrix" );
         uniform_uProjectionMatrix = glGetUniformLocation(program.getGLId(), "uProjectionMatrix" );
         uniform_uMVP = glGetUniformLocation(program.getGLId(), "uMVP");
         uniform_uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
         uniform_uPosition_glob = glGetUniformLocation(program.getGLId(), "uPosition_glob");



    }

};



struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    std::string type;
    aiString path;
};

class Mesh {
    public:
        /*  Mesh Data  */
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
        /*  Functions  */
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, const glimac::FilePath& applicationPath);
        void Draw(glm::mat4 Projection , glm::mat4 View);
        void Draw(glm::mat4 Projection , glm::mat4 View, LightManager &lightManager, glm::vec3 positionView );
        void Draw( glm::mat4 Projection, glm::mat4 View, glm::mat4 ModelMat, LightManager& lightManager, glm::vec3 positionView );
        void Draw(glm::mat4 Projection , glm::mat4 View, glm::mat4 modelMat);
        void Draw( glm::mat4 Projection, glm::mat4 View, glm::mat4 ModelMat, LightManager& lightManager );


    private:
        ///Program :
        MeshProgram shader_program;
        MeshProgram_unlit shader_program_unlit;
        /*  Render data  */
        GLuint VAO, VBO, IBO;
        /*  Functions    */
        void setupMesh();
};

#endif // MESH_HPP
