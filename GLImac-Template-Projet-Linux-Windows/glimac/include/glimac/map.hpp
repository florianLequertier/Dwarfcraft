#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

class Chunk;
class Cube;
class VisualLoad;

#include <cstdlib>
#include <vector>
#include "vertexType.hpp"
#include "camera.h"
#include "Program.hpp"
#include "FilePath.hpp"
#include "perlin.h"
#include "chunk.hpp"
#include "lightManager.hpp"
#include "visualLoad.hpp"
#include "particuleManager.hpp"



struct Cube_program{

glimac::Program program;

GLuint uniformID_uModelMatrix;
GLuint uniformID_uViewMatrix;
GLuint uniformID_uProjectionMatrix;
GLuint uniformID_uTexture;

Cube_program()
{
    uniformID_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    uniformID_uViewMatrix = glGetUniformLocation(program.getGLId(), "uViewMatrix");
    uniformID_uProjectionMatrix = glGetUniformLocation(program.getGLId(), "uProjectionMatrix");
    uniformID_uTexture = glGetUniformLocation(program.getGLId(), "uTexture");
}

Cube_program(const glimac::FilePath& applicationPath): program(loadProgram(applicationPath.dirPath() + "shaders/tex2D.vs.glsl", applicationPath.dirPath() + "shaders/tex2D.fs.glsl"))
{
    uniformID_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    uniformID_uViewMatrix = glGetUniformLocation(program.getGLId(), "uViewMatrix");
    uniformID_uProjectionMatrix = glGetUniformLocation(program.getGLId(), "uProjectionMatrix");
    uniformID_uTexture = glGetUniformLocation(program.getGLId(), "uTexture");
}

};


struct CubeMultiLightProgram{

glimac::Program m_program;
GLuint uniformID_uModelMatrix;
GLuint uniformID_uViewMatrix;
GLuint uniformID_uProjectionMatrix;
GLuint uniformID_uMVP;
GLuint uniformID_uNormalMatrix;
///uniform pour la gestion du materiel :
GLuint uniform_uKd;
GLuint uniform_uKs;
GLuint uniform_uShininess;
//GLuint uniform_uLightDir_vs;
GLuint uniform_uLightDir_vs;
GLuint uniform_uLightPos_vs;
GLuint uniform_uPointLightIntensity;
GLuint uniform_uDirectionnalLightIntensity;
GLuint uniform_textureCube;
GLuint uniformID_uMVMatrix;
GLuint uniform_uNbPointLight;
GLuint uniform_uNbDirectionnalLight;





    CubeMultiLightProgram(const glimac::FilePath& applicationPath = ""): m_program( loadProgram(applicationPath.dirPath() + "shaders/mapCubeL.vs.glsl", applicationPath.dirPath() + "shaders/mapCubeL.fs.glsl")){
    uniformID_uModelMatrix = glGetUniformLocation(m_program.getGLId(), "uModelMatrix");
    uniformID_uViewMatrix = glGetUniformLocation(m_program.getGLId(), "uViewMatrix");
    uniformID_uMVP = glGetUniformLocation(m_program.getGLId(), "uMVP");
    uniformID_uProjectionMatrix = glGetUniformLocation(m_program.getGLId(), "uProjectionMatrix");
    uniformID_uNormalMatrix = glGetUniformLocation(m_program.getGLId(), "uNormalMatrix");
    uniformID_uMVMatrix = glGetUniformLocation(m_program.getGLId(), "uMVMatrix");

    uniform_uKd = glGetUniformLocation(m_program.getGLId(), "uKd");
    uniform_uKs = glGetUniformLocation(m_program.getGLId(), "uKs");
    uniform_uShininess = glGetUniformLocation(m_program.getGLId(), "uShininess");
    //uniform_uLightDir_vs = glGetUniformLocation(m_program.getGLId(), "uLightDir_vs");
    uniform_uLightDir_vs = glGetUniformLocation(m_program.getGLId(), "uLightDir_vs");
    uniform_uLightPos_vs = glGetUniformLocation(m_program.getGLId(), "uLightPos_vs");
    uniform_uPointLightIntensity = glGetUniformLocation(m_program.getGLId(), "uPointLightIntensity");
    uniform_uDirectionnalLightIntensity = glGetUniformLocation(m_program.getGLId(), "uDirectionnalLightIntensity");
    uniform_uNbPointLight = glGetUniformLocation(m_program.getGLId(), "uNbPointLight");
    uniform_uNbDirectionnalLight = glGetUniformLocation(m_program.getGLId(), "uNbDirectionnalLight");

    uniform_textureCube = glGetUniformLocation(m_program.getGLId(), "uTexture");

    }


};


