#ifndef SHADOWMAP_HPP
#define SHADOWMAP_HPP

#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>

struct ShadowMapProgram{
 glimac::Program m_program;
 GLuint depthMatrixID;

    ShadowMapProgram(const glimac::FilePath& applicationPath): m_program( loadProgram(applicationPath.dirPath() + "shaders/shadowMap.vs.glsl", applicationPath.dirPath() + "shaders/shadowMap.fs.glsl"))    {
        depthMatrixID = glGetUniformLocation(m_program.getGLId(), "depthMVP");
    }
};

class ShadowMap
{

public:

        ShadowMap(const int WindowWindth, const int WindowHeight);

        ~ShadowMap();

        bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

        void BindForWriting();

        void BindForReading(GLenum TextureUnit);

        glm::mat4 computeShadowMap(GLuint depthMatrixID, glm::vec3 positionLight, glm::vec3 directionLight);

    private:
        GLuint m_fbo;
        GLuint m_shadowMap;



};


#endif // SHADOWMAP_HPP
