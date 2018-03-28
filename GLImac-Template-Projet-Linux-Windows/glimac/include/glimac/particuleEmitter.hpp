#ifndef PARTICULEEMITTER_HPP
#define PARTICULEEMITTER_HPP

#include <GL/glew.h>
#include <glimac/particule.hpp>
#include <glimac/transformable.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>

#define MAXPARTICULE 100





class ParticuleEmitter
{
public:    enum class TYPE_EMISSION {FONTAINE, FIRE};

protected:
    //Particule_program shaderProgram;
    GLuint textureID;

    int m_newParticules;
    Particle **m_particlesContainer;
    int m_lastUsedParticle;
    int m_particuleCount;

    GLubyte* m_particuleData_color;
    GLfloat* m_particuleData_positionSize;
    GLfloat m_g_vertex_buffer_data[20];

    GLuint m_vertexBuffer_colorParticule;
    GLuint m_vertexBuffer_positionParticule;
    GLuint m_vertexBuffer_modelParticule;
    GLuint vao;

    glimac::FilePath path;
    TYPE_EMISSION typeEmitter;

    glm::vec3 position;
    float nbNewParticulePre;

private:
    int FindUnusedParticle();
    void setNewParticules(float deltaTime, int nombreNouvellesParticules = 10000);
    int getNewParticules() const;
    virtual void simulateParticules(float delta, glm::vec3 cameraPosition);
    void simulateParticules_fire(float delta, glm::vec3 cameraPosition);
    void SortParticles();
    int getParticuleCount() const;
    void initialyseVBOs();
    void initialyseVAO();
    void InitManager();
    void createNewParticules(float delai, int nombreNouvellesParticules = 10000);
    void createNewParticules_fire(float delai, int nombreNouvellesParticules = 5000);
    void updateVBOs();

public:

    ParticuleEmitter( const glimac::FilePath& _path, TYPE_EMISSION _type = TYPE_EMISSION::FIRE);
    ParticuleEmitter( const glimac::FilePath& _path, std::string texturePath, TYPE_EMISSION _type = TYPE_EMISSION::FIRE);
    ~ParticuleEmitter();
    virtual void UpdateEmitter(float delai, glm::vec3 cameraPosition);
    virtual void UpdateEmitter(float delai, glm::vec3 cameraPosition, int nombreNouvellesParticules, TYPE_EMISSION type );
    void drawParticules(glm::mat4 Projection, glm::mat4 View);
    void drawParticules(); //draw dans le container
    void setPosition(glm::vec3 newPosition);
    glm::vec3 getPosition() const;
    glimac::FilePath getPath()const;

    ParticuleEmitter(const ParticuleEmitter& other):ParticuleEmitter(path)
    {
        position = other.getPosition();
    }


};

#endif // PARTICULEEMITTER_HPP
