#ifndef SHARED_HPP
#define SHARED_HPP



//Cube :
enum TYPE_CUBE{TERRE01, PIERRE01, FEUILLE01, BOIS01, BASE_FEU01};

struct Simple2DVertex{
    glm::vec2 position;
    Simple2DVertex(glm::vec2 position2D)
    {
        position = position2D;
    }
};
struct Simple2DTexVertex{
    glm::vec2 position;
    glm::vec2 texCoords;
    Simple2DTexVertex(glm::vec2 position2D, glm::vec2 coordTex)
    {
        position = position2D;
        texCoords = coordTex;
    }
};


struct Basic_2D_Tex_program{

glimac::Program program;

GLuint uniformID_uModelMatrix;
GLuint uniformID_uTexture;

Basic_2D_Tex_program()
{
    uniformID_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    uniformID_uTexture = glGetUniformLocation(program.getGLId(), "uTexture");
}

Basic_2D_Tex_program(const glimac::FilePath& applicationPath): program(loadProgram(applicationPath.dirPath() + "shaders/simpleTex2D.vs.glsl", applicationPath.dirPath() + "shaders/simpleTex2D.fs.glsl"))
{
    uniformID_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    uniformID_uTexture = glGetUniformLocation(program.getGLId(), "uTexture");
}

};

struct Basic_2D_program{

glimac::Program program;

GLuint uniformID_uModelMatrix;
GLuint uniformID_uColor;

Basic_2D_program()
{
    uniformID_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    uniformID_uColor = glGetUniformLocation(program.getGLId(), "uColor");
}

Basic_2D_program(const glimac::FilePath& applicationPath): program(loadProgram(applicationPath.dirPath() + "shaders/simple2D.vs.glsl", applicationPath.dirPath() + "shaders/simple2D.fs.glsl"))
{
    uniformID_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    uniformID_uColor = glGetUniformLocation(program.getGLId(), "uColor");
}

};





struct VAO_container{
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
};

#endif // SHARED_HPP