class Map{
public: static int MAX_BIOME;
private:
std::vector<Chunk*> tab_Chunk;
int seed;
int nbChunk_x;
int nbChunk_y;
int nbChunk_z;
int nbCubeByRowByChunk;
std::vector<int> hauteurStrates;

Cube_program cubeProgram;
CubeMultiLightProgram cubeLightProgram;

GLuint textureCubes;
int nbTextureCube;

glm::vec3 positionSpawn;

std::vector<Cube*> tabHover;

public:
Map()
{

}

Map(const glimac::FilePath& applicationPath, GLuint _textureCube, int nbTexturecube, int _nbChunk_x, int _nbChunk_y, int _nbChunk_z, int _nbCubeByRowByChunk, int seed, int nbStrates, VisualLoad *visualLoad = NULL, glimac::SDLWindowManager *windowManager = NULL);

Map(const glimac::FilePath& applicationPath, int nbTexturecube, int _nbChunk_x, int _nbChunk_y, int _nbChunk_z, int _nbCubeByRowByChunk, int _seed, ParticuleManager &particuleManager , VisualLoad *visualLoad = NULL, glimac::SDLWindowManager *windowManager = NULL);

~Map();
//void Draw(glm::mat4 const& Projection, glm::mat4 const& View);
//void Draw(Camera camera);
void Draw(glm::mat4 const& Projection, glm::mat4 const& View, glm::vec3 position_chunk, GLuint textureCube);
void Draw(glm::mat4 const& Projection, glm::mat4 const& View, glm::vec3 position_chunk, GLuint textureCube, glm::vec3 forward);
void Draw(glm::mat4 const& Projection, glm::mat4 const& View, glm::vec3 position_chunk, GLuint textureCube, glm::vec3 forward, bool activeLight, LightManager &lightManager);
glm::vec3 getActiveChunkPosition(glm::vec3 actorPosition);
Chunk& getActiveChunk(glm::vec3 actorPosition);
Chunk* getActiveChunk_ptr(glm::vec3 actorPosition);
glm::vec3 getPositionInActiveChunk(glm::vec3 actorPosition);
int getNbCubeByRowByChunk() const;
glm::vec3 getNbChunk() const;
void spawnTree(glm::vec3 globalPosition ,int textureId_feuille, int textureId_tronc);
void spawnAllTrees(Cube** tabCubeSurfaciques);
glm::vec3 getPositionSpawn() const;
void updateTarget(glm::vec3 position);

void setPositionSpawn(glm::vec3 pos);
void setTabChunk(std::vector<Chunk*> tabchunk); /**/
std::vector<Chunk*> getAllChunk() const;/**/

Map& operator=(Map&& other )
{
    tab_Chunk = other.tab_Chunk;
    for(int i=0;i<other.tab_Chunk.size();i++)
    {
        other.tab_Chunk[i] = nullptr;
    }
    seed = other.seed;
    nbChunk_x = other.nbChunk_x;
    nbChunk_y = other.nbChunk_y;
    nbChunk_z = other.nbChunk_z;
    nbCubeByRowByChunk = other.nbCubeByRowByChunk;

    hauteurStrates = other.hauteurStrates;

    cubeProgram = std::move(other.cubeProgram);
    cubeLightProgram = std::move(other.cubeLightProgram);

    textureCubes = other.textureCubes;
    other.textureCubes = 0;

    nbTextureCube = other.nbTextureCube;
    positionSpawn = other.positionSpawn;

    tabHover = other.tabHover;
    for(int i=0;i<other.tabHover.size();i++)
    {
        other.tabHover[i] = nullptr;
    }

    return *this;

}

Map(Map&& other)
{
    tab_Chunk = other.tab_Chunk;
    for(int i=0;i<other.tab_Chunk.size();i++)
    {
        other.tab_Chunk[i] = nullptr;
    }
    seed = other.seed;
    nbChunk_x = other.nbChunk_x;
    nbChunk_y = other.nbChunk_y;
    nbChunk_z = other.nbChunk_z;
    nbCubeByRowByChunk = other.nbCubeByRowByChunk;

    hauteurStrates = other.hauteurStrates;

    cubeProgram = std::move(other.cubeProgram);
    cubeLightProgram = std::move(other.cubeLightProgram);

    textureCubes = other.textureCubes;
    other.textureCubes = 0;

    nbTextureCube = other.nbTextureCube;
    positionSpawn = other.positionSpawn;

    tabHover = other.tabHover;
    for(int i=0;i<other.tabHover.size();i++)
    {
        other.tabHover[i] = nullptr;
    }
}


};


#endif // MAP_HPP_INCLUDED
